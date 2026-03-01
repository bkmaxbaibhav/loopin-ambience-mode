#pragma once
#include <X11/Xlib.h>
#include <functional>

class Hotkey {
public:
    Hotkey();
    ~Hotkey();

    bool init(Display* display, Window rootWindow);
    void poll();
    void setOnToggle(std::function<void()> cb);

private:
    Display* display_ = nullptr;
    Window root_ = 0;
    std::function<void()> onToggle_;
    int keyCode_ = 0;
};
