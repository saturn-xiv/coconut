# COCONUT

A flexible authentication solution by CPP.

## Usage

```bash
# use gcc-12
$ sudo apt install build-essential g++-12 cmake ninja-build unzip linux-libc-dev
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 150
$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 150
# clone source code
$ git clone --recurse-submodules https://github.com/saturn-xiv/coconut.git
$ cd coconut
# OR upgrade source code
$ git submodule update --init --recursive
# setup vcpkg
$ export VCPKG_FORCE_SYSTEM_BINARIES=1
$ export VCPKG_DISABLE_METRICS=1
$ ./vendors/vcpkg/bootstrap-vcpkg.sh
# building
$ ./scripts/build.sh
```

## Documents

- [Using OAuth 2.0 to Access Google APIs](https://developers.google.com/identity/protocols/oauth2)
