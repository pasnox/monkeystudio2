###########################################################################################
##      Created using Monkey Studio v1.8.4.0b2 (1.8.4.0b2)
##
##  Author      : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
##  Project     : QtAssistant
##  FileName    : QtAssistant.pro
##  Date        : 2009-12-05T20:46:15
##  Description : Qt Assistant Integration
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

TARGET  = QtAssistant
include( ../../plugins.pri )
DESTDIR = $$MONKEY_PLUGINS_DIR/child
greaterThan(QT_MAJOR_VERSION, 4):QT *= help
else:CONFIG  *= help
QT  *= network

contains(QT_CONFIG, webkit) {
    QT *= webkit
    DEFINES *= WEBKIT_VIEWER
}

INCLUDEPATH *= src \
    src/3rdparty

RESOURCES   = src/resources/QtAssistant.qrc

FORMS   = src/3rdparty/bookmarkdialog.ui \
    src/3rdparty/filternamedialog.ui \
    src/3rdparty/installdialog.ui \
    src/3rdparty/preferencesdialog.ui \
    src/3rdparty/topicchooser.ui
    
HEADERS = src/QtAssistant.h \
    src/QtAssistantDock.h \
    src/MkSQtDocInstaller.h \
    src/3rdparty/bookmarkmanager.h \
    src/3rdparty/filternamedialog.h \
    src/3rdparty/installdialog.h \
    src/3rdparty/preferencesdialog.h \
    src/3rdparty/qtdocinstaller.h \
    src/3rdparty/topicchooser.h \
    src/QtAssistantChild.h \
    src/QtAssistantInlineSearch.h

SOURCES = src/QtAssistant.cpp \
    src/QtAssistantDock.cpp \
    src/MkSQtDocInstaller.cpp \
    src/3rdparty/bookmarkmanager.cpp \
    src/3rdparty/filternamedialog.cpp \
    src/3rdparty/installdialog.cpp \
    src/3rdparty/preferencesdialog.cpp \
    src/3rdparty/qtdocinstaller.cpp \
    src/3rdparty/topicchooser.cpp \
    src/QtAssistantChild.cpp \
    src/QtAssistantInlineSearch.cpp

contains(DEFINES, WEBKIT_VIEWER) {
    INCLUDEPATH *= src/viewers/webkit
    
    HEADERS *= src/viewers/webkit/QtAssistantViewer.h
    SOURCES *= src/viewers/webkit/QtAssistantViewer.cpp
} else {
    INCLUDEPATH *= src/viewers/textbrowser
    
    HEADERS *= src/viewers/textbrowser/QtAssistantViewer.h
    SOURCES *= src/viewers/textbrowser/QtAssistantViewer.cpp
    
    message( "Building QtAssistant plugin using QTextBrowser, it's recommanded to use QtWebKit for better performance and rendering." )
}

include( src/3rdparty/fontpanel/fontpanel.pri )