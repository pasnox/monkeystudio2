# fresh lib project file

include( fresh.pri )

HEADER	+= MonkeyExport.h

# objects
# QSingleton
HEADERS	+= QSingleton.h
# psettings
HEADERS	+= pSettings.h
SOURCES	+= pSettings.cpp

# widgets
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
#	pActionGroup.h \
	pActionManager.h \
	pKeySequenceInput.h \
	pShortcutsEditor.h
SOURCES	+= pAction.cpp \
#	pActionGroup.cpp \
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
