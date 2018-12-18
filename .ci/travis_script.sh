#!/bin/bash

set -ex

meson_args="-Dbuild_tests=true"

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    export PATH="/usr/local/opt/qt/bin:$PATH"
    meson $meson_args builddir && ninja -C builddir test
elif [[ $TRAVIS_OS_NAME == 'linux' ]]; then
    docker run myimage /bin/sh -c "cd /root && TRAVIS=true CC=$CC CXX=$CXX meson $meson_args builddir && ninja -C builddir test"
fi
