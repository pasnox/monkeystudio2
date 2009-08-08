XUPProjectSettings {
	EXECUTE_DEBUG	= bin/monkeystudio_debug
	QT_VERSION	= Qt System (4.5.0)
}

# include install script
include( installs.pri )

TEMPLATE	= subdirs

SUBDIRS	= fresh \
	ctags \
	qCtagsSense \
	monkey \
	plugins

!isEqual( SYSTEM_QSCINTILLA, 1 ) {
	SUBDIRS	= qscintilla $$SUBDIRS
}
