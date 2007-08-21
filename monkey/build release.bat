qmake "CONFIG*=release"
mingw32-make -f Makefile.Release && ..\bin\monkey.exe
pause