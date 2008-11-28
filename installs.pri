# Monkey Studio Install Project File

include( config.pri )

!build_pass {
	isEqual( SYSTEM_QSCINTILLA, 1 ):message( "Using system QScintilla library" )
	else:message( "Using integrated QScintilla library" )
	message( "You can change qscintilla link type by giving the qmake parameter variable: system_qscintilla=0 or system_qscintilla=1" )
	message( "Defining system_qscintilla=1 will use system wide qscintilla installation, else the integrated one." )
	message( "MkS binary will be installed to : $$PACKAGE_PREFIX" )
	message( "MkS plugins will be installed to : $$PACKAGE_PLUGINS" )
	message( "MkS datas will be installed to : $$PACKAGE_DATAS" )
	message( "You can change this by giving qmake parameters variables: prefix, plugins, datas" )

	# templates
	monkey_templates.path	= $${PACKAGE_DATAS}
	monkey_templates.files	= templates
	monkey_templates.CONFIG += no_check_exist

	# apis
	monkey_apis.path	= $${PACKAGE_DATAS}/apis
	monkey_apis.files	= ctags/apis/*.api
	monkey_apis.CONFIG += no_check_exist

	# translations
	monkey_translations.path	= $${PACKAGE_DATAS}/translations
	monkey_translations.files	= translations/*.qm
	monkey_translations.CONFIG += no_check_exist

	# debugger know_list_and_id
	monkey_debuggerIniFile.path	= $${PACKAGE_PLUGINS}/GNUdbg2
	monkey_debuggerIniFile.files	= plugins/debugger/GNUDebugger2/file
	monkey_debuggerIniFile.CONFIG += no_check_exist


	INSTALLS	= monkey_templates monkey_apis monkey_translations monkey_debuggerIniFile

	unix:!mac {
		# plugins
		monkey_plugins.path	= $${PACKAGE_PLUGINS}
		monkey_plugins.files	= bin/plugins/*
		monkey_plugins.CONFIG += no_check_exist

		# binary
		monkey_target.path	= $${PACKAGE_PREFIX}
		monkey_target.files	= bin/$${PACKAGE_TARGET}

		# desktop file
		monkey_desktop.path	= $${prefix}/share/applications
		monkey_desktop.files	= links/monkeystudio.desktop

		# desktop icon file
		monkey_desktopicon.path	= $${prefix}/share/icons/hicolor/32x32/apps
		monkey_desktopicon.files	= links/monkeystudio.png

		INSTALLS	+= monkey_plugins monkey_target monkey_desktop monkey_desktopicon
	}
}
