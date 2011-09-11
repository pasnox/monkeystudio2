TARGET	= ClassBrowser
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/base

FORMS	= src/UISettingsClassBrowser.ui

HEADERS	= src/ClassBrowser.h \
	src/pDockClassBrowser.h \
	src/UISettingsClassBrowser.h

SOURCES	= src/ClassBrowser.cpp \
	src/pDockClassBrowser.cpp \
	src/UISettingsClassBrowser.cpp