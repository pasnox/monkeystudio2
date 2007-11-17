include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
TARGET	= GccParser
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS = src/GccParser.h \
	src/Parser.h

SOURCES	= src/GccParser.cpp \
	src/Parser.cpp
