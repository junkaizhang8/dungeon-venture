#!/bin/bash

[[ ! -d build ]] && echo "Run build.sh first" && exit 1
cd build/editor && ./editor
