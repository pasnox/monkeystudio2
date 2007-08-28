#! /bin/sh

qmake
make -f Makefile distclean
qmake-qt4 "CONFIG*=release"
make -f Makefile && bin/monkey