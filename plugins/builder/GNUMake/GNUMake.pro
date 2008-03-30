include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/builder
TARGET	= GNUMake
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS	= src/GNUMake.h \
	src/GNUMakeParser.h

SOURCES	= src/GNUMake.cpp
