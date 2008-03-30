include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/xup
TARGET	= QMakeProjectEditor
QT	*= xml

RESOURCES	*= src/resources/QMakeProjectEditor.qrc

HEADERS	*= src/QMakeProjectEditor.h \
	src/QMake2XUP.h \
	src/QMakeXUPItem.h

SOURCES	*= src/QMakeProjectEditor.cpp \
	src/QMake2XUP.cpp \
	src/QMakeXUPItem.cpp
