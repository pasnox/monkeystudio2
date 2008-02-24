# Monkey Studio Install Project File

# try getting infos from shell export
PROGRAM_PREFIX	= $$(MONKEY_PREFIX)
PROGRAM_DATAS	= $$(MONKEY_DATAS)
	
# prefix
isEmpty( PROGRAM_PREFIX ) {
	win32:PROGRAM_PREFIX	= $${DESTDIR}
	else:mac:PROGRAM_PREFIX	= $${DESTDIR}/$${TARGET}.app/Contents
	else:PROGRAM_PREFIX	= /usr/local
}

# prefixdatas
isEmpty( PROGRAM_DATAS ) {
	win32:PROGRAM_DATAS	= $$PROGRAM_PREFIX
	else:mac:PROGRAM_DATAS	= $$PROGRAM_PREFIX
	else:PROGRAM_DATAS	= $${PROGRAM_PREFIX}/lib/$${TARGET}
}

!CONFIG( debug, debug|release ) {
	message( "Monkey Studio binary will be installed to : $$PROGRAM_PREFIX" )
	message( "Monkey Studio datas will be installed to : $$PROGRAM_DATAS" )
	message( "You can change this by exporting these variables to your shell before calling qmake : MONKEY_PREFIX, MONKEY_DATAS" )
}

# templates
templates.path	= $$PROGRAM_DATAS
templates.files	= ../templates

# apis
apis.path	= $$PROGRAM_DATAS
apis.files	= ../ctags/apis

# translations
translations.path	= $$PROGRAM_DATAS/translations
translations.files	= ../translations/*.qm

# debugger know_list_and_id
debuggerIniFile.path		= $$PROGRAM_DATAS/plugins/debugger
debuggerIniFile.files		= ../plugins/debugger/GNUDebugger/file


INSTALLS	= apis templates translations debuggerIniFile

unix:!mac {
	# plugins
	plugins.path	= $$PROGRAM_DATAS
	plugins.files	= $${DESTDIR}/plugins
	
	# binary
	target.path	= $${PROGRAM_PREFIX}/bin

	# desktop file
	desktop.path	= $${PROGRAM_PREFIX}/share/applications
	desktop.files	= ../links/monkeystudio.desktop

	# desktop icon file
	desktopicon.path	= $${PROGRAM_PREFIX}/icons/hicolor/32x32/apps
	desktopicon.files	= ../links/monkeystudio.png
	
	INSTALLS	+= plugins target desktop desktopicon
}

