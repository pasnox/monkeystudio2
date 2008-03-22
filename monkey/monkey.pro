# Monkey Studio 2 project file

BUILD_PATH	= ../build

# include qscintilla framework
include( ../qscintilla/qscintilla.pri )

# include fresh framework
include( ../fresh/fresh.pri )

# include ctags framework
include( ../ctags/ctags.pri )

#include monkey framework
include( monkey.pri )

TEMPLATE	= app
LANGUAGE	= Qt4/C++
TARGET	= monkeystudio
DESTDIR	= ../bin
CONFIG	*= qt warn_on thread x11 windows debug_and_release
QT	*= gui core

QMAKE_TARGET_COMPANY = "Monkey Studio Team"
QMAKE_TARGET_PRODUCT = "Monkey Studio"
QMAKE_TARGET_DESCRIPTION = "Crossplatform Integrated Development Environment."
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2005 - 2008 Filipe AZEVEDO & The Monkey Studio Team"

unix:ICON	= src/resources/icons/application/monkey2.png
mac:ICON	= src/resources/icons/application/monkey2.icns
win32:ICON	= src/resources/icons/application/monkey2.ico

COPYRIGHTS	= "(c) 2005 - 2008 Azevedo Filipe"
DOMAIN	= "www.monkeystudio.org"
VERSION	= 1.8.2.0svn
NAME	= "Monkey Studio"

win32:RC_FILE	*= monkey.rc
RESOURCES	*= src/resources/resources.qrc

DEFINES	*= MONKEY_CORE_BUILD "PROGRAM_NAME=\"\\\"$${NAME}\\\"\"" "PROGRAM_VERSION=\"\\\"$${VERSION}\\\"\"" "PROGRAM_DOMAIN=\"\\\"$${DOMAIN}\\\"\"" "PROGRAM_COPYRIGHTS=\"\\\"$${COPYRIGHTS}\\\"\""

LIBS	*= -L$${BUILD_PATH}
*-g++:LIBS	*= -Wl,--whole-archive # import all symbols as the not used ones too
mac:*-g++:LIBS	*= -dynamic
else:unix:*-g++:LIBS	*= -rdynamic

PRE_TARGETDEPS	*= ../qscintilla ../fresh ../ctags

CONFIG( debug, debug|release ) {
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
	unix:LIBS	*= -lqscintilla2_debug -lfresh_debug -lctags_debug
	else:LIBS	*= -lqscintilla2d -lfreshd -lctagsd
	win32-g++:LIBS	*= -Wl,--out-implib,$${BUILD_PATH}/lib$${TARGET}.a
	win32-msvc*:LIBS	*= /IMPLIB:$${BUILD_PATH}/$${TARGET}.lib -lshell32
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
	LIBS	*= -lqscintilla2 -lctags -lfresh
	win32-g++:LIBS	*= -Wl,--out-implib,$${BUILD_PATH}/lib$${TARGET}.a
	win32-msvc*:LIBS	*= /IMPLIB:$${BUILD_PATH}/$${TARGET}.lib -lshell32
}

*-g++:LIBS	*= -Wl,--no-whole-archive # stop importing all symbols

# include install script
include( installs.pri )

DEFINES	*= "PROGRAM_PREFIX=\"\\\"$${PROGRAM_PREFIX}\\\"\"" "PROGRAM_DATAS=\"\\\"$${PROGRAM_DATAS}\\\"\""

FORMS	*= src/maininterface/ui/UITranslator.ui \
	src/maininterface/ui/UIAbout.ui \
	src/maininterface/ui/UISettings.ui \
	src/abbreviationsmanager/ui/UIAddAbbreviation.ui \
	src/templatesmanager/ui/UITemplatesWizard.ui \
	src/toolsmanager/ui/UIDesktopTools.ui \
	src/toolsmanager/ui/UIToolsEdit.ui \
	src/pluginsmanager/ui/UIPluginsSettings.ui \
	src/pluginsmanager/ui/UICLIToolSettings.ui \
	src/pluginsmanager/ui/UIBuilderSettings.ui \
	src/projectsmanager/ui/UIProjectsManager.ui \
	src/pluginsmanager/ui/UICompilerSettings.ui

