TARGET  = PyQt
include( ../../plugins.pri )
DESTDIR = $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH *= src .

RESOURCES   *= src/resources/PyQt.qrc


HEADERS *= src/PyQt.h \
    src/PyQtProjectItem.h

SOURCES = src/PyQt.cpp \
    src/PyQtProjectItem.cpp