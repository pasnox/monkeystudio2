XUP.QT_VERSION = Qt System (4.7.2)
XUP.WIN_PLATFORM_TARGET_RELEASE = bin/monkeystudio.exe
XUP.WIN_PLATFORM_TARGET_DEBUG = bin/monkeystudiod.exe
XUP.WIN_PLATFORM_TARGET_DEFAULT = bin/monkeystudiod.exe
XUP.MAC_PLATFORM_TARGET_RELEASE = bin/Monkey Studio.app
XUP.MAC_PLATFORM_TARGET_DEBUG = bin/Monkey Studio_debug.app
XUP.MAC_PLATFORM_TARGET_DEFAULT = bin/Monkey Studio_debug.app
XUP.OTHERS_PLATFORM_TARGET_RELEASE = bin/monkeystudio
XUP.OTHERS_PLATFORM_TARGET_DEBUG = bin/monkeystudio_debug
XUP.OTHERS_PLATFORM_TARGET_DEFAULT = bin/monkeystudio_debug

# include install script
include( installs.pri )

TEMPLATE = subdirs

CONFIG *= ordered

SUBDIRS = fresh \
    ctags \
    qCtagsSense \
    monkey \
    plugins

!isEqual( SYSTEM_QSCINTILLA, 1 ) {
    SUBDIRS = qscintilla \
        $$SUBDIRS
}