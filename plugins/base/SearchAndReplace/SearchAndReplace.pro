###########################################################################################
##		Created using Monkey Studio v1.8.4.0b2 (1.8.4.0b2)
##
##	Author      : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>, Andrei KOPATS aka hlamer <hlamer@tut.by>
##	Project     : SearchAndReplace
##	FileName    : SearchAndReplace.pro
##	Date        : 2009-12-21T22:53:59
##	Description : Search & Replace plugin
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

TARGET	= SearchAndReplace

include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
INCLUDEPATH	*= src
RESOURCES	= src/resources/SearchAndReplace.qrc

FORMS	= src/SearchWidget.ui \
	src/SearchAndReplaceSettings.ui

HEADERS	= src/SearchAndReplace.h \
	src/SearchThread.h \
	src/ReplaceThread.h \
	src/SearchWidget.h \
	src/SearchResultsDock.h \
	src/SearchResultsModel.h \
	src/SearchAndReplaceSettings.h

SOURCES	= src/SearchAndReplace.cpp \
	src/SearchThread.cpp \
	src/ReplaceThread.cpp \
	src/SearchWidget.cpp \
	src/SearchResultsDock.cpp \
	src/SearchResultsModel.cpp \
	src/SearchAndReplaceSettings.cpp