include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/xup
TARGET	= QMakeProjectEditor
QT	*= xml

RESOURCES	*= src/resources/QMakeProjectEditor.qrc

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

FORMS	*= src/ui/UISettingsQMakeXUPProjectEditor.ui \
	src/ui/UIQtConfiguration.ui