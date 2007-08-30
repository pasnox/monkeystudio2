include( ../../plugins.inc )

TEMPLATE 	= lib
LANGUAGE	= C++
CONFIG	*= warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= MessageBox
RESOURCES	= src/resources/MessageBox.qrc

HEADERS = src/MessageBox.h \
	src/pDockMessageBox.h

SOURCES	= src/MessageBox.cpp \
	src/pDockMessageBox.cpp
