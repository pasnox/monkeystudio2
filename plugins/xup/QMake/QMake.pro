TARGET  = QMake
include( ../../plugins.pri )
CONFIG  *= exceptions
DESTDIR = $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH *= src src/settings src/editor

RESOURCES   = src/resources/QMake.qrc

FORMS   = src/settings/UISettingsQMake.ui \
    src/editor/QMakeMainEditor.ui \
    src/editor/QMakeConfigurationEditor.ui

HEADERS = src/QMake.h \
    src/QMake2XUP.h \
    src/QMakeProjectItem.h \
    src/settings/UISettingsQMake.h \
    src/QtVersionManager.h \
    src/editor/QMakeMainEditor.h \
    src/editor/UIQMakeEditor.h \
    src/editor/QMakeConfigurationEditor.h \
    src/editor/QMakeFilesEditor.h \
    src/editor/QMakeVariablesEditor.h

SOURCES = src/QMake.cpp \
    src/QMake2XUP.cpp \
    src/QMakeProjectItem.cpp \
    src/settings/UISettingsQMake.cpp \
    src/QtVersionManager.cpp \
    src/editor/QMakeMainEditor.cpp \
    src/editor/UIQMakeEditor.cpp \
    src/editor/QMakeConfigurationEditor.cpp \
    src/editor/QMakeFilesEditor.cpp \
    src/editor/QMakeVariablesEditor.cpp
