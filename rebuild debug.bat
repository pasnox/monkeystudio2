qmake
mingw32-make -f Makefile.Debug distclean
qmake "CONFIG*=debug console"
mingw32-make -f Makefile.Debug