include( assistant.inc )
TARGET	= QtAssistant
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/child

FORMS	*= src/ui/UIQtAssistantSettings.ui

HEADERS	*= src/QtAssistant.h \
	src/QtAssistantChild.h \
	src/QtAssistantConfig.h \
	src/ui/UIQtAssistantSettings.h

SOURCES	*= src/QtAssistant.cpp \
	src/QtAssistantChild.cpp \
	src/QtAssistantConfig.cpp \
	src/ui/UIQtAssistantSettings.cpp
