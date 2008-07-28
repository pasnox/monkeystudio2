include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/reseau
TARGET	= Irc
QT	*=  xml network gui

RESOURCES	*= src/resources/irc.qrc

FORMS += src/ui/UIIrcMain.ui


HEADERS	+= src/IrcDock.h \
	src/Irc.h \
	src/IrcChannel.h \
	src/ui/UIIrcStatus.h

SOURCES	+= src/IrcDock.cpp \
	src/Irc.cpp \
	src/IrcChannel.cpp \
	src/ui/UIIrcStatus.cpp


