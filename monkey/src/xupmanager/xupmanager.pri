# xup framework

XUP_PWD	= $$PWD

QT	*= xml
RESOURCES	*= $${XUP_PWD}/resources/XUPManager.qrc

FORMS	*= $${XUP_PWD}/ui/UIAddVariable.ui \
	$${XUP_PWD}/ui/UIXUPProjectEditor.ui \
	$${XUP_PWD}/ui/UIXUPManager.ui

HEADERS	*= $${XUP_PWD}/XUPIO.h \
	$${XUP_PWD}/XUPItem.h \
	$${XUP_PWD}/XUPProjectItem.h \
	$${XUP_PWD}/ProjectItemModel.h \
	$${XUP_PWD}/ScopedProjectItemModel.h \
	$${XUP_PWD}/FilteredProjectItemModel.h \
	$${XUP_PWD}/ProjectEditorModel.h \
	$${XUP_PWD}/AddFilesDialog.h \
	$${XUP_PWD}/ui/UIAddVariable.h \
#	$${XUP_PWD}/ui/UIXUPProjectEditor.h \
	$${XUP_PWD}/ui/UIXUPManager.h

SOURCES	*= $${XUP_PWD}/XUPIO.cpp \
	$${XUP_PWD}/XUPItem.cpp \
	$${XUP_PWD}/XUPProjectItem.cpp \
	$${XUP_PWD}/ProjectItemModel.cpp \
	$${XUP_PWD}/ScopedProjectItemModel.cpp \
	$${XUP_PWD}/FilteredProjectItemModel.cpp \
	$${XUP_PWD}/ProjectEditorModel.cpp \
	$${XUP_PWD}/AddFilesDialog.cpp \
	$${XUP_PWD}/ui/UIAddVariable.cpp \
#	$${XUP_PWD}/ui/UIXUPProjectEditor.cpp \
	$${XUP_PWD}/ui/UIXUPManager.cpp
