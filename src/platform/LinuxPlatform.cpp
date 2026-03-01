#include "LinuxPlatform.h"
#include <iostream>
#include <cstring>
#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

bool LinuxPlatform::init(void* windowHandle) {
    windowHandle_ = windowHandle;
    GLFWwindow* window = static_cast<GLFWwindow*>(windowHandle);

    const char* wayland = getenv("WAYLAND_DISPLAY");
    if (wayland && strlen(wayland) > 0) {
        displayServer_ = DisplayServer::WAYLAND;
        std::cout << "[AMBIENCE] Wayland detected — X11 backend not applicable, use Session 7b" << std::endl;
        return false;
    }

    const char* display = getenv("DISPLAY");
    if (display && strlen(display) > 0) {
        displayServer_ = DisplayServer::X11;
        std::cout << "[AMBIENCE] X11 detected, proceeding with init" << std::endl;

        xDisplay_ = glfwGetX11Display();
        xWindow_ = glfwGetX11Window(window);

        if (!xDisplay_ || !xWindow_) {
            std::cerr << "[AMBIENCE] Failed to get X11 display or window" << std::endl;
            return false;
        }

        makeClickThrough();
        setAlwaysOnTop();

        return true;
    }

    std::cerr << "[AMBIENCE] No display server detected" << std::endl;
    displayServer_ = DisplayServer::UNKNOWN;
    return false;
}

void LinuxPlatform::makeClickThrough() {
    if (displayServer_ != DisplayServer::X11) return;

    // 1. Apply override-redirect
    XSetWindowAttributes attrs;
    attrs.override_redirect = True;
    XChangeWindowAttributes(xDisplay_, xWindow_, CWOverrideRedirect, &attrs);

    // 2. Remove window from taskbar by setting window type to dock
    Atom wmWindowType = XInternAtom(xDisplay_, "_NET_WM_WINDOW_TYPE", False);
    Atom wmWindowTypeDock = XInternAtom(xDisplay_, "_NET_WM_WINDOW_TYPE_DOCK", False);
    XChangeProperty(xDisplay_, xWindow_, wmWindowType, XA_ATOM, 32,
                    PropModeReplace, (unsigned char*)&wmWindowTypeDock, 1);

    // 3. Remove from pager and taskbar using state atoms
    Atom wmState = XInternAtom(xDisplay_, "_NET_WM_STATE", False);
    Atom wmStateSkipTaskbar = XInternAtom(xDisplay_, "_NET_WM_STATE_SKIP_TASKBAR", False);
    Atom wmStateSkipPager = XInternAtom(xDisplay_, "_NET_WM_STATE_SKIP_PAGER", False);
    Atom states[] = { wmStateSkipTaskbar, wmStateSkipPager };
    XChangeProperty(xDisplay_, xWindow_, wmState, XA_ATOM, 32, PropModeReplace,
                    (unsigned char*)states, 2);

    // 4. Implement XFixes click-through input region
    int event, error;
    if (!XFixesQueryExtension(xDisplay_, &event, &error)) {
        std::cerr << "[AMBIENCE] XFixes extension not available" << std::endl;
    } else {
        XserverRegion region = XFixesCreateRegion(xDisplay_, NULL, 0);
        XFixesSetWindowShapeRegion(xDisplay_, xWindow_, ShapeInput, 0, 0, region);
        XFixesDestroyRegion(xDisplay_, region);
    }

    XFlush(xDisplay_);
}

void LinuxPlatform::setAlwaysOnTop() {
    if (displayServer_ != DisplayServer::X11) return;

    Atom wmState = XInternAtom(xDisplay_, "_NET_WM_STATE", False);
    Atom wmStateAbove = XInternAtom(xDisplay_, "_NET_WM_STATE_ABOVE", False);

    // Set the property
    XChangeProperty(xDisplay_, xWindow_, wmState, XA_ATOM, 32, PropModeReplace,
                    (unsigned char*)&wmStateAbove, 1);

    // Send ClientMessage to root window
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

    XSendEvent(xDisplay_, DefaultRootWindow(xDisplay_), False,
               SubstructureNotifyMask | SubstructureRedirectMask, &event);

    XFlush(xDisplay_);
}

void LinuxPlatform::registerHotkey() {
    // TODO Session 9: XGrabKey Ctrl+Alt+V
}
