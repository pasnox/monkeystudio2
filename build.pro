ProjectSettings {
	EDITOR	= QMakeProjectEditor
	EXECUTE_RELEASE	= bin/monkeystudio
	QT_VERSION	= 4.4.0
	EXECUTE_DEBUG	= bin/monkeystudio_debug
	BUILDER	= GNUMake
	COMPILER	= G++
}

TEMPLATE	= subdirs
SUBDIRS	*= fresh ctags monkey plugins

# include install script
include( installs.pri )

!isEqual( SYSTEM_QSCINTILLA, 1 ) {
	SUBDIRS	*= qscintilla
}
