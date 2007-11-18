include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/compiler
TARGET	= MSVCCompiler
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

HEADERS = src/MSVCCompiler.h

SOURCES	= src/MSVCCompiler.cpp
