#pragma once
#include "Platform.h"

// Linux implementation using X11 + XFixes for click-through
// and wlr-layer-shell for Wayland support
class LinuxPlatform : public Platform {
public:
    bool init(void* windowHandle) override;
    void makeClickThrough()       override;
    void setAlwaysOnTop()         override;
    void registerHotkey()         override;
};
