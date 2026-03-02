#include "Hotkey.h"
#include <X11/Xutil.h>
#include <iostream>

Hotkey::Hotkey() {}

Hotkey::~Hotkey() {
    if (display_ && root_ && keyCode_) {
        XUngrabKey(display_, keyCode_, ControlMask | Mod1Mask, root_);
        XUngrabKey(display_, keyCode_, ControlMask | Mod1Mask | LockMask, root_);
        XUngrabKey(display_, keyCode_, ControlMask | Mod1Mask | Mod2Mask, root_);
        XUngrabKey(display_, keyCode_, ControlMask | Mod1Mask | LockMask | Mod2Mask, root_);
    }
}

bool Hotkey::init(Display* display, Window rootWindow) {
    if (!display) return false;
    display_ = display;
    root_ = rootWindow;

    keyCode_ = XKeysymToKeycode(display_, XStringToKeysym("v"));
    if (!keyCode_) {
        std::cerr << "[AMBIENCE] Failed to find keycode for 'v'" << std::endl;
        return false;
    }

    // Ctrl + Alt + V
    XGrabKey(display_, keyCode_, ControlMask | Mod1Mask, root_, True, GrabModeAsync, GrabModeAsync);
    // With Lock keys
    XGrabKey(display_, keyCode_, ControlMask | Mod1Mask | LockMask, root_, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display_, keyCode_, ControlMask | Mod1Mask | Mod2Mask, root_, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display_, keyCode_, ControlMask | Mod1Mask | LockMask | Mod2Mask, root_, True, GrabModeAsync, GrabModeAsync);

    std::cout << "[AMBIENCE] Hotkey registered: Ctrl+Alt+V" << std::endl;
    return true;
}

void Hotkey::poll() {
    if (!display_) return;

    XEvent event;
    while (XPending(display_)) {
        XNextEvent(display_, &event);
        if (event.type == KeyPress) {
            if (event.xkey.keycode == keyCode_) {
                if (onToggle_) onToggle_();
            }
        }
    }
}

void Hotkey::setOnToggle(std::function<void()> cb) {
    onToggle_ = cb;
}
