#include "LinuxPlatform.h"
#include <iostream>
#include <cstring>
#include <dlfcn.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>

#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Helper to get GLFW native functions dynamically to avoid link errors
// when the system GLFW only supports one backend.
template<typename T>
T getGlfwSymbol(const char* name) {
    void* symbol = dlsym(RTLD_DEFAULT, name);
    return reinterpret_cast<T>(symbol);
}

bool LinuxPlatform::init(void* windowHandle) {
    windowHandle_ = windowHandle;

    const char* waylandDisp = getenv("WAYLAND_DISPLAY");
    bool isWayland = waylandDisp && strlen(waylandDisp) > 0;

    if (isWayland) {
        if (initWayland(windowHandle)) {
            displayServer_ = DisplayServer::WAYLAND;
            return true;
        }

        const char* currentDesktop = getenv("XDG_CURRENT_DESKTOP");
        if (currentDesktop && strstr(currentDesktop, "GNOME")) {
            std::cout << "[AMBIENCE] GNOME Wayland detected — layer-shell unavailable, falling back to xwayland if available" << std::endl;
        } else {
            std::cout << "[AMBIENCE] Wayland layer-shell initialization failed, attempting XWayland fallback" << std::endl;
        }
    }

    const char* x11Disp = getenv("DISPLAY");
    if (x11Disp && strlen(x11Disp) > 0) {
        if (initX11(windowHandle)) {
            displayServer_ = DisplayServer::X11;
            return true;
        }
    }

    if (isWayland && (!x11Disp || strlen(x11Disp) == 0)) {
        std::cout << "[AMBIENCE] No XWayland available on this compositor — overlay may not behave correctly" << std::endl;
    } else {
        std::cerr << "[AMBIENCE] No suitable display server detected or initialization failed" << std::endl;
    }

    displayServer_ = DisplayServer::UNKNOWN;
    return false;
}

bool LinuxPlatform::initWayland(void* windowHandle) {
    auto getDisplay = getGlfwSymbol<wl_display*(*)()>("glfwGetWaylandDisplay");
    auto getWindow = getGlfwSymbol<wl_surface*(*)(GLFWwindow*)>("glfwGetWaylandWindow");

    if (!getDisplay || !getWindow) {
        return false;
    }

    GLFWwindow* window = static_cast<GLFWwindow*>(windowHandle);
    wlDisplay_ = getDisplay();
    wlSurface_ = getWindow(window);

    if (!wlDisplay_ || !wlSurface_) {
        return false;
    }

    wlRegistry_ = wl_display_get_registry(wlDisplay_);

    static const wl_registry_listener registryListener = {
        .global = [](void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
            auto self = static_cast<LinuxPlatform*>(data);
            if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
                self->layerShell_ = (zwlr_layer_shell_v1*)wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, 1);
            } else if (strcmp(interface, wl_compositor_interface.name) == 0) {
                self->wlCompositor_ = (wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
            }
        },
        .global_remove = [](void*, wl_registry*, uint32_t) {}
    };

    wl_registry_add_listener(wlRegistry_, &registryListener, this);
    wl_display_roundtrip(wlDisplay_);

    if (!layerShell_) {
        return false;
    }

    if (!wlCompositor_) {
        std::cerr << "[AMBIENCE] wl_compositor not found" << std::endl;
        return false;
    }

    layerSurface_ = zwlr_layer_shell_v1_get_layer_surface(layerShell_, wlSurface_, NULL, ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY, "loopin-ambience-mode");
    zwlr_layer_surface_v1_set_size(layerSurface_, 0, 0);
    zwlr_layer_surface_v1_set_anchor(layerSurface_, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT);
    zwlr_layer_surface_v1_set_exclusive_zone(layerSurface_, -1);

    static const zwlr_layer_surface_v1_listener layerSurfaceListener = {
        .configure = [](void* data, zwlr_layer_surface_v1* surface, uint32_t serial, uint32_t width, uint32_t height) {
            (void)data; (void)width; (void)height;
            zwlr_layer_surface_v1_ack_configure(surface, serial);
        },
        .closed = [](void*, zwlr_layer_surface_v1*) {}
    };

    zwlr_layer_surface_v1_add_listener(layerSurface_, &layerSurfaceListener, this);

    wl_region* inputRegion = wl_compositor_create_region(wlCompositor_);
    wl_surface_set_input_region(wlSurface_, inputRegion);
    wl_region_destroy(inputRegion);

    wl_surface_commit(wlSurface_);
    wl_display_roundtrip(wlDisplay_);

    std::cout << "[AMBIENCE] Wayland layer-shell initialized" << std::endl;
    return true;
}

