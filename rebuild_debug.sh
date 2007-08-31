#! /bin/sh

qmake-qt4
make -f Makefile distclean
qmake-qt4 "CONFIG*=debug console"
make -f Makefile debug