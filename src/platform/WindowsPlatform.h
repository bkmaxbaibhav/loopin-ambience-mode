#pragma once
#include "Platform.h"

// Windows implementation using Win32 layered window
// WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST
class WindowsPlatform : public Platform {
public:
    bool init(void* windowHandle) override;
    void makeClickThrough()       override;
    void setAlwaysOnTop()         override;
    void registerHotkey()         override;
};
