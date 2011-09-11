###########################################################################################
##		Created using Monkey Studio v1.8.4.0b2 (1.8.4.0b2)
##
##	Author      : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
##	Project     : Tools
##	FileName    : Tools.pro
##	Date        : 2010-02-17T21:55:43
##	Description : A plugin that allow you to define some external tools in the menu bar
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

TARGET	= Tools

include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
INCLUDEPATH	*= src
RESOURCES	= src/resources/Tools.qrc

FORMS	= src/ui/UIDesktopTools.ui \
	src/ui/UIToolsEdit.ui

HEADERS	= src/Tools.h \
	src/ToolsManager.h \
	src/DesktopApplications.h \
	src/ui/UIDesktopTools.h \
	src/ui/UIToolsEdit.h

SOURCES	= src/Tools.cpp \
	src/ToolsManager.cpp \
	src/DesktopApplications.cpp \
	src/ui/UIDesktopTools.cpp \
	src/ui/UIToolsEdit.cpp

mac:SOURCES	+= src/DesktopApplications_mac.cpp
else:unix:SOURCES	+= src/DesktopApplications_unix.cpp
win32:SOURCES	+= src/DesktopApplications_win.cpp