# Monkey Studio 2 qCtagsSense project include file

# needed dependencies
include( $${PWD}/../ctags/ctags.pri )

# include path
INCLUDEPATH *= $${PWD}/src
#INCLUDEPATH    *= $$getFolders( $${PWD}/src, resources )

# dependency
PRE_TARGETDEPS  *= $${PWD}

# library integration
QCTAGSSENSE_TARGET  = qCtagsSense
LIBS    *= -L$${PACKAGE_BUILD_PATH}

contains( TEMPLATE, .*app ) {
    CONFIG(debug, debug|release) {
        #Debug
        unix:LIBS   *= -l$${QCTAGSSENSE_TARGET}_debug
        else:LIBS   *= -l$${QCTAGSSENSE_TARGET}d
    } else {
        #Release
        LIBS    *= -l$${QCTAGSSENSE_TARGET}
    }
}
