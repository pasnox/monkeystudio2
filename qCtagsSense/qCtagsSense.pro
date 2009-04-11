# Monkey Studio 2 qCtagsSense library

#include qCtagsSense project settings
include( qCtagsSense.pri )

# include config file
include( ../config.pri )

TEMPLATE	= lib
CONFIG	*= staticlib
QT	*= sql
DEFINES	*= QCTAGSSENSE_EXPORT_BUILD
DESTDIR	= $${PACKAGE_BUILD_PATH}

PRE_TARGETDEPS	*= ../ctags
LIBS	*= -L$${PACKAGE_BUILD_PATH}
CONFIG(debug, debug|release) {
	#Debug
	unix:LIBS	*= -lctags_debug
	else:LIBS	*= -lctagsd
} else {
	#Release
	LIBS	*= -lctags
}

RESOURCES	= src/resources/qCtagsSense.qrc

FORMS	= src/qCtagsSenseBrowser.ui

HEADERS	= src/qCtagsSense.h \
	src/qCtagsSenseIndexer.h \
	src/qCtagsSenseBrowser.h \
	src/qCtagsSenseMembersModel.h \
	src/qCtagsSenseFilesModel.h \
	src/qCtagsSenseSQL.h \
	src/qCtagsSenseLanguagesModel.h \
	src/qCtagsSenseUtils.h \
	src/qCtagsSenseKindFinder.h \
	src/qCtagsSenseExport.h

SOURCES	= src/qCtagsSense.cpp \
	src/qCtagsSenseIndexer.cpp \
	src/qCtagsSenseBrowser.cpp \
	src/qCtagsSenseMembersModel.cpp \
	src/qCtagsSenseFilesModel.cpp \
	src/qCtagsSenseSQL.cpp \
	src/qCtagsSenseLanguagesModel.cpp \
	src/qCtagsSenseUtils.cpp \
	src/qCtagsSenseKindFinder.cpp