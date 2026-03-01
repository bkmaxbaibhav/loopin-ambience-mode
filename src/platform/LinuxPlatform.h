#pragma once
#include "Platform.h"
#include <X11/Xlib.h>

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

    Display* xDisplay_ = nullptr;
    Window xWindow_ = 0;
    void* windowHandle_ = nullptr;
};
