# Monkey Studio 2 project file

BUILD_PATH	 = ../build

# include qscintilla framework
include( ../qscintilla/qscintilla.pro )

unix:OBJECTS_DIR	= $${BUILD_PATH}/.o/unix
win32:OBJECTS_DIR	= $${BUILD_PATH}/.o/win32
mac:OBJECTS_DIR	= $${BUILD_PATH}/.o/mac
UI_DIR	= $${BUILD_PATH}/.ui
MOC_DIR	= $${BUILD_PATH}/.moc
RCC_DIR	= $${BUILD_PATH}/.rcc

unix:ICON	= src/resources/icons/application/monkey2.png
mac:ICON	= src/resources/icons/application/monkey2.icns
win32:ICON	= src/resources/icons/application/monkey2.ico

TEMPLATE	= app
LANGUAGE	= C++
DESTDIR	= ../bin
TARGET	= monkey

COPYRIGHTS	= "(c) 2005 - 2007 Azevedo Filipe"
DOMAIN	= "www.monkeystudio.org"
VERSION	= "1.7.0"
PROGRAM_NAME	= "Monkey Studio"

win32:RC_FILE	*= monkey.rc
RESOURCES	*= src/resources/resources.qrc

CONFIG	*= qt warn_on app_bundle thread x11 windows
!contains( CONFIG, release ) {
	CONFIG	*= console
	VERSION = ${VERSION}svn
}
QT	*= gui core

DEFINES	*= MONKEY_CORE_BUILD "PROGRAM_NAME=\"\\\"$${PROGRAM_NAME}\\\"\"" "PROGRAM_VERSION=\"\\\"$${VERSION}\\\"\"" "PROGRAM_DOMAIN=\"\\\"$${DOMAIN}\\\"\"" "PROGRAM_COPYRIGHTS=\"\\\"$${COPYRIGHTS}\\\"\""

LIBS	*= -L$${BUILD_PATH}
linux-g++:LIBS	*= -rdynamic
win32-msvc:LIBS	*= /IMPLIB:$${BUILD_PATH}/monkey.lib
win32-g++:LIBS	*= -Wl,--out-implib,$${BUILD_PATH}/libmonkey.a

INCLUDEPATH	*= . src src/maininterface src/maininterface/ui src/workspace src/recentsmanager src/qscintillamanager src/qscintillamanager/ui src/templatesmanager src/templatesmanager/ui src/abbreviationsmanager src/abbreviationsmanager/ui src/toolsmanager src/toolsmanager/ui src/consolemanager src/consolemanager/ui

# include fresh framework
include( ../fresh/fresh.pro )

FORMS	*= src/maininterface/ui/UITranslator.ui \
	src/maininterface/ui/UIAbout.ui \
	src/maininterface/ui/UISettings.ui \
	src/qscintillamanager/ui/pSearch.ui \
	src/abbreviationsmanager/ui/UIAddAbbreviation.ui \
	src/templatesmanager/ui/UIEditTemplate.ui \
	src/templatesmanager/ui/UITemplatesWizard.ui \
	src/toolsmanager/ui/UIDesktopTools.ui \
	src/toolsmanager/ui/UIToolsEdit.ui

HEADERS	*= src/maininterface/ui/UITranslator.h \
	src/maininterface/ui/UIAbout.h \
	src/maininterface/ui/UISettings.h \
	src/recentsmanager/pRecentsManager.h \
	src/workspace/pAbstractChild.h \
	src/qscintillamanager/ui/pSearch.h \
	src/qscintillamanager/pEditor.h \
	src/workspace/pChild.h \
	src/workspace/UISaveFiles.h \
	src/workspace/pFileManager.h \
	src/workspace/pWorkspace.h \
	src/maininterface/UIMain.h \
	src/abbreviationsmanager/pAbbreviationsManager.h \
	src/abbreviationsmanager/ui/UIAddAbbreviation.h \
	src/templatesmanager/pTemplatesManager.h \
	src/templatesmanager/pTemplatePreviewer.h \
	src/templatesmanager/ui/UIEditTemplate.h \
	src/templatesmanager/ui/UITemplatesWizard.h \
	src/pMonkeyStudio.h \
	src/toolsmanager/pDesktopApplications.h \
	src/toolsmanager/ui/UIDesktopTools.h \
	src/toolsmanager/ui/UIToolsEdit.h \
	src/consolemanager/ConsoleManager.h \
	src/toolsmanager/pToolsManager.h

SOURCES	*= src/maininterface/ui/UITranslator.cpp \
	src/maininterface/ui/UIAbout.cpp \
	src/maininterface/ui/UISettings.cpp \
	src/recentsmanager/pRecentsManager.cpp \
	src/qscintillamanager/ui/pSearch.cpp \
	src/qscintillamanager/pEditor.cpp \
	src/workspace/pChild.cpp \
	src/workspace/UISaveFiles.cpp \
	src/workspace/pFileManager.cpp \
	src/workspace/pWorkspace.cpp \
	src/maininterface/UIMain.cpp \
	src/abbreviationsmanager/pAbbreviationsManager.cpp \
	src/abbreviationsmanager/ui/UIAddAbbreviation.cpp \
	src/templatesmanager/pTemplatesManager.cpp \
	src/templatesmanager/pTemplatePreviewer.cpp \
	src/templatesmanager/ui/UIEditTemplate.cpp \
	src/templatesmanager/ui/UITemplatesWizard.cpp \
	src/pMonkeyStudio.cpp \
	src/toolsmanager/pDesktopApplications.cpp \
	src/toolsmanager/ui/UIDesktopTools.cpp \
	src/toolsmanager/ui/UIToolsEdit.cpp \
	src/toolsmanager/pToolsManager.cpp \
	src/consolemanager/ConsoleManager.cpp \
	src/main.cpp

unix:SOURCES	*= src/toolsmanager/pDesktopApplications_unix.cpp
mac:SOURCES	*= src/toolsmanager/pDesktopApplications_mac.cpp
win32:SOURCES	*= src/toolsmanager/pDesktopApplications_win32.cpp

TRANSLATIONS	= ../translations/monkey_french.ts
