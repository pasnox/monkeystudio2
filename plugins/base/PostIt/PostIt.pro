TARGET	= PostIt
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
INCLUDEPATH	*= src src/ui

QT	+= xml

FORMS	= src/ui/UIPostIt.ui

HEADERS	= src/PostIt.h \
	src/ui/UIPostIt.h \
	src/notesManager.h

SOURCES	= src/PostIt.cpp \
	src/ui/UIPostIt.cpp \
	src/notesManager.cpp
