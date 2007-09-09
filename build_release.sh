#! /bin/sh
qmake-qt4 "CONFIG += debug_and_release"
make -f Makefile release
