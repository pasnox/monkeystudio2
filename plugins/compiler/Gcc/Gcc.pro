include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
TARGET	= Gcc
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS = src/Gcc.h \
	src/GccParser.h

SOURCES	= src/Gcc.cpp \
	src/GccParser.cpp
