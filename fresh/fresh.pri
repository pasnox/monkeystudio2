# fresh includes path to include in plugins/core project

# include path
INCLUDEPATH *= $${PWD}/objects $${PWD}/widgets

# dependency
PRE_TARGETDEPS  *= $${PWD}

# library integration
FRESH_TARGET    = fresh
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets
LIBS    *= -L$${PACKAGE_BUILD_PATH}/$${Q_TARGET_ARCH}/$$buildMode()

contains( TEMPLATE, .*app ) {
    CONFIG(debug, debug|release) {
        #Debug
        unix:LIBS   *= -l$${FRESH_TARGET}_debug
        else:LIBS   *= -l$${FRESH_TARGET}d
    } else {
        #Release
        LIBS    *= -l$${FRESH_TARGET}
    }
}
