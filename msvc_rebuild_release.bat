qmake
nmake -f Makefile distclean
qmake "CONFIG*=release"
nmake -f Makefile release