TARGET	= QMakeProjectEditor
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup
QT	*= xml

RESOURCES	*= src/resources/QMakeProjectEditor.qrc

FORMS	*= src/ui/UISettingsQMakeXUPProjectEditor.ui \
	src/ui/UIQtConfiguration.ui

HEADERS	*= src/QMakeProjectEditor.h \
	src/QMake2XUP.h \
	src/QMakeXUPItem.h \
	src/QtVersionManager.h \
	src/ui/UISettingsQMakeXUPProjectEditor.h \
	src/QMakeProjectEditorDialog.h

SOURCES	*= src/QMakeProjectEditor.cpp \
	src/QMake2XUP.cpp \
	src/QMakeXUPItem.cpp \
	src/QtVersionManager.cpp \
	src/ui/UISettingsQMakeXUPProjectEditor.cpp \
	src/QMakeProjectEditorDialog.cpp
