TARGET	= QtAssistant
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/child

CONFIG	*= help
QT	*= network webkit

INCLUDEPATH	*= src src/3rdparty

RESOURCES	= src/resources/QtAssistant.qrc

include( src/3rdparty/fontpanel/fontpanel.pri )

FORMS	*= src/ui/QtAssistantBrowser.ui \
	src/ui/QtAssistantDock.ui \
	src/ui/UIQtAssistantSettings.ui \
	src/3rdparty/topicchooser.ui \
	src/3rdparty/bookmarkdialog.ui \
	src/3rdparty/filternamedialog.ui \
	src/3rdparty/preferencesdialog.ui \
	src/3rdparty/installdialog.ui

HEADERS	*= src/QtAssistant.h \
	src/ui/QtAssistantDock.h \
	src/ui/QtAssistantBrowser.h \
	src/ui/UIQtAssistantSettings.h \
	src/HelpViewer.h \
	src/3rdparty/topicchooser.h \
	src/3rdparty/qtdocinstaller.h \
	src/MkSQtDocInstaller.h \
	src/QtAssistantInlineSearch.h \
	src/3rdparty/installdialog.h \
	src/3rdparty/bookmarkmanager.h \
	src/3rdparty/preferencesdialog.h \
	src/3rdparty/filternamedialog.h

SOURCES	*= src/QtAssistant.cpp \
	src/ui/QtAssistantBrowser.cpp \
	src/ui/QtAssistantDock.cpp \
	src/ui/UIQtAssistantSettings.cpp \
	src/HelpViewer.cpp \
	src/3rdparty/topicchooser.cpp \
	src/3rdparty/qtdocinstaller.cpp \
	src/MkSQtDocInstaller.cpp \
	src/QtAssistantInlineSearch.cpp \
	src/3rdparty/installdialog.cpp \
	src/3rdparty/filternamedialog.cpp \
	src/3rdparty/bookmarkmanager.cpp \
	src/3rdparty/preferencesdialog.cpp
