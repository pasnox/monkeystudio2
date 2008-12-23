TARGET	= SearchEndReplace
include( ../../plugins.pri )
DESTDIR = $$MONKEY_PLUGINS_DIR/base

HEADERS	= src/SearchAndReplace.h \
	src/SearchWidget.h \
	src/SearchResultsDock.h \
	src/SearchThread.h

SOURCES	= src/SearchAndReplace.cpp \
	src/SearchWidget.cpp \
	src/SearchResultsDock.cpp \
	src/SearchThread.cpp

RESOURCES	*= resources/SearchAndReplace.qrc