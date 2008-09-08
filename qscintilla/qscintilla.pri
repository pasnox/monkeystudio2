# Monkey Studio 2.0.0 QScintilla Project file
include( qscintilla_check.pri )

QSCINTILLA_PWD	= $$PWD

# include sdk path
INCLUDEPATH	*= $${QSCINTILLA_PWD}/sdk

isEqual( SYSTEM_QSCINTILLA, 1 ) {
	# include path
	INCLUDEPATH	*= $$[QT_INSTALL_HEADERS]/Qsci
} else {
	# define where is the official qscintilla folder
	QSCINTILLAVERSION	= QScintilla-gpl-2.2.1-snapshot-20080309
	# include path
	INCLUDEPATH	*= $${QSCINTILLA_PWD}/$$QSCINTILLAVERSION/Qt4 $${QSCINTILLA_PWD}/$$QSCINTILLAVERSION/Qt4/Qsci
}
