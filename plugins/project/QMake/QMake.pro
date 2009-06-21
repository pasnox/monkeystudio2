include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/project
TARGET	= QMake
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""
INCLUDEPATH	*= src/ui
RESOURCES	= src/resources/QMake.qrc

FORMS	= src/ui/UISettingsQMake.ui \
	src/ui/UIItemSettings.ui \
	src/ui/UIQMakeProjectSettings.ui

HEADERS = src/QMake.h \
	src/QMakeParser.h \
	src/QMakeItem.h \
	src/QMakeItemDelegate.h \
	src/ui/UISettingsQMake.h \
	src/ui/UIItemSettings.h \
	src/QMakeProxy.h \
	src/ui/UIQMakeProjectSettings.h

SOURCES	= src/QMake.cpp \
	src/QMakeParser.cpp \
	src/QMakeItem.cpp \
	src/QMakeItemDelegate.cpp \
	src/ui/UISettingsQMake.cpp \
	src/ui/UIItemSettings.cpp \
	src/QMakeProxy.cpp \
	src/ui/UIQMakeProjectSettings.cpp
