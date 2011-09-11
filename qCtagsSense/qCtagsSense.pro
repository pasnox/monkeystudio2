# Monkey Studio 2 qCtagsSense library

# include functions file
include( ../functions.pri )

# include config file
include( ../config.pri )

# needed dependencies
include( ../ctags/ctags.pri )

TEMPLATE	= lib
CONFIG	*= staticlib
QT	*= sql
DEFINES	*= QCTAGSSENSE_EXPORT_BUILD
DESTDIR	= $${PACKAGE_BUILD_PATH}

QCTAGSSENSE_SOURCES_PATHS = $$getFolders( ./src, resources )
INCLUDEPATH	*= src
#INCLUDEPATH	*= $${QCTAGSSENSE_SOURCES_PATHS}
DEPENDPATH	*= $${QCTAGSSENSE_SOURCES_PATHS}

RESOURCES	*= src/resources/qCtagsSense.qrc

FORMS	*= src/qCtagsSenseBrowser.ui

HEADERS	*= src/qCtagsSense.h \
	src/qCtagsSenseIndexer.h \
	src/qCtagsSenseBrowser.h \
	src/qCtagsSenseMembersModel.h \
	src/qCtagsSenseFilesModel.h \
	src/qCtagsSenseSQL.h \
	src/qCtagsSenseLanguagesModel.h \
	src/qCtagsSenseUtils.h \
	src/qCtagsSenseKindFinder.h \
	src/qCtagsSenseExport.h \
	src/qCtagsSenseSearchModel.h

SOURCES	*= src/qCtagsSense.cpp \
	src/qCtagsSenseIndexer.cpp \
	src/qCtagsSenseBrowser.cpp \
	src/qCtagsSenseMembersModel.cpp \
	src/qCtagsSenseFilesModel.cpp \
	src/qCtagsSenseSQL.cpp \
	src/qCtagsSenseLanguagesModel.cpp \
	src/qCtagsSenseUtils.cpp \
	src/qCtagsSenseKindFinder.cpp \
	src/qCtagsSenseSearchModel.cpp