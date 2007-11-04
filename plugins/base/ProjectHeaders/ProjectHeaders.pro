include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= ProjectHeaders
RESOURCES	= src/resources/ProjectHeaders.qrc
INCLUDEPATH	*= src/ui

HEADERS	= src/ProjectHeaders.h \
	src/ui/UIProjectHeaders.h
SOURCES	= src/ProjectHeaders.cpp \
	src/ui/UIProjectHeaders.cpp
FORMS	= src/ui/UIProjectHeaders.ui
