#This is the include file to add to your plugins project

# set plugin name define
!isEmpty( TARGET ):DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

# include config file
include( $${PWD}/../config.pri )

# include scintilla framework
include( $${PACKAGE_PWD}/qscintilla/qscintilla.pri )

# include fresh framework
include( $${PACKAGE_PWD}/fresh/fresh.pri )

# include ctags framework
include( $${PACKAGE_PWD}/ctags/ctags.pri )

# include qCtagsSense framework
include( $${PACKAGE_PWD}/qCtagsSense/qCtagsSense.pri )

# include monkey framework
include( $${PACKAGE_PWD}/monkey/monkey.pri )

TEMPLATE	= lib
CONFIG	*= dll plugin
PRE_TARGETDEPS	*= $${PACKAGE_PWD}/monkey

mac:MONKEY_PLUGINS_DIR	= $${PACKAGE_DESTDIR}/$${PACKAGE_TARGET}.app/Contents/plugins
else:unix|win32:MONKEY_PLUGINS_DIR	= $${PACKAGE_DESTDIR}/plugins
#DEFINES	= __COVERAGESCANNER__
LIBS	= -L$${PACKAGE_BUILD_PATH}
mac:*-g++:LIBS	*= -Wl,-undefined,dynamic_lookup

# ubuntu hardy/debian fix
QMAKE_LFLAGS	-= -Wl,--no-undefined

win32:QMAKE_LIBDIR	+= $${PACKAGE_DESTDIR}
contains( DEFINES, __COVERAGESCANNER__ ) {
	message( "Coverage Meter Activated ( http://www.coveragemeter.com )." )
	QMAKE_CXX	= cs$$QMAKE_CXX
	QMAKE_LINK	= cs$$QMAKE_LINK
	QMAKE_CC	= cs$$QMAKE_CC
}

isEqual( SYSTEM_QSCINTILLA, 1 ):PRE_TARGETDEPS	*= $${PACKAGE_PWD}/qscintilla
PRE_TARGETDEPS	*= $${PACKAGE_PWD}/fresh $${PACKAGE_PWD}/ctags $${PACKAGE_PWD}/qCtagsSense

CONFIG(DebugBuild)|CONFIG(debug, debug|release) {
	#Debug
	#unix:LIBS += -lmonkeystudio_debug
	win32:LIBS	+= -lmonkeystudiod
	unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/plugins/debug/.obj/unix
	win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/plugins/debug/.obj/win32
	mac:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/plugins/debug/.obj/mac
	MOC_DIR	= $${PACKAGE_BUILD_PATH}/plugins/debug/.moc
} else {
	#Release
	win32:LIBS	+= -lmonkeystudio
	unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/plugins/release/.obj/unix
	win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/plugins/release/.obj/win32
	mac:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/plugins/release/.obj/mac
	MOC_DIR	= $${PACKAGE_BUILD_PATH}/plugins/release/.moc
}
