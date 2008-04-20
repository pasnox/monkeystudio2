# Monkey Studio 2.0.0 QScintilla Project file

include( qscintilla.pri )

# include original project
include( $$QSCINTILLAVERSION/Qt4/qscintilla.pro )

#include( qscintilla.pri )

# include path path
INCLUDEPATH	*= $${PWD}/$$QSCINTILLAVERSION/include
INCLUDEPATH	*= $${PWD}/$$QSCINTILLAVERSION/src
INCLUDEPATH	*= $${PWD}/$$QSCINTILLAVERSION/Qt4
# depend path
DEPENDPATH	*= $${PWD}/$$QSCINTILLAVERSION/Qt4

BUILD_PATH	= ../build
DESTDIR	= $${BUILD_PATH}

# overwrite some values to made no lib
CONFIG	-= dll
CONFIG	+= staticlib debug_and_release

CONFIG(DebugBuild)|CONFIG(debug, debug|release) {
	#Debug
	CONFIG	+= console
	unix:TARGET	= $$join(TARGET,,,_debug)
	else:TARGET	= $$join(TARGET,,,d)
	unix:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/mac
	UI_DIR	= $${BUILD_PATH}/debug/.ui
	MOC_DIR	= $${BUILD_PATH}/debug/.moc
	RCC_DIR	= $${BUILD_PATH}/debug/.rcc

} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
}
