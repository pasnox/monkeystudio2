rem SET PATH=%QTDIR423%\bin;%PATH%
qmake "CONFIG*=release"
mingw32-make -f Makefile.Release && pause &&  ..\bin\monkey.exe
pause