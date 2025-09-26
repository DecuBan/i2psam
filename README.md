![Linux](https://img.shields.io/badge/-Linux-grey?logo=linux)
![c++20](https://img.shields.io/badge/cpp-c%2B%2B20-green)

# i2psam

A C++ library for the [SAM v3 API](https://geti2p.net/en/docs/api/samv3).

## Development Status

The library will require SAM v3.1 server.  
Pre-release (ongoing refactoring work and migration to C++20).

## Build

* **Build presets:**

* Debug
* Release

1. **Configure**

    ```bash
    cmake --preset=release
    ```

2. **Build**

    ```bash
    cmake --build --clean-first --preset=release
    ```

## Installation

* **Install**

1. **Clone the repository**

   ```bash
   git clone https://github.com/DecuBan/i2psam.git && cd i2psam
   ```

2. **Compile**

    ``` bash
    cmake --preset=release
    ```
    
    ```bash
    cmake --build --preset=release
    ```
3. **Install**

    ```bash
    sudo cmake --install build
    ```

* **Uninstall**

    ```bash
    sudo cmake --build build --target uninstall
    ```

## LICENSE
[![LICENSE](https://img.shields.io/github/license/i2p/i2psam.svg)](LICENSE)
