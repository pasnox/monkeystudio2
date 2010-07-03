# Monkey Studio 2 Ctags project include file

# include shared ctags project file
include( ctags_shared.pri )

# include path
INCLUDEPATH	*= $$getFolders( $${PWD}/$${CTAGS_VERSION} )
