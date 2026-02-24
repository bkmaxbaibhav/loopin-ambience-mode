#include "WindowsPlatform.h"

bool WindowsPlatform::init(void* windowHandle) {
    // TODO Session 8: apply WS_EX_LAYERED | WS_EX_TRANSPARENT
    return false;
}

void WindowsPlatform::makeClickThrough() {
    // TODO Session 8: SetLayeredWindowAttributes
}

void WindowsPlatform::setAlwaysOnTop() {
    // TODO Session 8: SetWindowPos HWND_TOPMOST
}

void WindowsPlatform::registerHotkey() {
    // TODO Session 9: RegisterHotKey Ctrl+Alt+V
}
