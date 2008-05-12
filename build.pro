TEMPLATE	= subdirs
SUBDIRS	= qscintilla fresh ctags monkey plugins

# include install script
include( installs.pri )

ProjectSettings {
	EDITOR	= QMakeProjectEditor
	EXECUTE_RELEASE	= bin/monkeystudio
	QT_VERSION	= 4.4.0
}
