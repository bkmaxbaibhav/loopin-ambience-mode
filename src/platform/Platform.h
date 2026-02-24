#pragma once

// Abstract interface for platform-specific window management
// Implemented by LinuxPlatform and WindowsPlatform
class Platform {
public:
    virtual ~Platform() = default;
    virtual bool init(void* windowHandle) = 0;
    virtual void makeClickThrough()       = 0;
    virtual void setAlwaysOnTop()         = 0;
    virtual void registerHotkey()         = 0;
};
