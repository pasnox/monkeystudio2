# Monkey Studio Install Project File

# update & release translations
system( lupdate monkey.pro )
system( lrelease monkey.pro )
	
# install script

# templates
templates.path = $$DESTDIR
mac:templates.path = $${DESTDIR}/$${TARGET}.app/Contents
templates.files = ../templates

# apis
apis.path = $$DESTDIR
mac:apis.path = $${DESTDIR}/$${TARGET}.app/Contents
apis.files = ../ctags/apis

# translations
translations.path = $$DESTDIR
mac:translations.path = $${DESTDIR}/$${TARGET}.app/Contents/translations
translations.files = ../translations/*.qm

INSTALLS += apis templates translations