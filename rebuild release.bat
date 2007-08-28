qmake
mingw32-make -f Makefile.Release distclean
qmake "CONFIG*=release"
mingw32-make -f Makefile.Release