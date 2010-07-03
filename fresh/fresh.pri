# fresh includes path to include in plugins/core project

# include path
INCLUDEPATH	*= $$getFolders( $${PWD}/objects $${PWD}/widgets )

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
