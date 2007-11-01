include( assistant.inc )
include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/child
TARGET	= QtAssistant
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

SOURCES	+= src/QtAssistant.cpp \
	src/QtAssistantChild.cpp

HEADERS	+= src/QtAssistant.h \
	src/QtAssistantChild.h
