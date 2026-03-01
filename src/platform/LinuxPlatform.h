#pragma once
#include "Platform.h"
#include <X11/Xlib.h>
#include <wayland-client.h>
#include "wlr-layer-shell-unstable-v1-client-protocol.h"

// Linux implementation using X11 + XFixes for click-through
// and wlr-layer-shell for Wayland support
class LinuxPlatform : public Platform {
public:
    bool init(void* windowHandle) override;
    void makeClickThrough()       override;
    void setAlwaysOnTop()         override;
    void registerHotkey()         override;

private:
    enum class DisplayServer { X11, WAYLAND, UNKNOWN };
    DisplayServer displayServer_ = DisplayServer::UNKNOWN;

    // X11 members
    Display* xDisplay_ = nullptr;
    Window xWindow_ = 0;

    // Wayland members
    wl_display* wlDisplay_ = nullptr;
    wl_surface* wlSurface_ = nullptr;
    wl_registry* wlRegistry_ = nullptr;
    wl_compositor* wlCompositor_ = nullptr;
    zwlr_layer_shell_v1* layerShell_ = nullptr;
    zwlr_layer_surface_v1* layerSurface_ = nullptr;

    void* windowHandle_ = nullptr;

    bool initWayland(void* windowHandle);
    bool initX11(void* windowHandle);
};
