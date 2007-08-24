SET PATH=%QTDIR423%\bin;%PATH%
qmake "CONFIG*=debug console"
mingw32-make -f Makefile.Debug && pause && ..\bin\monkey.exe