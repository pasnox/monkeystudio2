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

# define config mode paths
CONFIG( debug, debug|release ) {
	#Debug
	message( Building in DEBUG )
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
	message( Building in RELEASE )
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
PACKAGE_VERSION	= 1.8.2.0svn

# define variable for source code
DEFINES	*= "PACKAGE_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"" \
	"PACKAGE_VERSION=\"\\\"$${PACKAGE_VERSION}\\\"\"" \
	"PACKAGE_DOMAIN=\"\\\"$${PACKAGE_DOMAIN}\\\"\"" \
	"PACKAGE_COPYRIGHTS=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\""

# get package install paths
PACKAGE_PREFIX	= $$(MONKEY_PREFIX)
PACKAGE_DATAS	= $$(MONKEY_DATAS)

# prefix
isEmpty( PACKAGE_PREFIX ) {
	win32:PACKAGE_PREFIX	= $${PACKAGE_DESTDIR}
	else:mac:PACKAGE_PREFIX	= $${PACKAGE_DESTDIR}/$${PACKAGE_TARGET}.app/Contents
	else:PACKAGE_PREFIX	= /usr/local
}

# datas
isEmpty( PACKAGE_DATAS ) {
	win32:PACKAGE_DATAS	= $${PACKAGE_PREFIX}
	else:mac:PACKAGE_DATAS	= $${PACKAGE_PREFIX}
	else:PACKAGE_DATAS	= $${PACKAGE_PREFIX}/lib/$${PACKAGE_TARGET}
}

PACKAGE_PREFIX	= $$quote($$PACKAGE_PREFIX)
PACKAGE_DATAS	= $$quote($$PACKAGE_DATAS)

# define package install paths so source code can use them
DEFINES	*= "PACKAGE_PREFIX=\"\\\"$${PACKAGE_PREFIX}\\\"\"" \
	"PACKAGE_DATAS=\"\\\"$${PACKAGE_DATAS}\\\"\""
