# Monkey Studio 2.0.0 QScintilla Project file
include( qscintilla_check.pri )

# include path
isEqual( SYSTEM_QSCINTILLA, 1 ) {
	# include path
	INCLUDEPATH	*= $$[QT_INSTALL_HEADERS]/Qsci
} else {
	# define where is the official qscintilla folder
	QSCINTILLAVERSION	= QScintilla-gpl-snapshot
	# include path
	INCLUDEPATH	*= $$getFolders( $${PWD}/$${QSCINTILLAVERSION} )
}

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
