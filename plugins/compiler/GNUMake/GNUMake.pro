include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
TARGET	= GNUMake

HEADERS = src/GNUMake.h \
	src/GNUMakeParser.h \
	src/GccParser.h

SOURCES	= src/GNUMake.cpp \
	src/GNUMakeParser.cpp \
	src/GccParser.cpp
