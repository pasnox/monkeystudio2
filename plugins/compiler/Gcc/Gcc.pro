TARGET	= Gcc
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS	= src/Gcc.h

SOURCES	= src/Gcc.cpp
