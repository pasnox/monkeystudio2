qmake
mingw32-make -f Makefile distclean
qmake "CONFIG*=release"
mingw32-make -f Makefile