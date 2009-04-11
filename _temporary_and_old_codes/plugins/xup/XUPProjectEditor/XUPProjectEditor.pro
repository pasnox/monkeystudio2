TARGET	= XUPProjectEditor
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup
QT	*= xml

HEADERS	= src/XUPProjectEditor.h

SOURCES	= src/XUPProjectEditor.cpp
