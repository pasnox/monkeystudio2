include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/xup
TARGET	= XUPProjectEditor
INCLUDEPATH	*= ../XUP/src/xupmanager
QT	*= xml

HEADERS	*= src/XUPProjectEditor.h

SOURCES	*= src/XUPProjectEditor.cpp
