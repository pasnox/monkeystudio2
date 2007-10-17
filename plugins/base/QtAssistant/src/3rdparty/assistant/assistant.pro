TEMPLATE = app
LANGUAGE = C++
TARGET = assistant

CONFIG += qt warn_on

unix:contains(QT_CONFIG, qdbus):CONFIG += qdbus

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}
QT += xml network

PROJECTNAME = Assistant
DESTDIR = ../../bin

FORMS += helpdialog.ui \
        mainwindow.ui \
        tabbedbrowser.ui \
        topicchooser.ui

SOURCES += main.cpp \
        helpwindow.cpp \
        topicchooser.cpp \
        docuparser.cpp \
        index.cpp \
        profile.cpp \
        config.cpp \
        helpdialog.cpp \
        mainwindow.cpp \
        tabbedbrowser.cpp

HEADERS += helpwindow.h \
        topicchooser.h \
        docuparser.h \
        index.h \
        profile.h \
        helpdialog.h \
        mainwindow.h \
        tabbedbrowser.h \
        config.h

RESOURCES += assistant.qrc

contains(QT_PRODUCT, OpenSource.*):DEFINES *= QT_OPENSOURCE
DEFINES += QT_KEYWORDS
#DEFINES +=  QT_PALMTOPCENTER_DOCS

win32 {
    LIBS += -lshell32
    RC_FILE = assistant.rc
}

mac {
    ICON = assistant.icns
    TARGET = assistant
#    QMAKE_INFO_PLIST = Info_mac.plist
}

target.path=$$[QT_INSTALL_BINS]
INSTALLS += target

#assistanttranslations.files = *.qm
#assistanttranslations.path = $$[QT_INSTALL_TRANSLATIONS]
#INSTALLS += assistanttranslations

TRANSLATIONS        = assistant_de.ts \
                  assistant_fr.ts


unix:!contains(QT_CONFIG, zlib):LIBS += -lz

contains(CONFIG, static): {
    win32 {
        exists($$[QT_INSTALL_PLUGINS]/imageformats/qjpeg.lib) {
            QTPLUGIN += qjpeg
            DEFINES += USE_STATIC_JPEG_PLUGIN            
        }
    } else {
        exists($$[QT_INSTALL_PLUGINS]/imageformats/qjpeg.a) {
            QTPLUGIN += qjpeg
            DEFINES += USE_STATIC_JPEG_PLUGIN            
        }        
    }
}
