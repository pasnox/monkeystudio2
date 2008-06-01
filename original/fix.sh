#!/bin/sh

# get cur dir
curdir=$(pwd)

# apply fix
cd /usr/bin && sudo patch -p0 < $curdir/patch.diff	
