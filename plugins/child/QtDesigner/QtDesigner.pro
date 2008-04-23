TARGET	= QtDesigner
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/child
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""
INCLUDEPATH	*= src/3rdparty/designer
CONFIG	*= designer
qtAddLibrary( QtDesignerComponents )

RESOURCES	*= src/resources/QtDesigner.qrc

HEADERS	= src/QtDesigner.h \
	src/QtDesignerChild.h \
	src/QDesignerActionEditor.h \
	src/QDesignerObjectInspector.h \
	src/QDesignerPropertyEditor.h \
	src/QDesignerSignalSlotEditor.h \
	src/QDesignerWidgetBox.h

SOURCES	= src/QtDesigner.cpp \
	src/QtDesignerChild.cpp \
	src/QDesignerActionEditor.cpp \
	src/QDesignerObjectInspector.cpp \
	src/QDesignerPropertyEditor.cpp \
	src/QDesignerSignalSlotEditor.cpp \
	src/QDesignerWidgetBox.cpp
