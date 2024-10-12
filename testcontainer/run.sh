#!/bin/sh
set -ex

cd "$(dirname "$0")"

docker build -t musl-nscd-tmp .
docker run -v "$(pwd)/../:/src" --rm -it musl-nscd-tmp
