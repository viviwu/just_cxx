#!/bin/bash

if [ -d "build" ]; then
	echo "Removing existing build directory..."
    rm -rf build
fi

echo "Creating new build directory..."
mkdir build

cd build

echo "Generating Makefile with CMake..."
cmake ..

echo "Building project with Make..."
make -j8

if [ $? -eq 0 ]; then
	echo "Build succeeded."
else
	echo "Build failed. Please check the output for errors."
	exit 1
fi