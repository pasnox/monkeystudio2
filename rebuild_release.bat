qmake
mingw32-make -f Makefile distclean
qmake "CONFIG += debug_and_release"
mingw32-make -f Makefile release