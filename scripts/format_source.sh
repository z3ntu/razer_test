#!/bin/sh

find . -name "*.cpp" -not -path "*/builddir/*" -o \
       -name "*.h" -not -path "*/builddir/*" | xargs astyle -n --style=kr --indent=spaces=4 --align-pointer=name --align-reference=name --convert-tabs --attach-namespaces --pad-header --pad-oper
