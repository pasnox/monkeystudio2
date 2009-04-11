# Monkey Studio 2 qCtagsSense library

#include qCtagsSense project settings
include( qCtagsSense.pri )

# include config file
include( ../config.pri )

TEMPLATE	= lib
CONFIG	*= staticlib
DESTDIR	= $${PACKAGE_BUILD_PATH}

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
	src/qCtagsSenseKindFinder.h

SOURCES	= src/qCtagsSense.cpp \
	src/qCtagsSenseIndexer.cpp \
	src/qCtagsSenseBrowser.cpp \
	src/qCtagsSenseMembersModel.cpp \
	src/qCtagsSenseFilesModel.cpp \
	src/qCtagsSenseSQL.cpp \
	src/qCtagsSenseLanguagesModel.cpp \
	src/qCtagsSenseUtils.cpp \
	src/qCtagsSenseKindFinder.cpp