# get current path
PACKAGE_PWD	= $$PWD

# package name
PACKAGE_TARGET	= monkeystudio
mac:PACKAGE_TARGET	= $$quote(Monkey Studio)

# package destdir
PACKAGE_DESTDIR	= $${PACKAGE_PWD}/bin

# temporary path for building
PACKAGE_BUILD_PATH	= $${PACKAGE_PWD}/build

# build mode
CONFIG	+= qt warn_on thread x11 windows debug
QT	+= xml

# define config mode paths
CONFIG( debug, debug|release ) {
	#Debug
	message( Building in DEBUG for architecture $$QT_ARCH )
	CONFIG	+= console
	unix:PACKAGE_TARGET	= $$quote($$join(PACKAGE_TARGET,,,_debug))
	else:PACKAGE_TARGET	= $$quote($$join(PACKAGE_TARGET,,,d))
	unix:TARGET	= $$quote($$join(TARGET,,,_debug))
	else:TARGET	= $$quote($$join(TARGET,,,d))
	unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/debug/.obj/mac
	UI_DIR	= $${PACKAGE_BUILD_PATH}/debug/.ui
	MOC_DIR	= $${PACKAGE_BUILD_PATH}/debug/.moc
	RCC_DIR	= $${PACKAGE_BUILD_PATH}/debug/.rcc
} else {
	#Release
	message( Building in RELEASE for architecture $$QT_ARCH )
	mac:TARGET	= $$quote($$TARGET)
	unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${PACKAGE_BUILD_PATH}/release/.ui
	MOC_DIR	= $${PACKAGE_BUILD_PATH}/release/.moc
	RCC_DIR	= $${PACKAGE_BUILD_PATH}/release/.rcc
}

# define some usefull values
QMAKE_TARGET_COMPANY	= "Monkey Studio Team"
QMAKE_TARGET_PRODUCT	= "Monkey Studio"
QMAKE_TARGET_DESCRIPTION	= "Crossplatform Integrated Development Environment"
QMAKE_TARGET_COPYRIGHT	= "Copyright (C) 2005 - 2008 Filipe AZEVEDO"
PACKAGE_DOMAIN	= "www.monkeystudio.org"

PACKAGE_VERSION	= 1.8.3.0

CONFIG( debug, debug|release ) {
	PACKAGE_VERSION	= $${PACKAGE_VERSION}svn_debug
} else {
	PACKAGE_VERSION	= $${PACKAGE_VERSION}svn_release
}

# define variable for source code
DEFINES	*= "PACKAGE_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"" \
	"PACKAGE_VERSION=\"\\\"$${PACKAGE_VERSION}\\\"\"" \
	"PACKAGE_DOMAIN=\"\\\"$${PACKAGE_DOMAIN}\\\"\"" \
	"PACKAGE_COPYRIGHTS=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\""

# get package install paths

unix:!mac {
	# default prefix path
	isEmpty( prefix ):prefix = /usr

	!isEmpty( prefix ) {
		# plugins path
		isEmpty( plugins ) {
			isEqual( QT_ARCH, "i386" ) {
				plugins	= $$prefix/lib
			} else {
				plugins	= $$prefix/lib64
			}
		}
		
		# datas path
		isEmpty( datas ) {
			datas	= $$prefix/share
		}
	}
} else:mac {
	prefix	= $${PACKAGE_DESTDIR}/$${PACKAGE_TARGET}.app/Contents
	plugins	= $${prefix}/plugins
	datas	= $${prefix}/Resources
} else:win32 {
	prefix	= $${PACKAGE_DESTDIR}
	plugins	= $${prefix}/plugins
	datas	= $${prefix}
}

unix:!mac {
	PACKAGE_PREFIX	= $$quote($$prefix/bin)
	PACKAGE_PLUGINS	= $$quote($$plugins/$$PACKAGE_TARGET)
	PACKAGE_DATAS	= $$quote($$datas/$$PACKAGE_TARGET)
} else:mac {
	PACKAGE_PREFIX	= $$quote($$prefix/MacOS)
	PACKAGE_PLUGINS	= $$quote($$plugins)
	PACKAGE_DATAS	= $$quote($$datas)
} else:win32 {
	PACKAGE_PREFIX	= $$quote($$prefix)
	PACKAGE_PLUGINS	= $$quote($$plugins)
	PACKAGE_DATAS	= $$quote($$datas)
}

# define package install paths so source code can use them
DEFINES	*= "PACKAGE_PREFIX=\"\\\"$${PACKAGE_PREFIX}\\\"\"" \
	"PACKAGE_PLUGINS=\"\\\"$${PACKAGE_PLUGINS}\\\"\"" \
	"PACKAGE_DATAS=\"\\\"$${PACKAGE_DATAS}\\\"\""

# qscintilla library
include( qscintilla/qscintilla_check.pri )