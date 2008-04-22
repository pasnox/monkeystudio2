TEMPLATE	= subdirs
CONFIG	+= debug_and_release
SUBDIRS	= qscintilla fresh ctags monkey plugins

ProjectSettings {
	EDITOR	= QMakeProjectEditor
	EXECUTE_RELEASE	= bin/monkeystudio
	QT_VERSION	= 4.3.4
}

include (installs.pri)
