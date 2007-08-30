include( ../../plugins.inc )

TEMPLATE 	= lib
LANGUAGE	= C++
CONFIG	*= warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= FileBrowser
RESOURCES	= src/resources/FileBrowser.qrc

HEADERS = src/FileBrowser.h \
	src/pDockFileBrowser.h

SOURCES	= src/FileBrowser.cpp \
	src/pDockFileBrowser.cpp
