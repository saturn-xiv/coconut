#!/bin/bash

set -e

export VERSION=$(date "+%4Y%m%d%H%M%S")
export UBUNTU_CODE="jammy"
export CODE="coconut-go-$UBUNTU_CODE"

buildah pull ubuntu:$UBUNTU_CODE
buildah bud --layers -t $CODE .
podman save --format=oci-archive -o $CODE-$VERSION.tar $CODE
md5sum $CODE-$VERSION.tar >$CODE-$VERSION.md5

echo "done($CODE-$VERSION.tar)."

exit 0
