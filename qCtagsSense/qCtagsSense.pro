# Monkey Studio 2 qCtagsSense library

include( ../ctags/ctags.pri )

# include config file
include( ../config.pri )

TEMPLATE    = lib
CONFIG  *= staticlib
DEFINES *= QCTAGSSENSE_EXPORT_BUILD
DESTDIR = $${PACKAGE_BUILD_PATH}

INCLUDEPATH *= ../../dev/fresh/objects

FORMS   = 

HEADERS = src/qCtagsSense.h \
    src/qCtagsSenseExport.h \
    src/qCtagsSenseEntry.h \
    src/qCtagsSenseProperties.h \
    ../../dev/fresh/objects/pIconManager.h \
    src/qCtagsSenseGlobals.h

SOURCES = src/qCtagsSense.cpp \
    src/qCtagsSenseEntry.cpp \
    src/qCtagsSenseProperties.cpp \
    ../../dev/fresh/objects/pIconManager.cpp \
    src/qCtagsSenseGlobals.cpp