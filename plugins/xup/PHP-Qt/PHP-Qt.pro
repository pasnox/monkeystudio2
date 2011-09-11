TARGET	= PHP-Qt
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH	*= src .

RESOURCES	*= src/resources/PHP-Qt.qrc

HEADERS	*= src/PHPQt.h \
	src/PHPQtProjectItem.h

SOURCES	*= src/PHPQt.cpp \
	src/PHPQtProjectItem.cpp