# Monkey Studio 2 qCtagsSense project include file

QT	*= core gui sql
LIBS	*= -L$${PACKAGE_BUILD_PATH}

# needed dependencies
PRE_TARGETDEPS	*= ../ctags
include( ../ctags/ctags.pri )

CONFIG(debug, debug|release) {
	#Debug
	unix:LIBS	*= -lctags_debug
	else:LIBS	*= -lctagsd
} else {
	#Release
	LIBS	*= -lctags
}

# include path
INCLUDEPATH	*= $$PWD/sdk