HEADERS	*= src/maininterface/ui/UITranslator.h \
	src/maininterface/ui/UIAbout.h \
	src/maininterface/ui/UISettings.h \
	src/recentsmanager/pRecentsManager.h \
	src/workspace/pAbstractChild.h \
	src/qscintillamanager/ui/pSearch.h \
	src/qscintillamanager/SearchThread.h \
	src/qscintillamanager/pEditor.h \
	src/qscintillamanager/qSciShortcutsManager.h \
	src/ctagsmanager/Ctags.h \
	src/workspace/pChild.h \
	src/workspace/UISaveFiles.h \
	src/workspace/pFileManager.h \
	src/workspace/pWorkspace.h \
	src/maininterface/UIMain.h \
	src/abbreviationsmanager/pAbbreviationsManager.h \
	src/abbreviationsmanager/ui/UIAddAbbreviation.h \
	src/variablesmanager/VariablesManager.h \
	src/templatesmanager/pTemplatesManager.h \
	src/templatesmanager/ui/UITemplatesWizard.h \
	src/pMonkeyStudio.h \
	src/toolsmanager/pDesktopApplications.h \
	src/toolsmanager/ui/UIDesktopTools.h \
	src/toolsmanager/ui/UIToolsEdit.h \
	src/consolemanager/pConsoleManager.h \
	src/consolemanager/pCommandParser.h \
	src/consolemanager/pCommand.h \
	src/toolsmanager/pToolsManager.h \
	src/pluginsmanager/BasePlugin.h \
	src/pluginsmanager/ChildPlugin.h \
	src/pluginsmanager/CLIToolPlugin.h \
	src/pluginsmanager/ProjectPlugin.h \
	src/pluginsmanager/BuilderPlugin.h \
	src/pluginsmanager/CompilerPlugin.h \
	src/pluginsmanager/PluginsManager.h \
	src/pluginsmanager/ui/UIPluginsSettings.h \
	src/pluginsmanager/ui/UICLIToolSettings.h \
	src/pluginsmanager/ui/UIBuilderSettings.h \
	src/projectsmanager/ui/UIProjectsManager.h \
	src/projectsmanager/ui/UIAddExistingFiles.h \
	src/projectsmanager/ProjectsModel.h \
	src/projectsmanager/ProjectsProxy.h \
	src/projectsmanager/ProjectItem.h \
	src/pluginsmanager/ui/UICompilerSettings.h \
	src/settingsmanager/Settings.h \
	src/coremanager/MonkeyCore.h

SOURCES	*= src/maininterface/ui/UITranslator.cpp \
	src/maininterface/ui/UIAbout.cpp \
	src/maininterface/ui/UISettings.cpp \
	src/recentsmanager/pRecentsManager.cpp \
	src/qscintillamanager/ui/pSearch.cpp \
	src/qscintillamanager/SearchThread.cpp \
	src/qscintillamanager/pEditor.cpp \
	src/qscintillamanager/qSciShortcutsManager.cpp \
	src/ctagsmanager/Ctags.cpp \
	src/workspace/pChild.cpp \
	src/workspace/UISaveFiles.cpp \
	src/workspace/pFileManager.cpp \
	src/workspace/pWorkspace.cpp \
	src/maininterface/UIMain.cpp \
	src/abbreviationsmanager/pAbbreviationsManager.cpp \
	src/abbreviationsmanager/ui/UIAddAbbreviation.cpp \
	src/variablesmanager/VariablesManager.cpp \
	src/templatesmanager/pTemplatesManager.cpp \
	src/templatesmanager/ui/UITemplatesWizard.cpp \
	src/pMonkeyStudio.cpp \
	src/toolsmanager/pDesktopApplications.cpp \
	src/toolsmanager/ui/UIDesktopTools.cpp \
	src/toolsmanager/ui/UIToolsEdit.cpp \
	src/toolsmanager/pToolsManager.cpp \
	src/consolemanager/pConsoleManager.cpp \
	src/consolemanager/pCommandParser.cpp \
	src/pluginsmanager/PluginsManager.cpp \
	src/pluginsmanager/ui/UIPluginsSettings.cpp \
	src/pluginsmanager/ui/UICLIToolSettings.cpp \
	src/pluginsmanager/ui/UIBuilderSettings.cpp \
	src/projectsmanager/ui/UIProjectsManager.cpp \
	src/projectsmanager/ui/UIAddExistingFiles.cpp \
	src/projectsmanager/ProjectsModel.cpp \
	src/projectsmanager/ProjectsProxy.cpp \
	src/projectsmanager/ProjectItem.cpp \
	src/main.cpp \
	src/pluginsmanager/ui/UICompilerSettings.cpp \
	src/settingsmanager/Settings.cpp \
	src/coremanager/MonkeyCore.cpp

mac:SOURCES	*= src/toolsmanager/pDesktopApplications_mac.cpp
else:unix:SOURCES	*= src/toolsmanager/pDesktopApplications_unix.cpp
win32:SOURCES	*= src/toolsmanager/pDesktopApplications_win32.cpp

TRANSLATIONS	*= ../translations/monkey_french.ts \
	../translations/monkey_belarusian.ts
