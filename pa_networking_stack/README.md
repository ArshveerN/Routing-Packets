CSC 358 Programming Assignments
==============================

This repository contains programming assignments of CSC 358 course. They are based on Stanford's CS 144 assignments.

To set up the build system: `cmake -S . -B build`

To compile: `cmake --build build`

To run tests: `cmake --build build --target pa2`

To run clang-tidy (which suggests improvements): `cmake --build build --target tidy`

To format code: `cmake --build build --target format`
