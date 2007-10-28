include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= QtDesigner
DEFINES	*= "PLUGIN_NAME=\"\\\"$${TARGET}\\\"\""
INCLUDEPATH	*= src/3rdparty/designer src/3rdparty/edyuk_designer
RESOURCES	*= src/resources/designer.qrc
CONFIG	*= designer
qtAddLibrary( QtDesignerComponents )

SOURCES	= src/QtDesigner.cpp \
	src/QtDesignerChild.cpp \
	src/3rdparty/edyuk_designer/qdesigneractioneditor.cpp \
	src/3rdparty/edyuk_designer/qdesignerobjectinspector.cpp \
	src/3rdparty/edyuk_designer/qdesignerpropertyeditor.cpp \
	src/3rdparty/edyuk_designer/qdesignersignalsloteditor.cpp \
	src/3rdparty/edyuk_designer/qdesignerwidgetbox.cpp

HEADERS	= src/QtDesigner.h \
	src/QtDesignerChild.h \
	src/3rdparty/edyuk_designer/qdesigneractioneditor.h \
	src/3rdparty/edyuk_designer/qdesignerobjectinspector.h \
	src/3rdparty/edyuk_designer/qdesignerpropertyeditor.h \
	src/3rdparty/edyuk_designer/qdesignersignalsloteditor.h \
	src/3rdparty/edyuk_designer/qdesignerwidgetbox.h
