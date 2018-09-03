#!/bin/bash

find . -name "*.cpp" -not -path "*/builddir/*" -o \
       -name "*.h" -not -path "*/builddir/*" | xargs astyle --style=linux -n
