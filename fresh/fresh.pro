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

HEADERS	= objects/GetOpt.h \
	objects/MonkeyExport.h \
	objects/pIconManager.h \
	objects/pSettings.h \
	objects/QSingleton.h \
	widgets/pActionsManager.h \
	widgets/pActionsShortcutsManager.h \
	widgets/pColorButton.h \
	widgets/pConsole.h \
	widgets/pConsoleCommand.h \
	widgets/pDockToolBar.h \
	widgets/pDockToolBarManager.h \
	widgets/pDockWidget.h \
	widgets/pDockWidgetTitleBar.h \
	widgets/pExtendedWorkspace.h \
	widgets/pFileDialog.h \
	widgets/pFileListEditor.h \
	widgets/pFilesListWidget.h \
	widgets/pGroupPath.h \
	widgets/pKeySequenceInput.h \
	widgets/pMainWindow.h \
	widgets/pMenuBar.h \
	widgets/pMultiToolBar.h \
	widgets/pPathListEditor.h \
	widgets/pQueuedMessageWidget.h \
	widgets/pQueuedStatusBar.h \
	widgets/pStringListEditor.h \
	widgets/pTabBar.h \
	widgets/pTabbedMainWindow.h \
	widgets/pTabbedWorkspace.h \
	widgets/pTabbedWorkspaceCorner.h \
	widgets/pTabbedWorkspaceCornerButton.h \
	widgets/pTabbedWorkspaceRightCorner.h \
	widgets/pTreeComboBox.h

SOURCES	= objects/GetOpt.cpp \
	objects/pIconManager.cpp \
	objects/pSettings.cpp \
	objects/QSingleton.cpp \
	widgets/pActionsManager.cpp \
	widgets/pActionsShortcutsManager.cpp \
	widgets/pColorButton.cpp \
	widgets/pConsole.cpp \
	widgets/pConsoleCommand.cpp \
	widgets/pDockToolBar.cpp \
	widgets/pDockToolBarManager.cpp \
	widgets/pDockWidget.cpp \
	widgets/pDockWidgetTitleBar.cpp \
	widgets/pExtendedWorkspace.cpp \
	widgets/pFileDialog.cpp \
	widgets/pFileListEditor.cpp \
	widgets/pFilesListWidget.cpp \
	widgets/pGroupPath.cpp \
	widgets/pKeySequenceInput.cpp \
	widgets/pMainWindow.cpp \
	widgets/pMenuBar.cpp \
	widgets/pMultiToolBar.cpp \
	widgets/pPathListEditor.cpp \
	widgets/pQueuedMessageWidget.cpp \
	widgets/pQueuedStatusBar.cpp \
	widgets/pStringListEditor.cpp \
	widgets/pTabBar.cpp \
	widgets/pTabbedMainWindow.cpp \
	widgets/pTabbedWorkspace.cpp \
	widgets/pTabbedWorkspaceCorner.cpp \
	widgets/pTabbedWorkspaceCornerButton.cpp \
	widgets/pTabbedWorkspaceRightCorner.cpp \
	widgets/pTreeComboBox.cpp