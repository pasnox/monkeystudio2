# Monkey Studio 2.0.0 QScintilla Project file

include( qscintilla.pri )

# include original project
include( $$QSCINTILLAVERSION/Qt4/qscintilla.pro )

# include path path
INCLUDEPATH	*= $$QSCINTILLAVERSION/include
INCLUDEPATH	*= $$QSCINTILLAVERSION/src
INCLUDEPATH	*= $$QSCINTILLAVERSION/Qt4
# depend path
DEPENDPATH	*= $$QSCINTILLAVERSION/Qt4

# overwrite some values to made no lib
CONFIG	-= dll
CONFIG	-= release
CONFIG	+= staticlib
DEFINES	-= QSCINTILLA_MAKE_DLL

# remove install script of qscintilla
INSTALLS	-= header trans qsci target

# include config file
include( ../config.pri )

DESTDIR	= $${PACKAGE_BUILD_PATH}