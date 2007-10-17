TEMPLATE        = lib
QT += network
TARGET                = QtAssistantClient
isEmpty(QT_MAJOR_VERSION) {
   VERSION=4.3.0
} else {
   VERSION=$${QT_MAJOR_VERSION}.$${QT_MINOR_VERSION}.$${QT_PATCH_VERSION}
}

CONFIG                += qt warn_on
mac|win32:CONFIG                += debug_and_release
mac:unix:CONFIG       += explicitlib
CONFIG                -= dll

HEADERS         = qassistantclient.h \
                  qassistantclient_global.h
SOURCES         = qassistantclient.cpp

DESTDIR                = ../../../lib
DLLDESTDIR             = ../../../bin

unix {
        QMAKE_CFLAGS += $$QMAKE_CFLAGS_SHLIB
        QMAKE_CXXFLAGS += $$QMAKE_CXXFLAGS_SHLIB
}

DEFINES += QT_ASSISTANT_CLIENT_LIBRARY
contains(CONFIG, static) {
    DEFINES += QT_ASSISTANT_CLIENT_STATIC
}

#load up the headers info
CONFIG += qt_install_headers
HEADERS_PRI = $$QT_BUILD_TREE/include/QtAssistant/headers.pri
include($$HEADERS_PRI)|clear(HEADERS_PRI)

#mac frameworks
mac:!static:contains(QT_CONFIG, qt_framework) {
   TARGET = QtAssistant    # Change the name to match the headers
   QMAKE_FRAMEWORK_BUNDLE_NAME = $$TARGET
   CONFIG += lib_bundle qt_no_framework_direct_includes qt_framework
   CONFIG(debug, debug|release) {
      !build_pass:CONFIG += build_all
   } else { #release
      !debug_and_release|build_pass {
	  CONFIG -= qt_install_headers #no need to install these as well
	  FRAMEWORK_HEADERS.version = Versions
	  FRAMEWORK_HEADERS.files = $$SYNCQT.HEADER_FILES $$SYNCQT.HEADER_CLASSES
      	  FRAMEWORK_HEADERS.path = Headers
      }
      QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
   }
}

!debug_and_release|build_pass {
   CONFIG(debug, debug|release) {
      mac:TARGET = $$member(TARGET, 0)_debug
      win32:TARGET = $$member(TARGET, 0)d
   }
}

target.path=$$[QT_INSTALL_LIBS]
INSTALLS        += target

qt_install_headers {
    assistant_headers.files = $$SYNCQT.HEADER_FILES $$SYNCQT.HEADER_CLASSES
    assistant_headers.path = $$[QT_INSTALL_HEADERS]/QtAssistant
    INSTALLS        += assistant_headers
}
