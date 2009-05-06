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
