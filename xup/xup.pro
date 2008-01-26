###########################################################################################
##		Created using Monkey Studio v1.8.0.0
##
##	Author    :
##	Project   : xup
##	FileName  : xup.pro
##	Date      : 2007-12-11T01:10:27
##	License   : GPL
##	Comment   : Creating using Monkey Studio RAD
##	Home Page   :
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

TEMPLATE	= app
LANGUAGE	= Qt4/C++
BUILD_PATH	= ./build
TARGET	= xup
CONFIG	+= debug_and_release
QT	+= xml
RESOURCES	+= src/resources/resources.qrc src/qmakemanager/resources/qmakemanager.qrc
INCLUDEPATH	= . src src/xupmanager src/xupmanager/ui src/qmakemanager

BUILDER	= GNUMake
COMPILER	= G++
EXECUTE_RELEASE	= xup.app/Contents/MacOS/xup
EXECUTE_DEBUG	= xup_debug.app/Contents/MacOS/xup_debug

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
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
}

FORMS	+= src/xupmanager/ui/UIProjectsManager.ui

HEADERS	+= src/xupmanager/ProjectItemModel.h \
	src/xupmanager/ProjectItem.h \
	src/xupmanager/XUPManager.h \
	src/xupmanager/ui/UIProjectsManager.h \
	src/qmakemanager/QMakeProjectItem.h \
	src/qmakemanager/QMake2XUP.h \
	src/xupmanager/FilteredProjectItemModel.h

SOURCES	+= src/xupmanager/ProjectItemModel.cpp \
	src/xupmanager/ProjectItem.cpp \
	src/xupmanager/XUPManager.cpp \
	src/xupmanager/ui/UIProjectsManager.cpp \
	src/main.cpp \
	src/qmakemanager/QMakeProjectItem.cpp \
	src/qmakemanager/QMake2XUP.cpp \
	src/xupmanager/FilteredProjectItemModel.cpp
