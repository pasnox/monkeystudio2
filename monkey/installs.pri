# Monkey Studio Install Project File
	
# prefix and prefixdatas
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

INSTALLS	+= apis templates translations

unix:!mac {
	# plugins
	plugins.path	= $$PREFIX_DATAS
	plugins.files	= $${DESTDIR}/plugins
	
	# binary
	target.path	= $${PREFIX}/bin
	
	INSTALLS	+= plugins target
}
