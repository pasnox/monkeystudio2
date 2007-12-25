# fresh lib project file

include( fresh.pri )

BUILD_PATH	 = ../build

TEMPLATE = lib
CONFIG	*= qt staticlib warn_on thread x11 windows debug_and_release
DESTDIR	= $${BUILD_PATH}

# make library exportable
DEFINES	*= MONKEY_CORE_BUILD

CONFIG(DebugBuild)|CONFIG(debug, debug|release) {
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

# Core
# Symbols Export
HEADER	+= MonkeyExport.h

# objects
# QSingleton
HEADERS	+= QSingleton.h
SOURCES	+= QSingleton.cpp
# psettings
HEADERS	+= pSettings.h
SOURCES	+= pSettings.cpp

# widgets
# pTabBar
HEADERS	+= pTabBar.h
SOURCES	+= pTabBar.cpp
# pTabbedWorkspace
HEADERS	+= pTabbedWorkspaceCorner.h \
	pTabbedWorkspaceCornerButton.h \
	pTabbedWorkspace.h \
	pTabbedWorkspaceRightCorner.h
SOURCES	+= pTabbedWorkspaceCorner.cpp \
	pTabbedWorkspaceCornerButton.cpp \
	pTabbedWorkspace.cpp \
	pTabbedWorkspaceRightCorner.cpp
# action manager
HEADERS	+= pAction.h \
	pActionManager.h \
	pKeySequenceInput.h \
	pShortcutsEditor.h
SOURCES	+= pAction.cpp \
	pActionManager.cpp \
	pKeySequenceInput.cpp \
	pShortcutsEditor.cpp
# pmenubar
HEADERS	+= pMenuBar.h
SOURCES	+= pMenuBar.cpp
# docktoolbar
HEADERS	+= pDockToolBar.h \
	pDockToolBarManager.h
SOURCES	+= pDockToolBar.cpp \
	pDockToolBarManager.cpp
# pMainWindow
HEADERS	+= pMainWindow.h
SOURCES	+= pMainWindow.cpp
# pTabbedMainWindow
HEADERS	+= pTabbedMainWindow.h
SOURCES	+= pTabbedMainWindow.cpp
# pTreeComboBox
HEADERS	+= pTreeComboBox.h
SOURCES	+= pTreeComboBox.cpp
# pStringListEditor
HEADERS	+= pStringListEditor.h
SOURCES	+= pStringListEditor.cpp
# pFileListEditor
HEADERS	+= pFileListEditor.h
SOURCES	+= pFileListEditor.cpp
# pPathListEditor
HEADERS	+= pPathListEditor.h
SOURCES	+= pPathListEditor.cpp
# pDockWidget
HEADERS	+= pDockWidget.h
SOURCES	+= pDockWidget.cpp
