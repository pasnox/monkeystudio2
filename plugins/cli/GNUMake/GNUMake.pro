include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/CLITool
TARGET	= GNUMake

HEADERS = src/GNUMake.h \
	src/GNUMakeParser.h

SOURCES	= src/GNUMake.cpp \
	src/GNUMakeParser.cpp
