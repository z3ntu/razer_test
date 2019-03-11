FROM ubuntu:latest
ARG build_type

ADD . /root

RUN apt-get update && apt-get install -y qt5-default g++ clang pkg-config libhidapi-dev libudev-dev
RUN if [ "$build_type" != 'cron' ]; then apt-get install -y meson; fi
RUN if [ "$build_type" = 'cron' ]; then apt-get install -y git python3 ninja-build; fi
RUN if [ "$build_type" = 'cron' ]; then git clone https://github.com/mesonbuild/meson; fi
RUN if [ "$build_type" = 'cron' ]; then ln -s $PWD/meson/meson.py /usr/local/bin/meson; fi
