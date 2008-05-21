include( assistant.inc )
TARGET	= QtAssistant
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/child

HEADERS	+= src/QtAssistant.h \
	src/QtAssistantChild.h

SOURCES	+= src/QtAssistant.cpp \
	src/QtAssistantChild.cpp