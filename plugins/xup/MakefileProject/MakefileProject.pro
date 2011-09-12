TARGET  = MakefileProject
include( ../../plugins.pri )
DESTDIR = $$MONKEY_PLUGINS_DIR/xup

#RESOURCES  = src/resources/MakefileProject.qrc


HEADERS = MakefileProject.h \
    MakefileProjectItem.h

SOURCES = MakefileProject.cpp \
    MakefileProjectItem.cpp

