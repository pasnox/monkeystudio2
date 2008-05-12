TARGET	= MSVCMake
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/builder
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS	= src/MSVCMake.h \
	src/MSVCMakeParser.h

SOURCES	= src/MSVCMake.cpp \
	src/MSVCMakeParser.cpp
