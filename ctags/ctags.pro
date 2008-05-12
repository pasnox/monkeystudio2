# Monkey Studio 2 Ctags library

include( ctags.pri )

# include config file
include( ../config.pri )

TEMPLATE	= lib
CONFIG	*= staticlib
CONFIG	-= qt
DESTDIR	= $${PACKAGE_BUILD_PATH}
win32-msvc*:DEFINES	*= WIN32 HAVE_REGCOMP REGEX_MALLOC STDC_HEADERS=1
unix:DEFINES	*= HAVE_STDLIB_H HAVE_FGETPOS HAVE_SYS_STAT_H HAVE_FCNTL_H

HEADERS	=  \
	#	$${CTAGS_VERSION}/debug.h \
	$${CTAGS_VERSION}/entry.h \
	$${CTAGS_VERSION}/general.h \
	$${CTAGS_VERSION}/get.h \
	$${CTAGS_VERSION}/keyword.h \
	$${CTAGS_VERSION}/options.h \
	$${CTAGS_VERSION}/parse.h \
	$${CTAGS_VERSION}/read.h \
	$${CTAGS_VERSION}/routines.h \
	$${CTAGS_VERSION}/strlist.h \
	$${CTAGS_VERSION}/vstring.h \
	#	$${CTAGS_VERSION}/readtags.h \
	#	$${CTAGS_VERSION}/sort.h \
	#	$${CTAGS_VERSION}/args.h \
	#	$${CTAGS_VERSION}/ctags.h \
	$${CTAGS_VERSION}/exuberantCtags.h

win32-msvc*:HEADERS	*= $${CTAGS_VERSION}/regex.h

SOURCES	= $${CTAGS_VERSION}/asm.c \
	$${CTAGS_VERSION}/asp.c \
	$${CTAGS_VERSION}/awk.c \
	$${CTAGS_VERSION}/basic.c \
	$${CTAGS_VERSION}/beta.c \
	$${CTAGS_VERSION}/c.c \
	$${CTAGS_VERSION}/cobol.c \
	$${CTAGS_VERSION}/eiffel.c \
	#	$${CTAGS_VERSION}/debug.c \
	$${CTAGS_VERSION}/entry.c \
	$${CTAGS_VERSION}/erlang.c \
	$${CTAGS_VERSION}/fortran.c \
	$${CTAGS_VERSION}/get.c \
	$${CTAGS_VERSION}/html.c \
	$${CTAGS_VERSION}/jscript.c \
	$${CTAGS_VERSION}/keyword.c \
	$${CTAGS_VERSION}/lisp.c \
	$${CTAGS_VERSION}/lregex.c \
	$${CTAGS_VERSION}/lua.c \
	$${CTAGS_VERSION}/make.c \
	$${CTAGS_VERSION}/options.c \
	$${CTAGS_VERSION}/parse.c \
	$${CTAGS_VERSION}/pascal.c \
	$${CTAGS_VERSION}/perl.c \
	$${CTAGS_VERSION}/php.c \
	$${CTAGS_VERSION}/python.c \
	$${CTAGS_VERSION}/read.c \
	$${CTAGS_VERSION}/rexx.c \
	$${CTAGS_VERSION}/routines.c \
	$${CTAGS_VERSION}/ruby.c \
	$${CTAGS_VERSION}/scheme.c \
	$${CTAGS_VERSION}/sh.c \
	$${CTAGS_VERSION}/slang.c \
	$${CTAGS_VERSION}/sml.c \
	$${CTAGS_VERSION}/sql.c \
	$${CTAGS_VERSION}/strlist.c \
	$${CTAGS_VERSION}/tcl.c \
	$${CTAGS_VERSION}/verilog.c \
	$${CTAGS_VERSION}/vim.c \
	$${CTAGS_VERSION}/vstring.c \
	$${CTAGS_VERSION}/yacc.c \
	#	$${CTAGS_VERSION}/readtags.c \
	#	$${CTAGS_VERSION}/sort.c \
	#	$${CTAGS_VERSION}/args.c \
	#	$${CTAGS_VERSION}/argproc.c \
	#	$${CTAGS_VERSION}/mac.c \
	#	$${CTAGS_VERSION}/qdos.c \
	#	$${CTAGS_VERSION}/e_amiga.h \
	#	$${CTAGS_VERSION}/e_djgpp.h \
	#	$${CTAGS_VERSION}/e_mac.h \
	#	$${CTAGS_VERSION}/e_msoft.h \
	#	$${CTAGS_VERSION}/e_os2.h \
	#	$${CTAGS_VERSION}/e_qdos.h \
	#	$${CTAGS_VERSION}/e_riscos.h \
	#	$${CTAGS_VERSION}/e_vms.h \
	#	$${CTAGS_VERSION}/main.c \
	$${CTAGS_VERSION}/exuberantCtags.c

win32-msvc*:SOURCES	*= $${CTAGS_VERSION}/regex.c
