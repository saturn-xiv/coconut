#!/bin/bash

set -e

export WORKSPACE=$PWD
export PROTOCOLS_DIR=$WORKSPACE/protocols

declare -a THRIFT_PROTOCOLS=(
    "auth"
    "monitor"

    "google-oauth2"
    "wechat-oauth2"
    "wechat-mini_program"
)

function generate_thrift_for_java() {
    echo "generate thrift-java"
    local target=$1/com/github/saturn_xiv/coconut
    if [ -d $target ]; then
        rm -r $target
    fi
    mkdir -p $1

    for p in "${THRIFT_PROTOCOLS[@]}"; do
        thrift -out $1 --gen java -r $PROTOCOLS_DIR/$p.thrift
    done
}

function generate_thrift_for_cpp() {
    echo 'generate thrift-cpp'
    if [ -d $1/src ]; then
        rm -r $1/src
    fi
    if [ -d $1/include ]; then
        rm -r $1/include
    fi

    mkdir -p $1/src
    for p in "${THRIFT_PROTOCOLS[@]}"; do
        thrift -out $1/src --gen cpp:no_skeleton -r $PROTOCOLS_DIR/$p.thrift
    done
    mkdir -p $1/include
    mv $1/src/*.h $1/include/
}

generate_thrift_for_java $WORKSPACE/sdk/java/src/main/java
generate_thrift_for_cpp $WORKSPACE/sdk/cpp

echo 'done.'

exit 0
