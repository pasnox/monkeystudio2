include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/builder
TARGET	= MSVCMake
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS = src/MSVCMake.h \
	src/MSVCMakeParser.h

SOURCES	= src/MSVCMake.cpp \
	src/MSVCMakeParser.cpp
