#!/bin/bash

set -e

export PROTOBUF_ROOT=$HOME/.local
export WORKSPACE=$PWD
export COCONUT_PROTOCOLS=$WORKSPACE/protocols

# ----------------------------------------------------------

function generate_grpc_for_go() {
    if [ -d $2 ]; then
        rm -r $2
    fi
    mkdir -p $2
    protoc -I $COCONUT_PROTOCOLS -I $PROTOBUF_ROOT/include/google/protobuf \
        --go_out=$2 --go_opt=paths=source_relative \
        --go-grpc_out=$2 --go-grpc_opt=paths=source_relative \
        $COCONUT_PROTOCOLS/$1.proto
}

# ----------------------------------------------------------

generate_grpc_for_go metasequoia $WORKSPACE/metasequoia/v2
generate_grpc_for_go orchid $WORKSPACE/orchid/v2

echo 'done.'
exit 0
