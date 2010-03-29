XUPProjectSettings {
	QT_VERSION	= Qt System (4.6.2)
	OTHERS_PLATFORM_TARGET_DEBUG	= bin/frontend
}

include( config.pri )

TEMPLATE	= subdirs
CONFIG	*= ordered

SUBDIRS	+= ctags \
	qCtagsSense \
	frontend