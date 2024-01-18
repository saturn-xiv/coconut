#!/bin/bash

set -e

. /etc/os-release

export WORKSPACE=$PWD

function build_coconut() {
    echo "build coconut with $1"

    local build_root=$WORKSPACE/build/$1-Release
    # local mailio_args="-DMAILIO_BUILD_DOCUMENTATION=OFF -DMAILIO_BUILD_EXAMPLES=OFF -DBUILD_SHARED_LIBS=OFF -DMAILIO_BUILD_TESTS=OFF"
    local casbin_args="-DCASBIN_BUILD_TEST=OFF -DCASBIN_BUILD_BENCHMARK=OFF -DCASBIN_BUILD_PYTHON_BINDINGS=OFF -DCASBIN_INSTALL=OFF"
    local thrift_args="-DWITH_ZLIB=ON -DBUILD_TESTING=OFF -DBUILD_COMPILER=OFF -DWITH_OPENSSL=ON -DWITH_QT5=OFF -DBUILD_C_GLIB=OFF -DBUILD_JAVA=OFF -DBUILD_JAVASCRIPT=OFF -DBUILD_NODEJS=OFF -DBUILD_PYTHON=OFF"
    local tink_args="-DABSL_PROPAGATE_CXX_STD=ON -DTINK_USE_SYSTEM_OPENSSL=ON"

    mkdir -p $build_root
    # -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=$WORKSPACE/toolchains/$1.cmake \
    cmake -DCMAKE_BUILD_TYPE=Release \
        -DVCPKG_HOST_TRIPLET=$1-linux-release -DVCPKG_TARGET_TRIPLET=$1-linux-release \
        -DCMAKE_TOOLCHAIN_FILE=$WORKSPACE/vendors/vcpkg/scripts/buildsystems/vcpkg.cmake \
        $casbin_args $tink_args $thrift_args \
        -B $build_root -S $WORKSPACE
    make -j $(nproc --ignore=2) -C $build_root
}

# -----------------------------------------------------------------------------

if [[ $ID != "ubuntu" ]]; then
    echo "unsupported system $ID"
    exit 1
fi

if [[ $UBUNTU_CODENAME != "jammy" ]] && [[ $UBUNTU_CODENAME != "focal" ]]; then
    echo "unsupported system $UBUNTU_CODENAME"
    exit 1
fi

export MACHINE=$(uname -m)

if [[ $MACHINE == "x86_64" ]]; then
    build_coconut "x64"
elif [[ $MACHINE == "aarch64" ]]; then
    build_coconut "arm64"
else
    echo "unsupported machine $MACHINE"
    exit 1
fi

cd $WORKSPACE/sdk/java
mvn clean
mvn package

echo 'done.'
exit 0
