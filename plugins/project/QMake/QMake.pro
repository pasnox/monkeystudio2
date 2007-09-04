include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/project
TARGET	= QMake
RESOURCES	= src/resources/QMake.qrc

HEADERS = src/QMake.h \
	src/QMakeParser.h \
	src/QMakeItem.h

SOURCES	= src/QMake.cpp \
	src/QMakeParser.cpp \
	src/QMakeItem.cpp
