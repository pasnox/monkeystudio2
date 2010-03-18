XUPProjectSettings {
	QT_VERSION	= Qt System (4.6.2)
}

include( config.pri )

TEMPLATE	= subdirs

SUBDIRS	+= ctags \
	qCtagsSense \
	frontend