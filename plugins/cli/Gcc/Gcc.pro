include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/CLITool
TARGET	= Gcc

HEADERS = src/Gcc.h \
	src/GccParser.h

SOURCES	= src/Gcc.cpp \
	src/GccParser.cpp
