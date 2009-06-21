include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/project
TARGET	= MonkeyProject
INCLUDEPATH	+= src/ui

FORMS	= src/ui/UIMonkeyProjectSettings.ui

HEADERS = src/MonkeyProject.h \
	src/MonkeyProjectItem.h \
	src/ui/UIMonkeyProjectSettings.h

SOURCES	= src/MonkeyProject.cpp \
	src/MonkeyProjectItem.cpp \
	src/ui/UIMonkeyProjectSettings.cpp 