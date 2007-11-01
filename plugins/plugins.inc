#This is the include file to add to your plugins project

# include scintilla framework
include( ../../../qscintilla/qscintilla.pri )

# include fresh framework
include( ../../../fresh/fresh.pri )

# include monkey framework
include( ../../../monkey/monkey.pri )

# remove core building define
DEFINES	-= MONKEY_CORE_BUILD

BUILD_PATH	= ../../../build

TEMPLATE 	= lib
LANGUAGE	= Qt4/C++
CONFIG	*= warn_on lib_bundle thread x11 dll plugin debug_and_release

MONKEY_PLUGINS_DIR	= ../../../bin/plugins
#DEFINES	= __COVERAGESCANNER__
LIBS	= -L$${BUILD_PATH}
win32 {
	CONFIG(DebugBuild)|CONFIG(debug, debug|release) {
		unix:LIBS += -lmonkey_debug
		else:LIBS += -lmonkey_d
	} else:LIBS += -lmonkey
}

win32:QMAKE_LIBDIR	+= ../../../bin
contains( DEFINES, __COVERAGESCANNER__ ) {
	message( "Coverage Meter Activated ( http://www.coveragemeter.com )." )
	QMAKE_CXX = cs$$QMAKE_CXX
	QMAKE_LINK = cs$$QMAKE_LINK
	QMAKE_CC = cs$$QMAKE_CC
}

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
