# Monkey Studio 2 Ctags library

# include functions file
include( ../functions.pri )

# include config file
include( ../config.pri )

# include shared ctags project file
include( ctags_shared.pri )

TEMPLATE	= lib
CONFIG	*= staticlib
CONFIG	-= qt
DESTDIR	= $${PACKAGE_BUILD_PATH}

CTAGS_SOURCES_PATHS = $$getFolders( ./$${CTAGS_VERSION} )
INCLUDEPATH	*= $${CTAGS_SOURCES_PATHS}
DEPENDPATH	*= $${CTAGS_SOURCES_PATHS}

HEADERS	=  $${CTAGS_VERSION}/debug.h \
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
	$${CTAGS_VERSION}/readtags.h \
	$${CTAGS_VERSION}/sort.h \
	$${CTAGS_VERSION}/args.h \
	$${CTAGS_VERSION}/ctags.h \
	$${CTAGS_VERSION}/exuberantCtags.h

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
	$${CTAGS_VERSION}/tex.c \
	$${CTAGS_VERSION}/flex.c \
	$${CTAGS_VERSION}/vhdl.c \
	$${CTAGS_VERSION}/matlab.c \
	$${CTAGS_VERSION}/ant.c \
	$${CTAGS_VERSION}/ocaml.c \
	$${CTAGS_VERSION}/dosbatch.c \
	$${CTAGS_VERSION}/exuberantCtags.c

win32 {
	HEADERS	*= $${CTAGS_VERSION}/gnu_regex/regex.h \
		$${CTAGS_VERSION}/gnu_regex/regex_internal.h
	
	SOURCES	*= $${CTAGS_VERSION}/gnu_regex/regex.c \
		$${CTAGS_VERSION}/gnu_regex/regcomp.c \
		$${CTAGS_VERSION}/gnu_regex/regexec.c \
		$${CTAGS_VERSION}/gnu_regex/regex_internal.c
}
