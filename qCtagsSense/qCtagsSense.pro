# Monkey Studio 2 qCtagsSense library

include( ../ctags/ctags.pri )

# include config file
include( ../config.pri )

TEMPLATE	= lib
CONFIG	*= staticlib
DEFINES	*= QCTAGSSENSE_EXPORT_BUILD
DESTDIR	= $${PACKAGE_BUILD_PATH}

HEADERS	+= src/qCtagsSense.h \
	src/qCtagsSenseExport.h \
	src/qCtagsSenseUtils.h \
	src/qCtagsSenseEntry.h
SOURCES	+= src/qCtagsSense.cpp \
	src/qCtagsSenseUtils.cpp \
	src/qCtagsSenseEntry.cpp