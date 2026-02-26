cmake_minimum_required(VERSION 3.20)
project(loopin-ambience-mode VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(PkgConfig REQUIRED)

# PortAudio: Try find_package first (standard for Windows/Vcpkg), fallback to pkg-config (standard for Linux)
find_package(portaudio QUIET)
if(NOT portaudio_FOUND)
    pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)
endif()

# FFTW3: Try find_package first, fallback to pkg-config
# We need the float version (fftw3f) for single precision
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
    target_sources(loopin-ambience-mode PRIVATE
        src/platform/LinuxPlatform.cpp)
    target_link_libraries(loopin-ambience-mode PRIVATE
        X11 Xfixes)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(loopin-ambience-mode PRIVATE
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
