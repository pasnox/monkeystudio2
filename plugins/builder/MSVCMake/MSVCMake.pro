TARGET	= MSVCMake
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/builder

HEADERS	= src/MSVCMake.h \
	src/MSVCMakeParser.h

SOURCES	= src/MSVCMake.cpp \
	src/MSVCMakeParser.cpp
