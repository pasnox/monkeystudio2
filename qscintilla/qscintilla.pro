# Monkey Studio 2.0.0 QScintilla Project file

include( qscintilla.pri )

# include original project
include( $$QSCINTILLAVERSION/Qt4/qscintilla.pro )

include( qscintilla.pri )

# overwrite some values to made no lib
CONFIG	-= dll release
