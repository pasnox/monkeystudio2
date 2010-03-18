# Monkey Studio 2 qCtagsSense library

include( ../ctags/ctags.pri )

# include config file
include( ../config.pri )

TEMPLATE	= lib
CONFIG	*= staticlib
DESTDIR	= $${PACKAGE_BUILD_PATH}