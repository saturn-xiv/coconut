#!/bin/bash

set -e

. /etc/os-release

export WORKSPACE=$PWD

function build_coconut() {
    local arch=$(uname -p)

    echo "build coconut with gcc-$1($arch)"
    apt install -y cmake g++-$1 golang linux-libc-dev

    local build_root=$WORKSPACE/build/$arch-Release

    mkdir -p $build_root
    CC=gcc-$1 CXX=g++-$1 cmake -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE=$WORKSPACE/vendors/vcpkg/scripts/buildsystems/vcpkg.cmake \
        -DABSL_PROPAGATE_CXX_STD=ON -DTINK_USE_SYSTEM_OPENSSL=ON \
        -DBUILD_COMPILER=OFF -DWITH_OPENSSL=ON -DWITH_QT5=OFF -DBUILD_C_GLIB=OFF -DBUILD_JAVA=OFF -DBUILD_JAVASCRIPT=OFF -DBUILD_NODEJS=OFF -DBUILD_PYTHON=OFF \
        -B $build_root -S $WORKSPACE
    make -j $(nproc --ignore=2) -C $build_root
}

# -----------------------------------------------------------------------------

if [ $ID != "ubuntu" ]; then
    echo "unsupported system $ID"
    exit 1
fi

if [ $UBUNTU_CODENAME == "jammy" ]; then
    build_coconut 12
elif [ $UBUNTU_CODENAME == "focal" ]; then
    build_coconut 11
elif [ $UBUNTU_CODENAME == "bionic" ]; then
    build_coconut 11
else
    echo "unsupported system $UBUNTU_CODENAME"
    exit 1
fi

cd $WORKSPACE/sdk/java
mvn clean
mvn package

echo 'done.'
exit 0
