# Monkey Studio 2 Ctags project include file

# include shared ctags project file
include( $${PWD}/ctags_shared.pri )

# include path
INCLUDEPATH	*= $$getFolders( $${PWD}/$${CTAGS_VERSION} )

# dependency
PRE_TARGETDEPS	*= $${PWD}

# library integration
LIBS	*= -L$${PACKAGE_BUILD_PATH}

CONFIG(debug, debug|release) {
	#Debug
	unix:LIBS	*= -l$${TARGET}_debug
	else:LIBS	*= -l$${TARGET}d
} else {
	#Release
	LIBS	*= -l$${TARGET}
}
