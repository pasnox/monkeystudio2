TARGET	= QMake
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup

RESOURCES	= src/resources/QMake.qrc

HEADERS = src/QMake.h \
	src/QMake2XUP.h \
	src/QMakeProjectItem.h

SOURCES	= src/QMake.cpp \
	src/QMake2XUP.cpp \
	src/QMakeProjectItem.cpp
