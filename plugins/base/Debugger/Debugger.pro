
include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= Debugger
RESOURCES	= src/resources/Debugger.qrc

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
	src/addon/gdbAnswer.cpp

OPENEDFILES	= src/GNUDebugger.h \
	src/GNUDebugger.cpp \
	src/kernel/gdbKernelDispatcher.h \
	src/kernel/gdbKernelDispatcher.cpp \
	src/kernel/gdbTemplateCore.h \
	src/kernel/gdbTemplateCore.cpp \
	src/kernel/gdbParser.h \
	src/kernel/gdbParser.cpp \
	src/addon/gdbControl.h \
	src/addon/gdbControl.cpp \
	src/addon/gdbBackTrace.h \
	src/addon/gdbBackTrace.cpp

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
	src/addon/gdbAnswer.h

