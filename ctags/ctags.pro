# Monkey Studio 2 Ctags library

include( ctags.pri.pri )

BUILD_PATH	 = ../build

TEMPLATE = lib
CONFIG	*= staticlib warn_on debug_and_release
DESTDIR	= $${BUILD_PATH}

CONFIG(DebugBuild)|CONFIG(debug, debug|release) {
	#Debug
	CONFIG	+= console
	unix:TARGET	= $$join(TARGET,,,_debug)
	else:TARGET	= $$join(TARGET,,,d)
	unix:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/mac
	
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
}

# Input
HEADERS = $${CTAGS_VERSION}/exuberantCtags.h \
           $${CTAGS_VERSION}/debug.h \
           $${CTAGS_VERSION}/entry.h \
           $${CTAGS_VERSION}/general.h \
           $${CTAGS_VERSION}/get.h \
           $${CTAGS_VERSION}/keyword.h \
           $${CTAGS_VERSION}/options.h \
           $${CTAGS_VERSION}/parse.h \
           $${CTAGS_VERSION}/read.h \
           $${CTAGS_VERSION}/routines.h \
           $${CTAGS_VERSION}/strlist.h \
           $${CTAGS_VERSION}/vstring.h

SOURCES = $${CTAGS_VERSION}/asm.c \
           $${CTAGS_VERSION}/asp.c \
           $${CTAGS_VERSION}/awk.c \
           $${CTAGS_VERSION}/basic.c \
           $${CTAGS_VERSION}/beta.c \
           $${CTAGS_VERSION}/c.c \
           $${CTAGS_VERSION}/cobol.c \
           $${CTAGS_VERSION}/exuberantCtags.c \
           $${CTAGS_VERSION}/eiffel.c \
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
           $${CTAGS_VERSION}/yacc.c
