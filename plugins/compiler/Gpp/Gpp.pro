include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
TARGET	= G++
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS = src/Gpp.h \
	src/GppParser.h

SOURCES	= src/Gpp.cpp \
	src/GppParser.cpp
