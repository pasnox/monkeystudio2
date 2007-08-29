qmake
mingw32-make -f Makefile distclean
qmake "CONFIG*=debug console"
mingw32-make -f Makefile