# Monkey Studio Install Project File

include( config.pri )

!build_pass {
	message( "Monkey Studio binary will be installed to : $$PACKAGE_PREFIX" )
	message( "Monkey Studio datas will be installed to : $$PACKAGE_DATAS" )
	message( "You can change this by exporting these variables to your shell before calling qmake : MONKEY_PREFIX, MONKEY_DATAS" )

	# templates
	templates.path	= $${PACKAGE_DATAS}
	templates.files	= templates

	# apis
	apis.path	= $${PACKAGE_DATAS}
	apis.files	= ctags/apis

	# translations
	translations.path	= $${PACKAGE_DATAS}/translations
	translations.files	= translations/*.qm

	# debugger know_list_and_id
	debuggerIniFile.path	= $${PACKAGE_DATAS}/plugins/GNUdbg
	debuggerIniFile.files	= plugins/debugger/GNUDebugger/file

	debuggerLog.path	= $${PACKAGE_DATAS}/plugins/GNUdbg
	debuggerLog.files	= plugins/debugger/GNUDebugger/log

	debuggerScript.path	= $${PACKAGE_DATAS}/plugins/GNUdbg
	debuggerScript.files	= plugins/debugger/GNUDebugger/scripts

	INSTALLS	= apis templates translations debuggerIniFile debuggerLog debuggerScript

	unix:!mac {
		# plugins
		plugins.path	= $${PACKAGE_DATAS}
		plugins.files	= bin/plugins

		# binary
		target.path	= $${PACKAGE_PREFIX}
		target.files	= bin/$${PACKAGE_TARGET}

		# desktop file
		desktop.path	= $${PACKAGE_PREFIX}/../share/applications
		desktop.files	= links/monkeystudio.desktop

		# desktop icon file
		desktopicon.path	= $${PACKAGE_PREFIX}/../icons/hicolor/32x32/apps
		desktopicon.files	= links/monkeystudio.png

		INSTALLS	+= plugins target desktop desktopicon
	}
}
