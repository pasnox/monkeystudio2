TARGET	= G++
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS	= src/Gpp.h

SOURCES	= src/Gpp.cpp
