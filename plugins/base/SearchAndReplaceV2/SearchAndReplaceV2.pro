###########################################################################################
##		Created using Monkey Studio v1.8.4.0b2 (1.8.4.0b2)
##
##	Author      : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>, Andrei KOPATS aka hlamer <hlamer@tut.by>
##	Project     : SearchAndReplaceV2
##	FileName    : SearchAndReplaceV2.pro
##	Date        : 2009-12-21T22:53:59
##	Description : Search & Replace plugin
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

TARGET	= SearchAndReplaceV2

include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
INCLUDEPATH	*= src
RESOURCES	= src/resources/SearchAndReplaceV2.qrc

HEADERS	= src/SearchAndReplaceV2.h \
	src/SearchThread.h \
	src/ReplaceThread.h \
	src/SearchWidget.h

SOURCES	= src/SearchAndReplaceV2.cpp \
	src/SearchThread.cpp \
	src/ReplaceThread.cpp \
	src/SearchWidget.cpp
FORMS	+= src/SearchWidget.ui