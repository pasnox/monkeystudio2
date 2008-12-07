TARGET	= PHP-Qt
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH	*= src/gui

RESOURCES	= src/resources/PHP-Qt.qrc

HEADERS	= src/PHPQt.h \
	src/PHPQtProjectItem.h \
	src/gui/UIPHPQtEditor.h

SOURCES	= src/PHPQt.cpp \
	src/PHPQtProjectItem.cpp \
	src/gui/UIPHPQtEditor.cpp
FORMS	= src/gui/UIPHPQtEditor.ui