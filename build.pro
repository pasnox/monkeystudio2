XUPProjectSettings {
	QT_VERSION	= 4.5.1 LGPL
	OTHERS_PLATFORM_TARGET_DEBUG	= bin/monkeystudio_debug
	MAC_PLATFORM_TARGET_RELEASE	= bin/Monkey Studio.app
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
