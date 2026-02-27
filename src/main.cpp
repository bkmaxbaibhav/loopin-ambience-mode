#include "renderer/Overlay.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "loopin-ambience-mode starting..." << std::endl;

    Overlay overlay;
    if (!overlay.init()) {
        std::cerr << "Failed to initialize overlay." << std::endl;
        return 1;
    }

    std::cout << "Starting 60fps render loop..." << std::endl;

    while (!overlay.shouldClose()) {
        auto frameStart = std::chrono::steady_clock::now();

        overlay.render(0.0f, 0.0f, 0.0f);

        auto frameEnd = std::chrono::steady_clock::now();
        auto elapsed  = frameEnd - frameStart;
        auto target   = std::chrono::microseconds(16667); // ~60fps

        if (elapsed < target) {
            std::this_thread::sleep_for(target - elapsed);
        }
    }

    overlay.shutdown();
    std::cout << "loopin-ambience-mode shutdown." << std::endl;

    return 0;
}
