FROM alpine:latest
ARG build_type

ADD . /root

# HIDAPI is only available in the testing repo
RUN echo "@testing http://dl-4.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories
RUN apk add qt5-qtbase-dev g++ clang pkgconf hidapi-dev@testing eudev-dev
RUN if [ "$build_type" != 'cron' ]; then apk add meson; fi
RUN if [ "$build_type" = 'cron' ]; then apk add git python3 ninja; fi
RUN if [ "$build_type" = 'cron' ]; then git clone https://github.com/mesonbuild/meson; fi
RUN if [ "$build_type" = 'cron' ]; then ln -s $PWD/meson/meson.py /usr/local/bin/meson; fi
