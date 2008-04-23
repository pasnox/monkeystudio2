TARGET	= GNUMake
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/builder
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS	= src/GNUMake.h \
	src/GNUMakeParser.h

SOURCES	= src/GNUMake.cpp