bool LinuxPlatform::initX11(void* windowHandle) {
    auto getDisplay = getGlfwSymbol<Display*(*)()>("glfwGetX11Display");
    auto getWindow = getGlfwSymbol<Window(*)(GLFWwindow*)>("glfwGetX11Window");

    if (!getDisplay || !getWindow) {
        return false;
    }

    GLFWwindow* window = static_cast<GLFWwindow*>(windowHandle);
    xDisplay_ = getDisplay();
    xWindow_ = getWindow(window);

    if (!xDisplay_ || !xWindow_) {
        return false;
    }

    std::cout << "[AMBIENCE] X11 backend initialized" << std::endl;
    displayServer_ = DisplayServer::X11;
    makeClickThrough();
    setAlwaysOnTop();
    return true;
}

void LinuxPlatform::makeClickThrough() {
    if (displayServer_ != DisplayServer::X11) return;

    // Unmap the window first so that the transition to override_redirect takes effect on mapping.
    XUnmapWindow(xDisplay_, xWindow_);

    XSetWindowAttributes attrs;
    attrs.override_redirect = True;
    XChangeWindowAttributes(xDisplay_, xWindow_, CWOverrideRedirect, &attrs);

    Atom wmWindowType = XInternAtom(xDisplay_, "_NET_WM_WINDOW_TYPE", False);
    Atom wmWindowTypeDock = XInternAtom(xDisplay_, "_NET_WM_WINDOW_TYPE_DOCK", False);
    XChangeProperty(xDisplay_, xWindow_, wmWindowType, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wmWindowTypeDock, 1);

    Atom wmState = XInternAtom(xDisplay_, "_NET_WM_STATE", False);
    Atom wmStateSkipTaskbar = XInternAtom(xDisplay_, "_NET_WM_STATE_SKIP_TASKBAR", False);
    Atom wmStateSkipPager = XInternAtom(xDisplay_, "_NET_WM_STATE_SKIP_PAGER", False);
    Atom wmStateAbove = XInternAtom(xDisplay_, "_NET_WM_STATE_ABOVE", False);
    Atom states[] = { wmStateSkipTaskbar, wmStateSkipPager, wmStateAbove };
    XChangeProperty(xDisplay_, xWindow_, wmState, XA_ATOM, 32, PropModeReplace, (unsigned char*)states, 3);

    int event, error;
    if (!XFixesQueryExtension(xDisplay_, &event, &error)) {
        std::cerr << "[AMBIENCE] XFixes extension not available" << std::endl;
    } else {
        XserverRegion region = XFixesCreateRegion(xDisplay_, NULL, 0);
        XFixesSetWindowShapeRegion(xDisplay_, xWindow_, ShapeInput, 0, 0, region);
        XFixesDestroyRegion(xDisplay_, region);
    }

    // Map the window again so the window manager (e.g. GNOME Mutter) sees it as override_redirect.
    XMapWindow(xDisplay_, xWindow_);
    XFlush(xDisplay_);
}

void LinuxPlatform::setAlwaysOnTop() {
    if (displayServer_ != DisplayServer::X11) return;

    Atom wmState = XInternAtom(xDisplay_, "_NET_WM_STATE", False);
    Atom wmStateAbove = XInternAtom(xDisplay_, "_NET_WM_STATE_ABOVE", False);

    XChangeProperty(xDisplay_, xWindow_, wmState, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wmStateAbove, 1);

    XEvent event = {};
    event.type = ClientMessage;
    event.xclient.window = xWindow_;
    event.xclient.message_type = wmState;
    event.xclient.format = 32;
    event.xclient.data.l[0] = 1; // _NET_WM_STATE_ADD
    event.xclient.data.l[1] = wmStateAbove;
    event.xclient.data.l[2] = 0;
    event.xclient.data.l[3] = 0;
    event.xclient.data.l[4] = 0;

    XSendEvent(xDisplay_, DefaultRootWindow(xDisplay_), False, SubstructureNotifyMask | SubstructureRedirectMask, &event);
    XMapRaised(xDisplay_, xWindow_);
    XRaiseWindow(xDisplay_, xWindow_);
    XFlush(xDisplay_);
}

void LinuxPlatform::registerHotkey() {
    // Intentionally empty.
    // Global hotkey on Linux is handled by the
    // dedicated Hotkey class using XGrabKey directly.
    // This override exists only to satisfy the
    // Platform abstract interface.
}
