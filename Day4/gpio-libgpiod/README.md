# Building the Program with CMake

This project uses CMake to manage the build process. Follow the steps below to build the program.

## Prerequisites

Make sure you have CMake installed on your system. You can install it using your package manager. 

For example, on Ubuntu:
```bash
sudo apt-get update
sudo apt-get install cmake
```

## Create a build directory
```bash
mkdir build
cd build
```

## Run CMake to configure the project
```bash
cmake ..
```

## Build the project
```bash
make
```

## Cleaning Up
To clean up the build files, you can simply remove the build directory:
```bash
rm -rf build
```