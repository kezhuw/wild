## Build and Install

Libwild is a C++ utility library written in C++ 14. Due to operating system, compiler and standard library support, currently, libwild is build only on GNU/Linux clang 3.4+ with libstdc++ 4.9.

### Consideration

*   `thread_local` in clang requires libstdc++ 4.8 or later.
*   std::regex has no implementation in libstdc++ 4.8, libstdc++ 4.9 implements.
*   Max OS X and FreeBSD contain gcc 4.2 in their base system due to license issue.
*   Clang has better standard compliant than gcc 4.9.

As soon as `thread_local` support in libc++ complete, I will switch to it. Anyway I prefer Clang.

### GNU/Linux

In Debian jessie:
```shell
apt-get install -y clang libc++-dev cmake
```

### Build
```shell
git clone https://github.com/kezhuw/wild.git
cd wild
mkdir build
cd build
CC=clang CXX=clang++ cmake ../
make            # with optional `make install` for other libraries and executables build on top of libwild.
```
