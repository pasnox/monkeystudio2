# Monkey Studio 2 Ctags project include file

# ctags version
CTAGS_VERSION   = ctags

# include path
INCLUDEPATH *= $$PWD/sdk

# Defines for platforms
DEFINES *= HAVE_REGCOMP

win32:DEFINES   *= WIN32 \
    REGEX_MALLOC \
    STDC_HEADERS=1 \
    __USE_GNU \
    HAVE_STDBOOL_H

unix:DEFINES    *= HAVE_STDLIB_H \
    HAVE_FGETPOS \
    HAVE_SYS_STAT_H \
    HAVE_FCNTL_H \
    HAVE_REGEX \
    HAVE_UNISTD_H \
    HAVE_STRSTR
