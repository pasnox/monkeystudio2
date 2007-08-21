# fresh demo project file

TEMPLATE	= app

TARGET	= fresh_demo

BUILD_PATH	 = build
unix:OBJECTS_DIR	= $$BUILD_PATH/.o/unix
win32:OBJECTS_DIR	= $$BUILD_PATH/.o/win32
mac:OBJECTS_DIR	= $$BUILD_PATH/.o/mac
UI_DIR	= $$BUILD_PATH/.ui
MOC_DIR	= $$BUILD_PATH/.moc
RCC_DIR	= $$BUILD_PATH/.rcc

include( ../fresh.pro )

CONFIG	+= release

SOURCES	+= main.cpp
