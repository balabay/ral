#!/bin/bash

set -xe

docker build . -t ral-antlr4-llvm-dev

docker run --rm -it \
--mount type=bind,source="$(pwd)"/,target=/usr/src/app/ \
ral-antlr4-llvm-dev
