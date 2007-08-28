rem SET PATH=%QTDIR423%\bin;%PATH%
qmake
mingw32-make -f Makefile.Debug distclean
qmake "CONFIG*=debug console"
mingw32-make -f Makefile.Debug
pause
