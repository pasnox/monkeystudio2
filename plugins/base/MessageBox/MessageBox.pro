include( ../../plugins.inc )

QT	*= xml
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= MessageBox
RESOURCES	= src/resources/MessageBox.qrc

HEADERS = src/MessageBox.h \
	src/pDockMessageBox.h

SOURCES	= src/MessageBox.cpp \
	src/pDockMessageBox.cpp
