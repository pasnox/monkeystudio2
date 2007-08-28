#! /bin/sh

qmake-qt4 "CONFIG*=release"
make -f Makefile && bin/monkey