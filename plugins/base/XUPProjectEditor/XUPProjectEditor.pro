include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/xup
TARGET	= XUPProjectEditor
INCLUDEPATH	*= . src src/ui ../XUP/src/xupmanager ../XUP/src/xupmanager/ui
QT	*= xml

FORMS	*= src/ui/UIXUPProjectEditor.ui

HEADERS	*= src/XUPProjectEditor.h \
	src/ui/UIXUPProjectEditor.h

SOURCES	*= src/XUPProjectEditor.cpp \
	src/ui/UIXUPProjectEditor.cpp

