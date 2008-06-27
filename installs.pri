# Monkey Studio Install Project File

include( config.pri )

!build_pass {
	message( "Monkey Studio binary will be installed to : $$PACKAGE_PREFIX" )
	message( "Monkey Studio datas will be installed to : $$PACKAGE_DATAS" )
	message( "You can change this by exporting these variables to your shell before calling qmake : MONKEY_PREFIX, MONKEY_DATAS" )

	# templates
	monkey_templates.path	= $${PACKAGE_DATAS}
	monkey_templates.files	= templates

	# apis
	monkey_apis.path	= $${PACKAGE_DATAS}/apis
	monkey_apis.files	= ctags/apis/*.api

	# translations
	monkey_translations.path	= $${PACKAGE_DATAS}/translations
	monkey_translations.files	= translations/*.qm

	# debugger know_list_and_id
	monkey_debuggerIniFile.path	= $${PACKAGE_DATAS}/plugins/GNUdbg2
	monkey_debuggerIniFile.files	= plugins/debugger/GNUDebugger2/file


	INSTALLS	= monkey_templates monkey_apis monkey_translations monkey_debuggerIniFile

	unix:!mac {
		# plugins
		monkey_plugins.path	= $${PACKAGE_DATAS}
		monkey_plugins.files	= bin/plugins

		# binary
		monkey_target.path	= $${PACKAGE_PREFIX}/bin
		monkey_target.files	= bin/$${PACKAGE_TARGET}

		# desktop file
		monkey_desktop.path	= $${PACKAGE_PREFIX}/share/applications
		monkey_desktop.files	= links/monkeystudio.desktop

		# desktop icon file
		monkey_desktopicon.path	= $${PACKAGE_PREFIX}/share/icons/hicolor/32x32/apps
		monkey_desktopicon.files	= links/monkeystudio.png

		INSTALLS	+= monkey_plugins monkey_target monkey_desktop monkey_desktopicon
	}
}
