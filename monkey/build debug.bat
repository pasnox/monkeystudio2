rem SET PATH=%QTDIR423%\bin;%PATH%
qmake "CONFIG*=debug"
mingw32-make -f Makefile.Debug && ..\bin\monkey.exe
pause