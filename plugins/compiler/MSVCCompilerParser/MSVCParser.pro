include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
TARGET	= MSVCParser
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS = src/MSVCParser.h

SOURCES	= src/MSVCParser.cpp
