# get current path
PACKAGE_PWD	= $$PWD

# package name
PACKAGE_TARGET	= frontend

# package destdir
PACKAGE_DESTDIR	= $${PACKAGE_PWD}/bin

# temporary path for building
PACKAGE_BUILD_PATH	= $${PACKAGE_PWD}/build

# build mode
CONFIG	+= qt warn_on thread x11 windows debug
QT	+= xml sql

# Mac universal build from 10.3 to up to 10.5
mac {
	QMAKE_MACOSX_DEPLOYMENT_TARGET	= 10.3
	QMAKE_MAC_SDK	= /Developer/SDKs/MacOSX10.4u.sdk
	CONFIG	+= x86 ppc
}

# define config mode paths
CONFIG( debug, debug|release ) {
	#Debug
	message( Building in DEBUG for architecture $$QT_ARCH )
	CONFIG	+= console
	unix:PACKAGE_TARGET	= $$quote($$join(PACKAGE_TARGET,,,_debug))
	else:PACKAGE_TARGET	= $$quote($$join(PACKAGE_TARGET,,,d))
	unix:TARGET	= $$quote($$join(TARGET,,,_debug))
	else:TARGET	= $$quote($$join(TARGET,,,d))
} else {
	#Release
	message( Building in RELEASE for architecture $$QT_ARCH )
	mac:TARGET	= $$quote($$TARGET)
}

unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/$${TARGET}/obj/unix
win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/$${TARGET}/obj/win32
mac:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/$${TARGET}/obj/mac
UI_DIR	= $${PACKAGE_BUILD_PATH}/$${TARGET}/ui
MOC_DIR	= $${PACKAGE_BUILD_PATH}/$${TARGET}/moc
RCC_DIR	= $${PACKAGE_BUILD_PATH}/$${TARGET}/rcc

INCLUDEPATH	*= $${UI_DIR} # some qmake versions has bug and do not do it automatically

# define some usefull values
QMAKE_TARGET_COMPANY	= "Monkey Studio Team"
QMAKE_TARGET_PRODUCT	= "qCtagsSense"
QMAKE_TARGET_DESCRIPTION	= "Code Sources Indexer"
QMAKE_TARGET_COPYRIGHT	= "Copyright (C) 2010 - Monkey Studio Team"
PACKAGE_DOMAIN	= "monkeystudio.org"

PACKAGE_VERSION	= 0.5.0

CONFIG( debug, debug|release ) {
	PACKAGE_VERSION_STR	= $${PACKAGE_VERSION}svn_debug
} else {
	PACKAGE_VERSION_STR	= $${PACKAGE_VERSION}svn_release
}

# define variable for source code
DEFINES	*= "_PACKAGE_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"" \
	"_PACKAGE_VERSION=\"\\\"$${PACKAGE_VERSION}\\\"\"" \
	"_PACKAGE_VERSION_STR=\"\\\"$${PACKAGE_VERSION_STR}\\\"\"" \
	"_PACKAGE_DOMAIN=\"\\\"$${PACKAGE_DOMAIN}\\\"\"" \
	"_PACKAGE_COPYRIGHTS=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\""