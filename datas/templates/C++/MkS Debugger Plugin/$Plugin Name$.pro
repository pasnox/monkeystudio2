###########################################################################################
##      Created using $editor_version_string$
##
##  Author      : $Authors$
##  Project     : $Plugin Name$
##  FileName    : $Plugin Name$.pro
##  Date        : $date$
##  Description : $Description$
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

TARGET  = $Plugin Name$

include( ../../plugins.pri )
DESTDIR = $$MONKEY_PLUGINS_DIR/debugger
INCLUDEPATH *= src
RESOURCES   *= $Resources Path$$Plugin Name$.qrc

HEADERS *= $Sources Path$$Plugin Name$.h

SOURCES *= $Sources Path$$Plugin Name$.cpp