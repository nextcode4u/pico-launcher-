#!/usr/bin/env bash
set -e

make clean
make -j"$(nproc)" V=1 2>&1 | tee build.log
