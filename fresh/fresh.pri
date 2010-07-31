# fresh includes path to include in plugins/core project

# include path
INCLUDEPATH	*= $${PWD} #$$getFolders( $${PWD}/objects $${PWD}/widgets )

# dependency
PRE_TARGETDEPS	*= $${PWD}

# library integration
FRESH_TARGET	= fresh
LIBS	*= -L$${PACKAGE_BUILD_PATH}

contains( TEMPLATE, .*app ) {
	CONFIG(debug, debug|release) {
		#Debug
		unix:LIBS	*= -l$${FRESH_TARGET}_debug
		else:LIBS	*= -l$${FRESH_TARGET}d
	} else {
		#Release
		LIBS	*= -l$${FRESH_TARGET}
	}
}
