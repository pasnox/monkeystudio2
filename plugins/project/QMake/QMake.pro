include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/project
TARGET	= QMake
RESOURCES	= src/resources/QMake.qrc
INCLUDEPATH	+= src/ui

FORMS	= src/ui/UISettingsQMake.ui

HEADERS = src/QMake.h \
	src/QMakeParser.h \
	src/QMakeItem.h \
	src/QMakeItemDelegate.h \
	src/ui/UISettingsQMake.h

SOURCES	= src/QMake.cpp \
	src/QMakeParser.cpp \
	src/QMakeItem.cpp \
	src/QMakeItemDelegate.cpp \
	src/ui/UISettingsQMake.cpp
