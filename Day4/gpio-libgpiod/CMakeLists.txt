cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GpioThreadsDemo)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/)

# Add the executable
add_executable(gpio_threads_demo main.c libgpiod.c)

# Link the library to the executable
target_link_libraries(gpio_threads_demo gpiod)
