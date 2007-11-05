# Monkey Studio 2 Ctags library

include( ctags.pri )

BUILD_PATH	 = ../build

TEMPLATE = lib
CONFIG	*= staticlib warn_on debug_and_release
DESTDIR	= $${BUILD_PATH}
DLL_DESTDIR	= $$DESTDIR
win32-msvc*:DEFINES	*= WIN32 HAVE_REGCOMP REGEX_MALLOC STDC_HEADERS=1

CONFIG(DebugBuild)|CONFIG(debug, debug|release) {
	#Debug
	CONFIG	+= console
	unix:TARGET	= $$join(TARGET,,,_debug)
	else:TARGET	= $$join(TARGET,,,d)
	unix:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/mac
	UI_DIR	= $${BUILD_PATH}/debug/.ui
	MOC_DIR	= $${BUILD_PATH}/debug/.moc
	RCC_DIR	= $${BUILD_PATH}/debug/.rcc
	
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
}

CTAGS_PATH	= $${CTAGS_VERSION}

HEADERS	= $${CTAGS_PATH}/debug.h \
	$${CTAGS_PATH}/entry.h \
	$${CTAGS_PATH}/general.h \
	$${CTAGS_PATH}/get.h \
	$${CTAGS_PATH}/keyword.h \
	$${CTAGS_PATH}/options.h \
	$${CTAGS_PATH}/parse.h \
	$${CTAGS_PATH}/read.h \
	$${CTAGS_PATH}/routines.h \
	$${CTAGS_PATH}/strlist.h \
	$${CTAGS_PATH}/vstring.h \
	$${CTAGS_PATH}/readtags.h \
    $${CTAGS_PATH}/sort.h \
	$${CTAGS_PATH}/args.h \
    $${CTAGS_PATH}/ctags.h
#	src/exuberantCtags.h \

win32-msvc*:HEADERS	*= $${CTAGS_PATH}/regex.h

SOURCES	= $${CTAGS_PATH}/asm.c \
	$${CTAGS_PATH}/asp.c \
	$${CTAGS_PATH}/awk.c \
	$${CTAGS_PATH}/basic.c \
	$${CTAGS_PATH}/beta.c \
	$${CTAGS_PATH}/c.c \
	$${CTAGS_PATH}/cobol.c \
	$${CTAGS_PATH}/eiffel.c \
	$${CTAGS_PATH}/debug.c \
	$${CTAGS_PATH}/entry.c \
	$${CTAGS_PATH}/erlang.c \
	$${CTAGS_PATH}/fortran.c \
	$${CTAGS_PATH}/get.c \
	$${CTAGS_PATH}/html.c \
	$${CTAGS_PATH}/jscript.c \
	$${CTAGS_PATH}/keyword.c \
	$${CTAGS_PATH}/lisp.c \
	$${CTAGS_PATH}/lregex.c \
	$${CTAGS_PATH}/lua.c \
	$${CTAGS_PATH}/make.c \
	$${CTAGS_PATH}/options.c \
	$${CTAGS_PATH}/parse.c \
	$${CTAGS_PATH}/pascal.c \
	$${CTAGS_PATH}/perl.c \
	$${CTAGS_PATH}/php.c \
	$${CTAGS_PATH}/python.c \
	$${CTAGS_PATH}/read.c \
	$${CTAGS_PATH}/rexx.c \
	$${CTAGS_PATH}/routines.c \
	$${CTAGS_PATH}/ruby.c \
	$${CTAGS_PATH}/scheme.c \
	$${CTAGS_PATH}/sh.c \
	$${CTAGS_PATH}/slang.c \
	$${CTAGS_PATH}/sml.c \
	$${CTAGS_PATH}/sql.c \
	$${CTAGS_PATH}/strlist.c \
	$${CTAGS_PATH}/tcl.c \
	$${CTAGS_PATH}/verilog.c \
	$${CTAGS_PATH}/vim.c \
	$${CTAGS_PATH}/vstring.c \
	$${CTAGS_PATH}/yacc.c \
	$${CTAGS_PATH}/readtags.c \
	$${CTAGS_PATH}/sort.c \
	$${CTAGS_PATH}/args.c
#	$${CTAGS_PATH}/argproc.c \
#	$${CTAGS_PATH}/mac.c \
#	$${CTAGS_PATH}/qdos.c \
#	$${CTAGS_PATH}/e_amiga.h \
#	$${CTAGS_PATH}/e_djgpp.h \
#	$${CTAGS_PATH}/e_mac.h \
#	$${CTAGS_PATH}/e_msoft.h \
#	$${CTAGS_PATH}/e_os2.h \
#	$${CTAGS_PATH}/e_qdos.h \
#	$${CTAGS_PATH}/e_riscos.h \
#	$${CTAGS_PATH}/e_vms.h \
#	$${CTAGS_PATH}/main.c \
#	src/exuberantCtags.c \

win32-msvc*:SOURCES	*= $${CTAGS_PATH}/regex.c
