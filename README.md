## Build and Install

Libwild was written in C++ 14, build with clang (libc++) using cmake 3.x.

### GNU/Linux (eg. Debian jessie)

```shell
apt-get install -y clang libc++-dev cmake
```

### FreeBSD

```shell
pkg install -y cmake
```

### Build

```shell
git clone https://github.com/kezhuw/wild.git
cd wild
mkdir build
cd build
cmake ../
make
# make install      # for other libraries and executables build on top of libwild
```
