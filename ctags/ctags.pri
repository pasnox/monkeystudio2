# Monkey Studio 2 Ctags project include file

# ctags version
CTAGS_VERSION	= ctags

# include path
INCLUDEPATH	*= $$PWD/sdk

# For correctly include ctags headers
unix:DEFINES *=HAVE_FGETPOS
