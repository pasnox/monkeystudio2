#This is the include file to add to your plugins project

# include config file
include( $${PWD}/../config.pri )

# include scintilla framework
include( $${PACKAGE_PWD}/qscintilla/qscintilla.pri )

# include fresh framework
include( $${PACKAGE_PWD}/fresh/fresh.pri )

# include ctags framework
include( $${PACKAGE_PWD}/ctags/ctags.pri )

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

CONFIG(DebugBuild)|CONFIG(debug, debug|release) {
	#Debug
	#unix:LIBS += -lmonkeystudio_debug
	win32:LIBS	+= -lmonkeystudiod
} else {
	#Release
	win32:LIBS	+= -lmonkeystudio
}
