qmake
nmake -f Makefile distclean
qmake "CONFIG += debug_and_release console"
nmake -f Makefile debug