TARGET  = QtDesigner
include( ../../plugins.pri )
DESTDIR = $$MONKEY_PLUGINS_DIR/child
INCLUDEPATH *= src/3rdparty/designer/$${QT_MAJOR_VERSION}.$${QT_MINOR_VERSION}
greaterThan(QT_MAJOR_VERSION, 4):QT *= designer
else:CONFIG  *= designer
!macx:LIBS *= -lQtDesignerComponents
else:LIBS *= "-framework QtDesignerComponents"

RESOURCES   *= src/resources/QtDesigner.qrc

HEADERS = src/QtDesigner.h \
    src/QtDesignerChild.h \
    src/QDesignerActionEditor.h \
    src/QDesignerObjectInspector.h \
    src/QDesignerPropertyEditor.h \
    src/QDesignerSignalSlotEditor.h \
    src/QDesignerWidgetBox.h \
    src/QDesignerResourcesEditor.h \
    src/QtDesignerManager.h \
    src/LegacyDesigner.h \
    src/MkSDesignerIntegration.h

SOURCES = src/QtDesigner.cpp \
    src/QtDesignerChild.cpp \
    src/QDesignerActionEditor.cpp \
    src/QDesignerObjectInspector.cpp \
    src/QDesignerPropertyEditor.cpp \
    src/QDesignerSignalSlotEditor.cpp \
    src/QDesignerWidgetBox.cpp \
    src/QDesignerResourcesEditor.cpp \
    src/QtDesignerManager.cpp \
    src/LegacyDesigner.cpp \
    src/MkSDesignerIntegration.cpp

include( src/3rdparty/qtcreator/designerintegrationv2/designerintegration.pri )