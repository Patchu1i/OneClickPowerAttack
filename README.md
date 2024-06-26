# One Click Power Attack NG - Fixed

## What this is:

This code is derived from the One Click Power Attack - NG [source](https://github.com/jarari/OneClickPowerAttack), and restructured and fitted to [alandtse's](https://github.com/alandtse/CommonLibVR/tree/ng) fork of CommonLibSSE-NG. In addition, I made some logical changes to address issues with blocking, and bash behavior to fix outstanding bugs in the original source. 

## Requirements

- Any terminal of your choice (e.g., PowerShell)
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
  - Desktop development with C++
- [CMake](https://cmake.org/)
  - Edit the `PATH` environment variable and add the cmake.exe install path as a new value
  - Instructions for finding and editing the `PATH` environment variable can be found [here](https://www.java.com/en/download/help/path.html)
- [Git](https://git-scm.com/downloads)
  - Edit the `PATH` environment variable and add the Git.exe install path as a new value
- [Vcpkg](https://github.com/microsoft/vcpkg)
  - Install vcpkg using the directions in vcpkg's [Quick Start Guide](https://github.com/microsoft/vcpkg#quick-start-windows)
  - After install, add a new environment variable named `VCPKG_ROOT` with the value as the path to the folder containing vcpkg

## Clone and Build
- Clone this repository
- Open project in Visual Studio Code (or Studio)
- Run CMAKE configuration "ALL"
- Build

(note): The git submodule should automatically init and update recursively on CMake update. If it fails, you'll have to do it manually.

