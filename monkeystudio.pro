XUP.QT_VERSION = Qt System (4.8.3)
XUP.WIN_PLATFORM_TARGET_RELEASE = bin/monkeystudio.exe
XUP.WIN_PLATFORM_TARGET_DEBUG = bin/monkeystudiod.exe
XUP.WIN_PLATFORM_TARGET_DEFAULT = bin/monkeystudiod.exe
XUP.MAC_PLATFORM_TARGET_RELEASE = bin/MonkeyStudio.app
XUP.MAC_PLATFORM_TARGET_DEBUG = bin/MonkeyStudio_debug.app
XUP.MAC_PLATFORM_TARGET_DEFAULT = bin/MonkeyStudio_debug.app
XUP.OTHERS_PLATFORM_TARGET_RELEASE = bin/monkeystudio
XUP.OTHERS_PLATFORM_TARGET_DEBUG = bin/monkeystudio_debug
XUP.OTHERS_PLATFORM_TARGET_DEFAULT = bin/monkeystudio_debug

# include install script
include( installs.pri )

TEMPLATE = subdirs

CONFIG *= ordered

qt4_initvars.file = qt4-initvars.pro
qt4_initvars.depends =

fresh.subdir = fresh
fresh.depends = qt4_initvars

ctags.subdir = ctags
ctags.depends = qt4_initvars

qCtagsSense.subdir = qCtagsSense
qCtagsSense.depends = qt4_initvars

monkey.subdir = monkey
monkey.depends = qt4_initvars

plugins.subdir = plugins
plugins.depends = qt4_initvars

qscintilla.subdir = qscintilla
qscintilla.depends = qt4_initvars

isEqual( QT_MAJOR_VERSION, 4 ) {
    SUBDIRS *= qt4_initvars
}

isEqual( SYSTEM_QSCINTILLA, 0 ) {
    SUBDIRS *= qscintilla
}

SUBDIRS *= fresh \
    ctags \
    qCtagsSense \
    monkey \
    plugins
