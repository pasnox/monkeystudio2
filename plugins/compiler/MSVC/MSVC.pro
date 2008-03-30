include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
TARGET	= MSVC
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS	= src/MSVC.h \
	src/MSVCParser.h

SOURCES	= src/MSVC.cpp \
	src/MSVCParser.cpp
