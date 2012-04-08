TARGET  = XUP
include( ../../plugins.pri )
DESTDIR = $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH *= src

FORMS   =  \
    src/gui/UIXUPEditor.ui \
    src/gui/CommandsEditor.ui

HEADERS =  \
    src/XUP.h \
    src/gui/UIXUPEditor.h \
    src/gui/CommandsEditor.h \
    src/XUPProjectItemHelper.h

SOURCES =  \
    src/XUP.cpp \
    src/gui/UIXUPEditor.cpp \
    src/gui/CommandsEditor.cpp \
    src/XUPProjectItemHelper.cpp
