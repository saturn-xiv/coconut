#!/bin/bash

set -e

export WORKSPACE=$PWD
export GIT_VERSION=$(git describe --tags --always --dirty --first-parent)
export PACKAGE_NAME=coconut-$GIT_VERSION
export TARGET=$WORKSPACE/tmp/$PACKAGE_NAME

# ---------------------------------------------------------

function build_backend() {
    cd $WORKSPACE

    local pkg="github.com/saturn-xiv/coconut/cmd"
    local ldflags="-s -w -X '$pkg.repo_url=$(git remote get-url origin)' -X '$pkg.author_name=$(git config --get user.name)' -X '$pkg.author_email=$(git config --get user.email)' -X '$pkg.build_time=$(date -R)' -X '$pkg.git_version=$(git describe --tags --always --dirty --first-parent)'"

    echo "build for $2"
    GOOS=linux GOARCH=$1 go build -ldflags "$ldflags"
    mkdir -p $TARGET/bin/$2
    mv coconut $TARGET/bin/$2/coconut
}

# ---------------------------------------------------------

build_backend amd64 x86_64
build_backend arm64 aarch64
build_backend riscv64 riscv64

cd $WORKSPACE/tmp
echo "compressing $PACKAGE_NAME..."
XZ_OPT=-9 tar -cJf $PACKAGE_NAME.tar.xz $PACKAGE_NAME
md5sum $PACKAGE_NAME.tar.xz >$PACKAGE_NAME.txt

echo "done($PACKAGE_NAME)."

exit 0
