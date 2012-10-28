# Monkey Studio 2.0.0 QScintilla Project file

# include functions file
include( ../functions.pri )

# include qscintilla check
include( qscintilla_check.pri )

# include path
isEqual( SYSTEM_QSCINTILLA, 1 ) {
    # include path
    INCLUDEPATH *= $$[QT_INSTALL_HEADERS]/Qsci
} else {
    # define where is the official qscintilla folder
    QSCINTILLAVERSION   = ./QScintilla-gpl-snapshot
    
    # include original project
    include( $$QSCINTILLAVERSION/Qt4Qt5/qscintilla.pro )
    
    # include path
    QSCINTILLA_SOURCES_PATHS = $$getFolders( $${QSCINTILLAVERSION}, designer-Qt3 designer-Qt4 example-Qt3 example-Qt4Qt5 Qt3 doc Python python api )
    INCLUDEPATH -= .
    INCLUDEPATH -= ../include
    INCLUDEPATH -= ../src
    INCLUDEPATH -= ../lexers
    INCLUDEPATH -= ../lexlib
    INCLUDEPATH *= $${QSCINTILLAVERSION}/include $${QSCINTILLAVERSION}/src $${QSCINTILLAVERSION}/lexers $${QSCINTILLAVERSION}/lexlib $${QSCINTILLAVERSION}/Qt4Qt5 $${QSCINTILLAVERSION}/Qt4/Qsci
    #INCLUDEPATH    *= $${QSCINTILLA_SOURCES_PATHS}
    DEPENDPATH  *= $${QSCINTILLA_SOURCES_PATHS}

    # overwrite some values to made no lib
    CONFIG  -= dll
    CONFIG  -= release
    CONFIG  *= staticlib exceptions

    # remove install script of qscintilla
    INSTALLS    -= header trans qsci target
}

greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets printsupport

# include config file
include( ../config.pri )

DESTDIR = $${PACKAGE_BUILD_PATH}/$${Q_TARGET_ARCH}/$$buildMode()