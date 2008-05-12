# I'm putting original assistant change here and include in the plugin project, so i don't need to modify the original asssitant.pro

# original projects
include( src/3rdparty/fontpanel/fontpanel.pri )
include( src/3rdparty/assistant/assistant.pro )

# remove some variables
win32:RC_FILE	-= assistant.rc
mac:ICON	-= assitant.icns

# add some depend/include path
DEPENDPATH	+= src/3rdparty/assistant
INCLUDEPATH	+= src/3rdparty/assistant

# remove some files
SOURCES	-= main.cpp

# remove install script
INSTALLS	-= target