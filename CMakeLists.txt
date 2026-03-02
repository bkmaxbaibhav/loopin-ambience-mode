cmake_minimum_required(VERSION 3.20)
project(loopin-ambience-mode VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(PkgConfig REQUIRED)

# PortAudio
find_package(portaudio QUIET)
if(NOT portaudio_FOUND)
    pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)
endif()

# FFTW3
find_package(FFTW3f QUIET)
if(NOT FFTW3f_FOUND)
    pkg_check_modules(FFTW3 REQUIRED fftw3f)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(
    src
    third_party
    ${PORTAUDIO_INCLUDE_DIRS}
    ${FFTW3_INCLUDE_DIRS}
)

# Core sources
set(SOURCES
    src/main.cpp
    src/audio/AudioCapture.cpp
    src/audio/FFTProcessor.cpp
    src/renderer/Overlay.cpp
    src/renderer/Shader.cpp
    src/renderer/BandMapper.cpp
    src/settings/Config.cpp
    third_party/glad/glad.c
)

add_executable(loopin-ambience-mode ${SOURCES})

# Link common dependencies
target_link_libraries(loopin-ambience-mode PRIVATE
    OpenGL::GL
    glfw
    nlohmann_json::nlohmann_json
)

# Link PortAudio
if(TARGET portaudio)
    target_link_libraries(loopin-ambience-mode PRIVATE portaudio)
elseif(TARGET PortAudio::PortAudio)
    target_link_libraries(loopin-ambience-mode PRIVATE PortAudio::PortAudio)
else()
    target_link_libraries(loopin-ambience-mode PRIVATE ${PORTAUDIO_LIBRARIES})
endif()

# Link FFTW3
if(TARGET FFTW3f::fftw3f)
    target_link_libraries(loopin-ambience-mode PRIVATE FFTW3f::fftw3f)
elseif(TARGET FFTW3::fftw3)
    target_link_libraries(loopin-ambience-mode PRIVATE FFTW3::fftw3)
else()
    target_link_libraries(loopin-ambience-mode PRIVATE ${FFTW3_LIBRARIES})
endif()

# Conditional platform compilation
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    pkg_check_modules(APPINDICATOR REQUIRED appindicator3-0.1)
    pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
    pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
    pkg_get_variable(WAYLAND_SCANNER wayland-scanner wayland_scanner)

    set(XDG_SHELL_XML /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml)
    set(XDG_SHELL_SRC ${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c)
    set(XDG_SHELL_HDR ${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h)

    set(WLR_LAYER_SHELL_XML ${CMAKE_SOURCE_DIR}/third_party/wlr-layer-shell/wlr-layer-shell-unstable-v1.xml)
    set(WLR_LAYER_SHELL_SRC ${CMAKE_CURRENT_BINARY_DIR}/wlr-layer-shell-unstable-v1-protocol.c)
    set(WLR_LAYER_SHELL_HDR ${CMAKE_CURRENT_BINARY_DIR}/wlr-layer-shell-unstable-v1-client-protocol.h)

    add_custom_command(
        OUTPUT ${XDG_SHELL_SRC} ${XDG_SHELL_HDR}
        COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_HDR}
        COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_SRC}
        DEPENDS ${XDG_SHELL_XML}
        COMMENT "Generating Wayland protocol bindings for xdg-shell"
    )

    add_custom_command(
        OUTPUT ${WLR_LAYER_SHELL_SRC} ${WLR_LAYER_SHELL_HDR}
        COMMAND ${WAYLAND_SCANNER} client-header ${WLR_LAYER_SHELL_XML} ${WLR_LAYER_SHELL_HDR}
        COMMAND ${WAYLAND_SCANNER} private-code ${WLR_LAYER_SHELL_XML} ${WLR_LAYER_SHELL_SRC}
        DEPENDS ${WLR_LAYER_SHELL_XML}
        COMMENT "Generating Wayland protocol bindings for wlr-layer-shell"
    )

    target_sources(loopin-ambience-mode PRIVATE
        src/platform/Tray.cpp
        src/platform/Hotkey.cpp
        src/platform/AutoStart.cpp
        src/platform/LinuxPlatform.cpp
        ${XDG_SHELL_SRC}
        ${XDG_SHELL_HDR}
        ${WLR_LAYER_SHELL_SRC}
        ${WLR_LAYER_SHELL_HDR}
    )
    target_include_directories(loopin-ambience-mode PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${APPINDICATOR_INCLUDE_DIRS})
    target_link_libraries(loopin-ambience-mode PRIVATE
        ${APPINDICATOR_LIBRARIES}
        X11 Xfixes
        ${WAYLAND_CLIENT_LIBRARIES}
        ${CMAKE_DL_LIBS}
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(loopin-ambience-mode PRIVATE
        src/platform/Tray.cpp
        src/platform/Hotkey.cpp
        src/platform/AutoStart.cpp
        src/platform/WindowsPlatform.cpp)
    target_link_libraries(loopin-ambience-mode PRIVATE
        dwmapi)
endif()

# Test target
add_executable(loopin-ambience-fft-test src/audio/fft_test.cpp src/audio/FFTProcessor.cpp)
if(TARGET FFTW3f::fftw3f)
    target_link_libraries(loopin-ambience-fft-test PRIVATE FFTW3f::fftw3f)
elseif(TARGET FFTW3::fftw3)
    target_link_libraries(loopin-ambience-fft-test PRIVATE FFTW3::fftw3)
else()
    target_link_libraries(loopin-ambience-fft-test PRIVATE ${FFTW3_LIBRARIES})
endif()

# Post-build copy commands
add_custom_command(TARGET loopin-ambience-mode POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:loopin-ambience-mode>/shaders)

add_custom_command(TARGET loopin-ambience-mode POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config
    $<TARGET_FILE_DIR:loopin-ambience-mode>/config)
