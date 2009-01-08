TARGET	= Python
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/interpreter

HEADERS	= src/Python.h \
	src/PythonParser.h

SOURCES	= src/Python.cpp
