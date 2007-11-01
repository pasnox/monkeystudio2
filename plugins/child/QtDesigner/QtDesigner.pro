include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/child
TARGET	= QtDesigner
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""
INCLUDEPATH	*= src/3rdparty/designer
RESOURCES	*= src/resources/QtDesigner.qrc
CONFIG	*= designer
qtAddLibrary( QtDesignerComponents )

SOURCES	= src/QtDesigner.cpp \
	src/QtDesignerChild.cpp \
	src/QDesignerActionEditor.cpp \
	src/QDesignerObjectInspector.cpp \
	src/QDesignerPropertyEditor.cpp \
	src/QDesignerSignalSlotEditor.cpp \
	src/QDesignerWidgetBox.cpp

HEADERS	= src/QtDesigner.h \
	src/QtDesignerChild.h \
	src/QDesignerActionEditor.h \
	src/QDesignerObjectInspector.h \
	src/QDesignerPropertyEditor.h \
	src/QDesignerSignalSlotEditor.h \
	src/QDesignerWidgetBox.h
