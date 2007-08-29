#! /bin/sh

qmake-qt4 "CONFIG*=debug console"
make -f Makefile && bin/monkey