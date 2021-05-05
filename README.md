# RakCloudDesktop

RakCloudDesktop is the official client of [RakCloud](https://rakcloud.ru).

RakCloud — A cloud-based platform for building fully automated bot systems of any size in SA-MP and CR-MP.

Look at the documentation: [wiki.rinwares.com](https://wiki.rinwares.com/rakcloud)

## Build instructions

#### Required software
* [Qt Creator](https://www.qt.io/product/development-tools)
* [CMake](https://cmake.org)
* [Ninja](https://ninja-build.org)
* [Vcpkg](https://github.com/microsoft/vcpkg)
* [Visual Studio 2019](https://visualstudio.microsoft.com), [GCC](https://gcc.gnu.org) or [Clang](https://clang.llvm.org)

#### Install libraries
```bash
vcpkg install protobuf
vcpkg install openssl
vcpkg install jwt-cpp
```

#### Building
The assembly can be done in different ways, for example:
1. Open `CMakeLists.txt` in Qt Creator and click build button.
2. Use `mkdir build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake .. && cmake --build .`.

#### Deployment
In order to collect all the libraries necessary for the work, use: [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html), [linuxdeployqt](https://github.com/probonopd/linuxdeployqt) or [macdeployqt](https://doc.qt.io/qt-5/macos-deployment.html).
Also manually copy libraries from OpenSSL (libcrypto-1_1, libssl-1_1) and Protobuf (libprotobuf), if you installed them using vcpkg you can find them in the following path: `[path to vcpkg]/installed/[triplet]/bin`.
Also create a folder `languages` and move the translation files with the extension `.qm` into it, in the same folder there should also be a folder `icons` containing icons for languages.

## Third-party

* Qt ([LGPL](http://doc.qt.io/qt-5/lgpl.html))
* Protobuf ([New BSD License](https://github.com/protocolbuffers/protobuf/blob/master/LICENSE))
* OpenSSL ([Apache License 2.0](https://www.openssl.org/source/license.html))
* jwt-cpp ([MIT License](https://github.com/Thalhammer/jwt-cpp/blob/master/LICENSE))
* Ninja ([Apache License 2.0](https://github.com/ninja-build/ninja/blob/master/COPYING))
* CMake ([New BSD License](https://github.com/Kitware/CMake/blob/master/Copyright.txt))

## License
The source code is published under GPLv3 with an exception for OpenSSL and jwt-cpp, the license is available [here](LICENSE).
