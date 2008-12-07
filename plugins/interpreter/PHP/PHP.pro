TARGET	= PHP
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/interpreter

HEADERS	= src/PHP.h \
	src/PHPParser.h

SOURCES	= src/PHP.cpp
