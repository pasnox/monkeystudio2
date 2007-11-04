include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= ProjectHeaders
INCLUDEPATH	*= src/ui

HEADERS	= src/ProjectHeaders.h \
	src/ui/UIProjectHeaders.h
SOURCES	= src/ProjectHeaders.cpp \
	src/ui/UIProjectHeaders.cpp
FORMS	= src/ui/UIProjectHeaders.ui
TRANSLATIONS_PATH	= translations
