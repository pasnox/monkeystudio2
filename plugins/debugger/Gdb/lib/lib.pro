TEMPLATE    = lib
TARGET      = Gdb
RESOURCES   = ./resources/gdb_res.qrc
DESTDIR     = ./

CONFIG += qt  staticlib debug_and_release

BUILD_PATH  = ./build

CONFIG(debug, debug|release) {
    #Debug
    CONFIG  += console
    unix:OBJECTS_DIR    = $${BUILD_PATH}/debug/.obj/unix
    win32:OBJECTS_DIR   = $${BUILD_PATH}/debug/.obj/win32
    mac:OBJECTS_DIR = $${BUILD_PATH}/debug/.obj/mac
    UI_DIR  = $${BUILD_PATH}/debug/.ui
    MOC_DIR = $${BUILD_PATH}/debug/.moc
    RCC_DIR = $${BUILD_PATH}/debug/.rcc
} else {
    #Release
    unix:OBJECTS_DIR    = $${BUILD_PATH}/release/.obj/unix
    win32:OBJECTS_DIR   = $${BUILD_PATH}/release/.obj/win32
    mac:OBJECTS_DIR = $${BUILD_PATH}/release/.obj/mac
    UI_DIR  = $${BUILD_PATH}/release/.ui
    MOC_DIR = $${BUILD_PATH}/release/.moc
    RCC_DIR = $${BUILD_PATH}/release/.rcc
}


FORMS   = addon/gdbBreakpoint/ui/UIGdbBreakpoint.ui \
    addon/gdbBacktrace/ui/UIGdbBacktrace.ui \
    addon/gdbRegister/ui/UIGdbRegister.ui 

HEADERS = kernel/gdbProcess.h \
    kernel/gdbCore.h \
    kernel/gdbAddonBase.h \
    kernel/gdbInterpreter.h \
    kernel/gdbParser.h \
    kernel/gdbConnectTemplate.h \
    kernel/gdbKernelDispatcher.h \
    kernel/gdbSequencer.h \
    kernel/gdbControler.h \
    kernel/gdbRestoreLine.h \
    kernel/gdbSetting.h \
    addon/gdbBreakpoint/gdbBreakpoint.h \
    addon/gdbBreakpoint/gdbBreakpointThread.h \
    addon/gdbBreakpoint/ui/UIGdbBreakpoint.h \
    addon/gdbBreakpoint/ui/UIBreakpointDelegate.h \
    addon/gdbBacktrace/gdbBacktrace.h \
    addon/gdbBacktrace/ui/UIGdbBacktrace.h \
    addon/gdbRegister/gdbRegister.h \
    addon/gdbRegister/ui/UIGdbRegister.h \
    addon/gdbToolTip/gdbToolTip.h 

SOURCES = kernel/gdbProcess.cpp \
    kernel/gdbCore.cpp \
    kernel/gdbAddonBase.cpp \
    kernel/gdbInterpreter.cpp \
    kernel/gdbParser.cpp \
    kernel/gdbConnectTemplate.cpp \
    kernel/gdbKernelDispatcher.cpp \
    kernel/gdbSequencer.cpp \
    kernel/gdbControler.cpp \
    kernel/gdbRestoreLine.cpp \
    kernel/gdbSetting.cpp \
    addon/gdbBreakpoint/gdbBreakpoint.cpp \
    addon/gdbBreakpoint/gdbBreakpointThread.cpp \
    addon/gdbBreakpoint/ui/UIGdbBreakpoint.cpp \
    addon/gdbBreakpoint/ui/UIBreakpointDelegate.cpp \
    addon/gdbBacktrace/gdbBacktrace.cpp \
    addon/gdbBacktrace/ui/UIGdbBacktrace.cpp \
    addon/gdbRegister/gdbRegister.cpp \
    addon/gdbRegister/ui/UIGdbRegister.cpp \
    addon/gdbToolTip/gdbToolTip.cpp 
