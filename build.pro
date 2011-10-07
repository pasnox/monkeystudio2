XUPProjectSettings {
    QT_VERSION  = Qt System (4.7.2)
    WIN_PLATFORM_TARGET_RELEASE = bin/monkeystudio.exe
    WIN_PLATFORM_TARGET_DEBUG   = bin/monkeystudiod.exe
    MAC_PLATFORM_TARGET_RELEASE = bin/Monkey Studio.app
    MAC_PLATFORM_TARGET_DEBUG   = bin/Monkey Studio_debug.app
    OTHERS_PLATFORM_TARGET_RELEASE  = bin/monkeystudio
    OTHERS_PLATFORM_TARGET_DEBUG    = bin/monkeystudio_debug
}

# include install script
include( installs.pri )

TEMPLATE    = subdirs

CONFIG  *= ordered

SUBDIRS = fresh \
    ctags \
    qCtagsSense \
    monkey \
    plugins

!isEqual( SYSTEM_QSCINTILLA, 1 ) {
    SUBDIRS = qscintilla $$SUBDIRS
}
