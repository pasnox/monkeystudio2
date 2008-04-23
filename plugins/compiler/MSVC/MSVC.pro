TARGET	= MSVC
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS	= src/MSVC.h \
	src/MSVCParser.h

SOURCES	= src/MSVC.cpp \
	src/MSVCParser.cpp
