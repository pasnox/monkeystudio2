
TARGET	= GnuGdb

include( ../../../plugins.pri )

DESTDIR	= $$MONKEY_PLUGINS_DIR/Gdb
LIBS += -L../lib/ -lGdb

RESOURCES	= resources/bridgeresources.qrc

FORMS = src/UISetting/UIGNUDebuggerSetting.ui

HEADERS	= src/GNUDebugger.h \
	src/DockGNUDebugger.h \
	src/BridgeQsci.h \
	src/UISetting/UIGNUDebuggerSetting.h


SOURCES	= src/GNUDebugger.cpp \
	src/DockGNUDebugger.cpp \
	src/BridgeQsci.cpp \
	src/UISetting/UIGNUDebuggerSetting.cpp
