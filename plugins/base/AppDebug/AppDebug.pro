TARGET	= AppDebug

include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/base

INCLUDEPATH	*= src

RESOURCES	= src/resources/AppDebug.qrc

FORMS	= src/DebugDockWidget.ui

HEADERS	= src/AppDebug.h \
	src/DebugDockWidget.h

SOURCES	= src/AppDebug.cpp \
	src/DebugDockWidget.cpp