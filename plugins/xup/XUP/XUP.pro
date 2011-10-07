###########################################################################################
##      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
##
##  Author      : You Name <your@email.org>
##  Project     : XUP
##  FileName    : XUP.pro
##  Date        : 2011-09-14T23:35:58
##  Description : Generic XUP Plugin
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

TARGET  = XUP

include( ../../plugins.pri )
DESTDIR = $$MONKEY_PLUGINS_DIR/xup
INCLUDEPATH *= src
RESOURCES   = src/resources/XUP.qrc

HEADERS = src/XUP.h

SOURCES = src/XUP.cpp