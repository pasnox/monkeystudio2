XUPProjectSettings {
	EXECUTE_DEBUG	= bin/monkeystudio_debug
	QT_VERSION	= Qt System (4.5.2)
	OTHERS_PLATFORM_TARGET_DEBUG	= bin/monkeystudio_debug
}

# include install script
include( installs.pri )

TEMPLATE	= subdirs

CONFIG	+= ordered

SUBDIRS	= fresh \
	ctags \
	qCtagsSense \
	monkey \
	plugins

!isEqual( SYSTEM_QSCINTILLA, 1 ) {
	SUBDIRS	= qscintilla $$SUBDIRS
}
