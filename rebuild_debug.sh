#! /bin/sh

qmake
make -f Makefile distclean
qmake-qt4 "CONFIG*=debug console"
make -f Makefile && bin/monkey