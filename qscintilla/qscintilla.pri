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
	INCLUDEPATH	*= $$getFolders( $${PWD}/$${QSCINTILLAVERSION}, designer-Qt3 designer-Qt4 example-Qt3 example-Qt4 Qt3 doc Python python api )
	
	# dependency
	PRE_TARGETDEPS	*= $${PWD}

	# library integration
	LIBS	*= -L$${PACKAGE_BUILD_PATH}
}

QSCINTILLA_TARGET	= qscintilla2

CONFIG(debug, debug|release) {
	#Debug
	unix:LIBS	*= -l$${QSCINTILLA_TARGET}_debug
	else:LIBS	*= -l$${QSCINTILLA_TARGET}d
} else {
	#Release
	LIBS	*= -l$${QSCINTILLA_TARGET}
}
