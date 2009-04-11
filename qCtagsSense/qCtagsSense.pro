###########################################################################################
##		Created using Monkey Studio v1.8.3.2
##
##	Author    : You Name <your@email.org>
##	Project   : qintellisense
##	FileName  : qintellisense.pro
##	Date      : 2009-03-27T22:20:17
##	License   : GPL
##	Comment   : Creating using Monkey Studio RAD
##	Home Page   : http://www.mydomain.org
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

XUPProjectSettings {
	EDITOR	= QMake
	QT_VERSION	= Qt System (4.4.3)
}

TEMPLATE	= app
LANGUAGE	= C++/Qt4
TARGET	= $$quote(qCtagsSense)
CONFIG	+= qt resources debug_and_release warn_on thread x11 windows embed_manifest_exe x86 ppc app_bundle
QT	= core gui sql
BUILD_PATH	= ./build

LIBS	= -L../libs

PRE_TARGETDEPS	+= ../ctags

CONFIG(debug, debug|release) {
	#Debug
	CONFIG	+= console
	unix:TARGET	= $$join(TARGET,,,_debug)
	else:TARGET	= $$join(TARGET,,,d)
	unix:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/mac
	UI_DIR	= $${BUILD_PATH}/debug/.ui
	MOC_DIR	= $${BUILD_PATH}/debug/.moc
	RCC_DIR	= $${BUILD_PATH}/debug/.rcc
	unix:LIBS	*= -lctags_debug
	else:LIBS	*= -lctagsd
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
	LIBS	*= -lctags
}

include( ../ctags/ctags.pri )

RESOURCES	= src/resources/qCtagsSense.qrc

FORMS	= src/qCtagsSenseBrowser.ui

HEADERS	= src/qCtagsSense.h \
	src/qCtagsSenseIndexer.h \
	src/qCtagsSenseBrowser.h \
	src/qCtagsSenseMembersModel.h \
	src/qCtagsSenseFilesModel.h \
	src/qCtagsSenseSQL.h \
	src/qCtagsSenseLanguagesModel.h \
	src/FileManager.h \
	src/QTimeTracker.h \
	src/qCtagsSenseUtils.h \
	src/qCtagsSenseKindFinder.h

SOURCES	= src/main.cpp \
	src/qCtagsSense.cpp \
	src/qCtagsSenseIndexer.cpp \
	src/qCtagsSenseBrowser.cpp \
	src/qCtagsSenseMembersModel.cpp \
	src/qCtagsSenseFilesModel.cpp \
	src/qCtagsSenseSQL.cpp \
	src/qCtagsSenseLanguagesModel.cpp \
	src/FileManager.cpp \
	src/QTimeTracker.cpp \
	src/qCtagsSenseUtils.cpp \
	src/qCtagsSenseKindFinder.cpp
