#This is the include file to add to your plugins project

TEMPLATE    = lib
CONFIG  *= dll plugin

# set plugin name define
!isEmpty( TARGET ):DEFINES  *= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""

# include config file
include( $${PWD}/../config.pri )

# include monkey framework
include( $${PACKAGE_PWD}/monkey/monkey.pri )

# include scintilla framework
include( $${PACKAGE_PWD}/qscintilla/qscintilla.pri )

# include fresh framework
include( $${PACKAGE_PWD}/fresh/fresh.pri )

# include qCtagsSense framework
include( $${PACKAGE_PWD}/qCtagsSense/qCtagsSense.pri )

mac:MONKEY_PLUGINS_DIR  = $${PACKAGE_DESTDIR}/$${PACKAGE_TARGET}.app/Contents/plugins
else:unix|win32:MONKEY_PLUGINS_DIR  = $${PACKAGE_DESTDIR}/plugins

mac:*-g++*:LIBS *= -Wl,-undefined,dynamic_lookup

# ubuntu hardy/debian fix
unix:!mac:QMAKE_LFLAGS  -= -Wl,--no-undefined

win32:QMAKE_LIBDIR  *= $${PACKAGE_DESTDIR}

setTemporaryDirectories( $${PACKAGE_BUILD_PATH} )
