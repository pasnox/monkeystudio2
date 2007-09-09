#! /bin/sh
qmake-qt4 "CONFIG += debug_and_release console"
make -f Makefile debug
