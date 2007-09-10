qmake
nmake -f Makefile distclean
qmake "CONFIG += debug_and_release"
nmake -f Makefile release