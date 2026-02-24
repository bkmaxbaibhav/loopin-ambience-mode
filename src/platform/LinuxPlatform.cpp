#include "LinuxPlatform.h"

bool LinuxPlatform::init(void* windowHandle) {
    // TODO Session 7: detect X11 vs Wayland, setup overlay window
    return false;
}

void LinuxPlatform::makeClickThrough() {
    // TODO Session 7: XFixesCreateRegion, XShapeCombineRectangles
}

void LinuxPlatform::setAlwaysOnTop() {
    // TODO Session 7: _NET_WM_STATE_ABOVE atom
}

void LinuxPlatform::registerHotkey() {
    // TODO Session 9: XGrabKey Ctrl+Alt+V
}
