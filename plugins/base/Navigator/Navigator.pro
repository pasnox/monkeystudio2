include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= Navigator
RESOURCES	= icons/icons.qrc

HEADERS	= src/Navigator.h \
		src/Entity.h \
		src/EntityContainer.h \
		src/NavigatorSettings.h

SOURCES	= src/Navigator.cpp \
		src/Entity.cpp \
		src/EntityContainer.cpp \
		src/NavigatorSettings.cpp
