# build mode
CONFIG  *= qt warn_on thread x11 windows rtti release
CONFIG  -= warn_off debug debug_and_release x86 x86_64 ppc ppc64
QT  *= xml sql

# Mac universal build from 10.4 to up to 10.5
mac {
    QMAKE_MACOSX_DEPLOYMENT_TARGET  = 10.4
    #QMAKE_MAC_SDK   = /Developer/SDKs/MacOSX10.4u.sdk
    CONFIG  *= x86
    #CONFIG  *= x86_64
    #CONFIG  *= ppc
    #CONFIG  *= ppc64
    # this link is required for building the ppc port to avoid the undefined __Unwind_Resume symbol
    CONFIG( ppc ):LIBS *= -lgcc_eh
}

# get current path
PACKAGE_PWD = $$PWD

# helpers functions
include( $${PACKAGE_PWD}/functions.pri )

# package name
PACKAGE_TARGET  = $$targetForMode( monkeystudio )
mac:PACKAGE_TARGET  = $$targetForMode( "MonkeyStudio" )
TARGET = $$targetForMode( $${TARGET} )

# package destdir
PACKAGE_DESTDIR = $${PACKAGE_PWD}/bin

# temporary path for building
#PACKAGE_BUILD_PATH  = $${PACKAGE_PWD}/build
PACKAGE_BUILD_PATH  = $$(PWD)/build
RAMDISK_PATH = /media/ramdisk

exists( $${RAMDISK_PATH} ) {
    PACKAGE_BUILD_PATH  = $${RAMDISK_PATH}/monkeystudio
}

setTemporaryDirectories( $${PACKAGE_BUILD_PATH} )

# define config mode paths
CONFIG( debug, debug|release ) {
    message( Building in DEBUG for $${Q_TARGET_ARCH} )
    CONFIG  *= console
} else {
    message( Building in RELEASE for $${Q_TARGET_ARCH} )
}

INCLUDEPATH *= $${UI_DIR} # some qmake versions has bug and do not do it automatically

# define some usefull values
QMAKE_TARGET_COMPANY    = "The Monkey Studio Team"
QMAKE_TARGET_PRODUCT    = "Monkey Studio IDE"
QMAKE_TARGET_DESCRIPTION    = "Crossplatform Integrated Development Environment"
QMAKE_TARGET_COPYRIGHT  = "\\251 2005 - 2012 Filipe AZEVEDO and $$QMAKE_TARGET_COMPANY"
QMAKE_TARGET_DOMAIN  = "monkeystudio.org"
QMAKE_TARGET_VERSION = 1.9.0.4

CONFIG( debug, debug|release ) {
    QMAKE_TARGET_VERSION_STR = $${QMAKE_TARGET_VERSION}svn_debug
} else {
    QMAKE_TARGET_VERSION_STR = $${QMAKE_TARGET_VERSION}svn_release
}

QMAKE_TARGET_VERSION_STR = $${QMAKE_TARGET_VERSION}

# get package install paths

unix:!mac {
    # default prefix path
    isEmpty( prefix ):prefix = /usr/local

    !isEmpty( prefix ) {
        # plugins path
        isEmpty( plugins ) {
            isEqual( QT_ARCH, "i386" ) {
                plugins = $${prefix}/lib
            } else {
                plugins = $${prefix}/lib64
            }
        }

        # datas path
        isEmpty( datas ) {
            datas   = $${prefix}/share
        }
        
        # docs path
        isEmpty( docs ) {
            docs = $${datas}/doc
        }
    }
} else:mac {
    prefix  = $${PACKAGE_DESTDIR}/$${PACKAGE_TARGET}.app/Contents
    plugins = $${prefix}/plugins
    datas   = $${prefix}/Resources
    docs    = $${datas}
} else:win32 {
    prefix  = $${PACKAGE_DESTDIR}
    plugins = $${prefix}/plugins
    datas   = $${prefix}
    docs   = $${datas}
}

unix:!mac {
    PACKAGE_PREFIX  = $$quote($${prefix}/bin)
    PACKAGE_PLUGINS = $$quote($${plugins}/$${PACKAGE_TARGET})
    PACKAGE_DATAS   = $$quote($${datas}/$${PACKAGE_TARGET})
    PACKAGE_DOCS   = $$quote($${docs}/$${PACKAGE_TARGET})
} else:mac {
    PACKAGE_PREFIX  = $$quote($${prefix}/MacOS)
    PACKAGE_PLUGINS = $$quote($${plugins})
    PACKAGE_DATAS   = $$quote($${datas})
    PACKAGE_DOCS   = $$quote($${docs})
} else:win32 {
    PACKAGE_PREFIX  = $$quote($${prefix})
    PACKAGE_PLUGINS = $$quote($${plugins})
    PACKAGE_DATAS   = $$quote($${datas})
    PACKAGE_DOCS   = $$quote($${docs})
}

# qscintilla library
include( qscintilla/qscintilla_check.pri )
