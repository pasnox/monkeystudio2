TARGET	= MSVC
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler

HEADERS	= src/MSVC.h \
	src/MSVCParser.h

SOURCES	= src/MSVC.cpp \
	src/MSVCParser.cpp
