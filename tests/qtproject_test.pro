include( ../../plugins.inc )

# my comment

mac:LIBS	+= -framework SDL # framework sdl comment
else:LIBS	+= -lSDL # lib sdl comment

win32 { # the win32 scope comment
	include( toto.pri ) # include comment
	LIBS	+= -lie4 # lib ie4 comment
}

mac { # the mac scope comment
	include( toto.pri ) # hihihihi
	LIBS	+= -lie4
} else { # the else comment
	IDE	+= MONKEY_STUDIO
}

imbricScope { # the imbricScope scope comment
	winxp {
		WINXP	= true \
			false \ # false comment
			# gigigi \
			ggggg # ggggg comment

		# mlouil
	} else {
		WINXP	= false
	}
} else {
	include( oki.pri )
	# comm
}

DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= FileBrowser
RESOURCES	= src/resources/FileBrowser.qrc

TRANSLATIONS	= okmichel.pro \
	okgerard.pri \ # okgerard comment
	#okrobert \
	#	oktoto \
	#	okipoirot \
	# okhenry.pru \
	oktiti.h

# pouet comme !

HEADERS	= src/FileBrowser.h \ # src/filebrowser comment
	src/pDockFileBrowser.h \
	src/FileBrowserSettings.h

SOURCES	= src/FileBrowser.cpp \
	src/pDockFileBrowser.cpp \
	src/FileBrowserSettings.cpp

OTHER_FILES	= ../license.gpl \
	../readme.txt \
	../dev-readme

