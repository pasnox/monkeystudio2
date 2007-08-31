#! /bin/sh

qmake-qt4
make -f Makefile distclean
qmake-qt4 "CONFIG*=release"
make -f Makefile release