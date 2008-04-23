TARGET	= MessageBox
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
QT	*= xml
RESOURCES	= src/resources/MessageBox.qrc

FORMS	= src/ui/UIBuildStep.ui \
	src/ui/UIOutput.ui \
	src/ui/UICommand.ui \
	src/ui/UISearchResult.ui

HEADERS	= src/ui/MessageBoxDocks.h \
	src/MessageBox.h

SOURCES	= src/ui/MessageBoxDocks.cpp \
	src/MessageBox.cpp
