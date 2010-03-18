# include ctags framework
include( ../ctags/ctags.pri )

# include qCtagsSense framework
include( ../qCtagsSense/qCtagsSense.pri )

# include config file
include( ../config.pri )

TEMPLATE	= app
TARGET	= $$PACKAGE_TARGET
DESTDIR	= $$PACKAGE_DESTDIR

LIBS	*= -L$${PACKAGE_BUILD_PATH}
mac:*-g++:LIBS	*= -Wl,-all_load # import all symbols as the not used ones too
else:*-g++:LIBS	*= -Wl,--whole-archive # import all symbols as the not used ones too
mac:*-g++:LIBS	*= -dynamic
else:unix:*-g++:LIBS	*= -rdynamic

PRE_TARGETDEPS	*= ../ctags ../qCtagsSense

CONFIG( debug, debug|release ) {
	#Debug
	!isEqual( SYSTEM_QSCINTILLA, 1 ) {
		unix:	LIBS	*= -lctags_debug -lqCtagsSense_debug
		else:	LIBS	*= -lctagsd -lqCtagsSensed
	} else {
		unix:	LIBS	*= -lctags_debug -lqCtagsSense_debug
		else:	LIBS	*= -lctagsd -lqCtagsSensed
	}
	win32-*g++:LIBS	*= -Wl,--out-implib,$${PACKAGE_BUILD_PATH}/lib$${TARGET}.a
	win32-msvc*:LIBS	*= /IMPLIB:$${PACKAGE_BUILD_PATH}/$${TARGET}.lib -lshell32
} else {
	#Release
	LIBS	*= -lctags -lqCtagsSense
	win32-*g++:LIBS	*= -Wl,--out-implib,$${PACKAGE_BUILD_PATH}/lib$${TARGET}.a
	win32-msvc*:LIBS	*= /IMPLIB:$${PACKAGE_BUILD_PATH}/$${TARGET}.lib -lshell32
}

mac:*-g++:LIBS	*= -Wl,-noall_load # stop importing all symbols
else:*-g++:LIBS	*= -Wl,--no-whole-archive # stop importing all symbols

FORMS	*=

HEADERS	*=

SOURCES	*= src/main.cpp

TRANSLATIONS	*=