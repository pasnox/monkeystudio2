# Monkey Studio Install Project File

# update & release translations
unix:!mac {
	system( lupdate-qt4 monkey.pro )
	system( lrelease-qt4 monkey.pro )
} else {
	system( lupdate monkey.pro )
	system( lrelease monkey.pro )
}
	
# install script

# templates
templates.path	= $$DESTDIR
mac:templates.path	= $${DESTDIR}/$${TARGET}.app/Contents
else:unix:templates.path	= /usr/lib/$${TARGET}
templates.files	= ../templates

# apis
apis.path	= $$DESTDIR
mac:apis.path	= $${DESTDIR}/$${TARGET}.app/Contents
else:unix:apis.path	= /usr/lib/$${TARGET}
apis.files	= ../ctags/apis

# translations
translations.path	= $$DESTDIR
mac:translations.path	= $${DESTDIR}/$${TARGET}.app/Contents/translations
else:unix:translations.path	= /usr/lib/$${TARGET}/translations
translations.files	= ../translations/*.qm

unix:!mac {
	# plugins
	plugins.path	= /usr/lib/$${TARGET}
	plugins.files	= $${DESTDIR}/plugins

	# binary
	target.path	= /usr/bin

}

INSTALLS	+= apis templates translations plugins target