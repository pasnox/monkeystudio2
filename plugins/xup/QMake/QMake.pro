TARGET	= QMake
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH	*= src/gui

RESOURCES	= src/resources/QMake.qrc

FORMS	= src/gui/UISimpleQMakeEditor.ui

HEADERS = src/QMake.h \
	src/QMake2XUP.h \
	src/QMakeProjectItem.h \
	src/gui/UISimpleQMakeEditor.h

SOURCES	= src/QMake.cpp \
	src/QMake2XUP.cpp \
	src/QMakeProjectItem.cpp \
	src/gui/UISimpleQMakeEditor.cpp
