qmake
nmake -f Makefile distclean
qmake "CONFIG*=debug console"
nmake -f Makefile debug