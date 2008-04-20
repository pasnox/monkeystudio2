include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/GNUdbg
TARGET	= GNUDebugger
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""
RESOURCES	*= src/resources/GNUDebugger.qrc

SOURCES	= src/GNUDebugger.cpp \
	src/DockGNUDebugger.cpp \
	src/kernel/gdbTemplateCore.cpp \
	src/kernel/gdbCore.cpp \
	src/kernel/gdbParser.cpp \
	src/kernel/gdbKernelDispatcher.cpp \
	src/addon/gdbBackTrace.cpp \
	src/addon/gdbBreakPoint.cpp \
	src/addon/gdbRegisters.cpp \
	src/addon/gdbControl.cpp \
	src/addon/gdbBridgeEditor.cpp \
	src/addon/gdbWatch.cpp \
	src/addon/gdbAnswer.cpp \
	src/kernel/gdbProcess.cpp

HEADERS	= src/GNUDebugger.h \
	src/DockGNUDebugger.h \
	src/kernel/gdbTemplateCore.h \
	src/kernel/gdbInterpreter.h \
	src/kernel/gdbCore.h \
	src/kernel/gdbParser.h \
	src/kernel/gdbKernelDispatcher.h \
	src/addon/gdbBackTrace.h \
	src/addon/gdbBreakPoint.h \
	src/addon/gdbRegisters.h \
	src/addon/gdbControl.h \
	src/addon/gdbBridgeEditor.h \
	src/addon/gdbWatch.h \
	src/addon/gdbAnswer.h \
	src/kernel/gdbProcess.h
