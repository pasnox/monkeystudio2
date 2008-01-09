# Monkey Studio Install Project File

# update & release translations
update_ts {
	unix:!mac {
		system( lupdate-qt4 monkey.pro )
		system( lrelease-qt4 monkey.pro )
	} else {
		system( lupdate monkey.pro )
		system( lrelease monkey.pro )
	}
}
	
# install script

isEmpty( $$PREFIX ):PREFIX	= /usr/local
mac:PREFIX	= $${DESTDIR}/$${TARGET}.app/Contents
isEmpty( $$PREFIX_DATAS ):PREFIX_DATAS	= $${PREFIX}/lib/$${TARGET}
mac:PREFIX_DATAS	= $$PREFIX

# templates
templates.path	= $$PREFIX_DATAS
templates.files	= ../templates

# apis
apis.path	= $$PREFIX_DATAS
apis.files	= ../ctags/apis

# translations
translations.path	= $$PREFIX_DATAS/translations
translations.files	= ../translations/*.qm

unix:!mac {
	# plugins
	plugins.path	= $$PREFIX_DATAS
	plugins.files	= $${DESTDIR}/plugins
	
	# binary
	target.path	= $${PREFIX}/bin
}

INSTALLS	+= apis templates translations plugins target
