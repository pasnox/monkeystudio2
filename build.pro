ProjectSettings {
	EDITOR	= QMakeProjectEditor
	EXECUTE_RELEASE	= bin/monkeystudio
	QT_VERSION	= 4.4.0
	EXECUTE_DEBUG	= bin/monkeystudio_debug
	BUILDER	= GNUMake
	COMPILER	= G++
}

# include install script
include( installs.pri )

TEMPLATE	= subdirs

!isEqual( SYSTEM_QSCINTILLA, 1 ) {
	SUBDIRS	*= qscintilla
}
SUBDIRS	*= fresh ctags monkey plugins
