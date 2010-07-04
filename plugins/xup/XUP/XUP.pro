TARGET	= XUP
include( ../../plugins.pri )
message( "include: $$INCLUDEPATH" )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH	*= src

FORMS	=  \
	src/gui/UIXUPEditor.ui

HEADERS	=  \
	src/XUP.h \
	src/gui/UIXUPEditor.h

SOURCES	=  \
	src/XUP.cpp \
	src/gui/UIXUPEditor.cpp
