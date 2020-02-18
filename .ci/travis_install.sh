#!/bin/bash

set -ex

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    # Work around "Error: The `brew link` step did not complete successfully"
    brew unlink python@2
    # Work around "Homebrew must be run under Ruby 2.6! You're running 2.3.3. (RuntimeError)"
    brew update
    brew install qt hidapi
    # Install meson from git master on the daily builds
    if [[ $TRAVIS_EVENT_TYPE == 'cron' ]]; then
        brew install meson --HEAD
    else
        brew install meson
    fi
elif [[ $TRAVIS_OS_NAME == 'linux' ]]; then
    docker build -t myimage -f ./.ci/Dockerfile.$DISTRO --build-arg build_type=$TRAVIS_EVENT_TYPE .
fi
