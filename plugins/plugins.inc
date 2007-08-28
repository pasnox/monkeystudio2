#This is the include file to add to your plugins project

# include fresh framework
include( ../../../fresh/fresh.pri )

# include monkey framework
include( ../../../monkey/monkey.pri )

# remove core building define
DEFINES	-= MONKEY_CORE_BUILD

BUILD_PATH	= ../../../build
unix:OBJECTS_DIR	= $${BUILD_PATH}/.o/unix/plugins
win32:OBJECTS_DIR	= $${BUILD_PATH}/.o/win32/plugins
mac:OBJECTS_DIR	= $${BUILD_PATH}/.o/mac/plugins
UI_DIR	= $${BUILD_PATH}/.ui
MOC_DIR	= $${BUILD_PATH}/.moc/plugins
RCC_DIR	= $${BUILD_PATH}/.rcc/plugins

MONKEY_PLUGINS_DIR	= ../../../bin/plugins
#DEFINES	= __COVERAGESCANNER__
LIBS	= -L$${BUILD_PATH}
win32:LIBS += -lmonkey

win32:QMAKE_LIBDIR	+= ../../../bin
contains( DEFINES, __COVERAGESCANNER__ ) {
	message( "Coverage Meter Activated ( http://www.coveragemeter.com )." )
	QMAKE_CXX = cs$$QMAKE_CXX
	QMAKE_LINK = cs$$QMAKE_LINK
	QMAKE_CC = cs$$QMAKE_CC
}
