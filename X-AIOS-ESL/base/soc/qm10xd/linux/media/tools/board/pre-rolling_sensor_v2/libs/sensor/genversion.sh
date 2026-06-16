#!/bin/bash
usage() {
    echo "Usage: $0 [srctree]" >&2
    exit 1
}

srctree=.
if test $# -gt 0; then
    srctree=$1
    shift
fi
if test $# -gt 0 -o ! -d "$srctree"; then
    usage
fi
version="version-control not find"
cd "$srctree"
if test -d .git && head=`git rev-parse --verify --short HEAD 2>/dev/null`; then
    version="g$head"

    # Check for uncommitted changes
    if git diff-index --name-only HEAD . ':!*.md' | read dummy; then
        version+='-dirty'
    fi
fi

VER_PATH=sensor_common/sensor_ver.h
echo "#ifndef _SENSOR_GIT_VERSION_">$VER_PATH
echo "#define _SENSOR_GIT_VERSION_">>$VER_PATH
echo "/*Auto Gen File: Define Git Version*/">>$VER_PATH
echo "#define SENSOR_GIT_VERSION \"$version\"">>$VER_PATH
echo "#endif">>$VER_PATH