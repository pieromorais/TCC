cmake_minimum_required(VERSION 3.8)
project(tilted_camera)

# Enable the generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add your source files
file(GLOB_RECURSE sources "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add your include directories
include_directories("${CMAKE_SOURCE_DIR}/include")

# Add your executable
add_executable(main ${sources})

# find OpenCV package
find_package(OpenCV REQUIRED)

# add linkable headers from sfml
target_link_libraries(main opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_videoio)
