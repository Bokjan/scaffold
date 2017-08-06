# What is scaffold
`scaffold` is a cross-platform toy web framework. I develop it for finishing my C++ course.

Inspired by [`Express`](https://expressjs.com), `scaffold` is a minimal and flexible C++ web application framework that provides a set of features for web and mobile applications, and provides a thin layer of fundamental web application features.

# Environment
To build this project, `cmake`, `make` and a proper C++ compiler is required. The compiler should at least support C++14 standard.

I use macOS 10.12.5 for developing and Fedora 25 for testing. With the help of [cesanta/mongoose](https://github.com/cesanta/mongoose), `scaffold` can run on various operating systems and hardware architectures.

# Dependencies
- [cesanta/mongoose](https://github.com/cesanta/mongoose)
- [openssl/openssl](https://github.com/openssl/openssl) (optional)

# How to use
Firstly, we need to build the `scaffold` library.

1. Clone the repository.
   ```bash
   git clone https://github.com/Bokjan/scaffold
   ```
2. Create a directory for building.
   ```bash
   cd scaffold
   mkdir build && cd build
   ```
   All the following commands run under `/path/to/scaffold/build`.
3. Run CMake to configure the project.
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=RELEASE
   ```
   If you need HTTPS support, add a definition when configuring.
   ```bash
   cmake .. -DENABLE_SSL=1
   ```
   Note that `OpenSSL` is required (by `Mongoose`). If your environment cannot automatically provide this, you need to specify the paths (following is an example).
   ```bash
   cmake .. -DENABLE_SSL=1 \
            -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl \
            -DOPENSSL_LIBRARIES=/usr/local/opt/openssl/lib \
            -DOPENSSL_INCLUDE_DIR=/usr/local/opt/openssl/include
   ```
   You can set environment variables. For example, you specify `clang++` as the C++ compiler by setting the `CXX` environment variable as following:
   ```bash
   CXX=clang++ cmake ..
   ```
   You can specify the root directory where program files will install by defining `CMAKE_INSTALL_PREFIX` (default: `/usr/local`). If you would like to install `scaffold` in `/home/bokjan/install`, run:
   ```bash
   cmake .. -DCMAKE_INSTALL_PREFIX=/home/bokjan/install
   ```

4. Build and install.
   ```bash
   make && make install
   ```
   Add `VERBOSE=1` to display the programs invoked by `make`: `VERBOSE=1 make`.

After running `make install`, library (both static and dynamic) and headers are installed. You can build web applications with `scaffold` now. Usually, you don't need to manually specify library informations if you use default install configuration.

Following are some demonstrations of specifying the path:
- Using command line (dynamic linking)
    ```bash
    g++ -o app app.cpp -I/path/to/headers -L/path/to/libraries -lscaffold
    ```
- Using CMake (static linking)
    ```cmake
    INCLUDE_DIRECTORIES(/path/to/headers)
    FIND_LIBRARY(LIBSCAFFOLD_STATIC libscaffold.a /path/to/libraries)
    TARGET_LINK_LIBRARIES(${LIBSCAFFOLD_STATIC})
    ```
- Using SCons (dynamic linking)
    ```python
    Env = Environment()
    Env.Append(LIBS = ['scaffold'])
    Env.Append(CPPPATH = ['/path/to/headers'])
    Env.Append(LIBPATH = ['/path/to/libraries'])
    ```

# Miscellaneous
- License: GNU General Public License v2.0
