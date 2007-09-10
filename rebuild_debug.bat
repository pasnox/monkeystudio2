qmake
mingw32-make -f Makefile distclean
qmake "CONFIG += debug_and_release console"
mingw32-make -f Makefile debug