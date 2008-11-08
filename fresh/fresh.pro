# fresh lib project file

include( fresh.pri )

# include config file
include( ../config.pri )

TEMPLATE	= lib
CONFIG	*= staticlib
DESTDIR	= $${PACKAGE_BUILD_PATH}
DEPENDPATH	*= $${PWD}/objects $${PWD}/widgets

# make library exportable
DEFINES	*= MONKEY_CORE_BUILD

# Core
# Symbols Export
HEADERS	+= MonkeyExport.h

# objects
# QSingleton
HEADERS	+= QSingleton.h
SOURCES	+= QSingleton.cpp
# psettings
HEADERS	+= pSettings.h
SOURCES	+= pSettings.cpp
# pIconManager
HEADERS	+= pIconManager.h
SOURCES	+= pIconManager.cpp

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
# pExtendedWorkspace
# pFilesList
HEADERS	+= pFilesListWidget.h \
	pExtendedWorkspace.h
SOURCES	+= pFilesListWidget.cpp \
	pExtendedWorkspace.cpp
# pQueuedMessageWidget
HEADERS	+= pQueuedMessageWidget.h
SOURCES	+= pQueuedMessageWidget.cpp
# pQueuedStatusBar
HEADERS	+= pQueuedStatusBar.h
SOURCES	+= pQueuedStatusBar.cpp
