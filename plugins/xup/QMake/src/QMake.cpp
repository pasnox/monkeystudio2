/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "QMake.h"
#include "QtVersionManager.h"
#include "QMakeProjectItem.h"
#include "UISettingsQMake.h"

#include <coremanager/MonkeyCore.h>
#include <maininterface/UIMain.h>
#include <pMonkeyStudio.h>

#include <QDir>

// Q_GLOBAL_STATIC( Type, accessorMemberName )
QPointer<QtVersionManager> QMake::mQtVersionManager = 0;

QWidget* QMake::settingsWidget() const
{
    return new UISettingsQMake( MonkeyCore::mainWindow() );
}

QStringList QMake::availableParsers() const
{
    return QStringList( PLUGIN_NAME );
}

pCommand QMake::defaultCommand() const
{
    return pCommand();
}

QtVersionManager* QMake::versionManager()
{
    return mQtVersionManager;
}

void QMake::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "QMake Project" );
    mPluginInfos.Description = tr( "QMake Project support for XUPManager" );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Roper Alexander aka minirop <minirop@peyj.com>";
    mPluginInfos.Type = BasePlugin::iBase | BasePlugin::iCLITool | BasePlugin::iXUP;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.1.0";
    mPluginInfos.FirstStartEnabled = true;
    mPluginInfos.HaveSettingsWidget = true;
    mPluginInfos.dependencies
        << "MSVCMake"
        << "GNUMake"
        << "BeaverDebugger"
        ;
}

bool QMake::install()
{
    mQtVersionManager = new QtVersionManager( this );
    
    const QStringList cf = pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
    mFilters = DocumentFilterMap( ":/qmakeitems" );
    QStringList hf; // HEADERS filters
    QStringList sf; // SOURCES filters
    QStringList yf; // YACC filters
    QStringList lf; // LEX filters
    int weight = 0;
    
    foreach ( const QString& f, cf ) {
        if ( f.startsWith( "*.h", Qt::CaseInsensitive ) ) {
            hf << f;
        }
        
        if ( f.startsWith( "*.c", Qt::CaseInsensitive ) ) {
            sf << f;
        }
    }
    
    foreach ( const QString& s, sf ) {
        const QString y = QString( s ).replace( "c", "y", Qt::CaseInsensitive );
        const QString l = QString( s ).replace( "c", "l", Qt::CaseInsensitive );
        
        if ( !yf.contains( y ) ) {
            yf << y;
        }
        
        if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !lf.contains( l ) ) {
            lf << l;
        }
    }
    
    mFilters[ "PROJECT" ].weight = weight++;
    mFilters[ "PROJECT" ].label = tr( "Qt Projects" );
    mFilters[ "PROJECT" ].icon = "project.png";
    mFilters[ "PROJECT" ].type = DocumentFilter::Project;
    mFilters[ "PROJECT" ].filters << "*.pro";
    
    mFilters[ "INCLUDE_PROJECT" ].weight = weight++;
    mFilters[ "INCLUDE_PROJECT" ].label = tr( "Qt Include Projects" );
    mFilters[ "INCLUDE_PROJECT" ].icon = "project.png";
    mFilters[ "INCLUDE_PROJECT" ].type = DocumentFilter::Project;
    mFilters[ "INCLUDE_PROJECT" ].filters << "*.pri" << "*.prf";
    
    mFilters[ "FORMS3" ].weight = weight++;
    mFilters[ "FORMS3" ].label = QMakeProjectItem::tr( "Qt3 Forms" );
    mFilters[ "FORMS3" ].icon = "forms3.png";
    mFilters[ "FORMS3" ].filters << "*.ui";
    mFilters[ "FORMS3" ].type = DocumentFilter::File;
    mFilters[ "FORMS3" ].filtered = true;
    
    mFilters[ "FORMS" ].weight = weight++;
    mFilters[ "FORMS" ].label = QMakeProjectItem::tr( "Qt Forms" );
    mFilters[ "FORMS" ].icon = "forms.png";
    mFilters[ "FORMS" ].filters << "*.ui";
    mFilters[ "FORMS" ].type = DocumentFilter::File;
    mFilters[ "FORMS" ].filtered = true;
    
    mFilters[ "HEADERS" ].weight = weight++;
    mFilters[ "HEADERS" ].label = QMakeProjectItem::tr( "C++ Headers" );
    mFilters[ "HEADERS" ].icon = "headers.png";
    mFilters[ "HEADERS" ].filters << hf;
    mFilters[ "HEADERS" ].type = DocumentFilter::File;
    mFilters[ "HEADERS" ].filtered = true;
    
    mFilters[ "SOURCES" ].weight = weight++;
    mFilters[ "SOURCES" ].label = QMakeProjectItem::tr( "C++ Sources" );
    mFilters[ "SOURCES" ].icon = "sources.png";
    mFilters[ "SOURCES" ].filters << sf;
    mFilters[ "SOURCES" ].type = DocumentFilter::File;
    mFilters[ "SOURCES" ].filtered = true;
    
    mFilters[ "OBJECTIVE_SOURCES" ].weight = weight++;
    mFilters[ "OBJECTIVE_SOURCES" ].label = QMakeProjectItem::tr( "Objective-C++ Sources" );
    mFilters[ "OBJECTIVE_SOURCES" ].icon = "obj_sources.png";
    mFilters[ "OBJECTIVE_SOURCES" ].filters << "*.m" << "*.mm";
    mFilters[ "OBJECTIVE_SOURCES" ].type = DocumentFilter::File;
    mFilters[ "OBJECTIVE_SOURCES" ].filtered = true;
    
    mFilters[ "YACCSOURCES" ].weight = weight++;
    mFilters[ "YACCSOURCES" ].label = QMakeProjectItem::tr( "YACC Sources" );
    mFilters[ "YACCSOURCES" ].icon = "yacc_sources.png";
    mFilters[ "YACCSOURCES" ].filters << yf;
    mFilters[ "YACCSOURCES" ].type = DocumentFilter::File;
    mFilters[ "YACCSOURCES" ].filtered = true;
    
    mFilters[ "LEXSOURCES" ].weight = weight++;
    mFilters[ "LEXSOURCES" ].label = QMakeProjectItem::tr( "LEX Sources" );
    mFilters[ "LEXSOURCES" ].icon = "lex_sources.png";
    mFilters[ "LEXSOURCES" ].filters << lf;
    mFilters[ "LEXSOURCES" ].type = DocumentFilter::File;
    mFilters[ "LEXSOURCES" ].filtered = true;
    
    mFilters[ "TRANSLATIONS" ].weight = weight++;
    mFilters[ "TRANSLATIONS" ].label = QMakeProjectItem::tr( "Qt Translations" );
    mFilters[ "TRANSLATIONS" ].icon = "translations.png";
    mFilters[ "TRANSLATIONS" ].filters << "*.ts";
    mFilters[ "TRANSLATIONS" ].type = DocumentFilter::File;
    mFilters[ "TRANSLATIONS" ].filtered = true;
    
    mFilters[ "RESOURCES" ].weight = weight++;
    mFilters[ "RESOURCES" ].label = QMakeProjectItem::tr( "Qt Resources" );
    mFilters[ "RESOURCES" ].icon = "resources.png";
    mFilters[ "RESOURCES" ].filters << "*.qrc";
    mFilters[ "RESOURCES" ].type = DocumentFilter::File;
    mFilters[ "RESOURCES" ].filtered = true;
    
    mFilters[ "DEF_FILE" ].weight = weight++;
    mFilters[ "DEF_FILE" ].label = QMakeProjectItem::tr( "DEF Files" );
    mFilters[ "DEF_FILE" ].icon = "def_file.png";
    mFilters[ "DEF_FILE" ].filters << "*.def";
    mFilters[ "DEF_FILE" ].type = DocumentFilter::File;
    mFilters[ "DEF_FILE" ].filtered = true;
    
    mFilters[ "RC_FILE" ].weight = weight++;
    mFilters[ "RC_FILE" ].label = QMakeProjectItem::tr( "RC Files" );
    mFilters[ "RC_FILE" ].icon = "rc_file.png";
    mFilters[ "RC_FILE" ].filters << "*.rc";
    mFilters[ "RC_FILE" ].type = DocumentFilter::File;
    mFilters[ "RC_FILE" ].filtered = true;
    
    mFilters[ "RES_FILE" ].weight = weight++;
    mFilters[ "RES_FILE" ].label = QMakeProjectItem::tr( "RES Files" );
    mFilters[ "RES_FILE" ].icon = "res_file.png";
    mFilters[ "RES_FILE" ].filters << "*.res";
    mFilters[ "RES_FILE" ].type = DocumentFilter::File;
    mFilters[ "RES_FILE" ].filtered = true;
    
    mFilters[ "SUBDIRS" ].weight = weight++;
    mFilters[ "SUBDIRS" ].label = QMakeProjectItem::tr( "Qt Sub Projects" );
    mFilters[ "SUBDIRS" ].icon = "project.png";
    mFilters[ "SUBDIRS" ].filters << mFilters[ "PROJECT" ].filters << mFilters[ "INCLUDE_PROJECT" ].filters;
    mFilters[ "SUBDIRS" ].type = DocumentFilter::File;
    mFilters[ "SUBDIRS" ].filtered = false;
    
    mFilters[ "OTHER_FILES" ].weight = weight++;
    mFilters[ "OTHER_FILES" ].label = QMakeProjectItem::tr( "Other Files" );
    mFilters[ "OTHER_FILES" ].icon = "file.png";
    mFilters[ "OTHER_FILES" ].filters << "*.txt" << "*README*" << "*Change*" << "*.sh" << "*.bat" << "*.html" << "*.xml" << "*.ini" << "*Makefile*";
    mFilters[ "OTHER_FILES" ].type = DocumentFilter::File;
    mFilters[ "OTHER_FILES" ].filtered = true;
    
    //
    mFilters[ "INCLUDEPATH" ].weight = weight++;
    mFilters[ "INCLUDEPATH" ].label = QMakeProjectItem::tr( "Includes Paths" );
    mFilters[ "INCLUDEPATH" ].icon = "includepath.png";
    mFilters[ "INCLUDEPATH" ].filters.clear();
    mFilters[ "INCLUDEPATH" ].type = DocumentFilter::Path;
    mFilters[ "INCLUDEPATH" ].filtered = false; // was true
    
    mFilters[ "DEPENDPATH" ].weight = weight++;
    mFilters[ "DEPENDPATH" ].label = QMakeProjectItem::tr( "Depends Paths" );
    mFilters[ "DEPENDPATH" ].icon = "dependpath.png";
    mFilters[ "DEPENDPATH" ].filters.clear();
    mFilters[ "DEPENDPATH" ].type = DocumentFilter::Path;
    mFilters[ "DEPENDPATH" ].filtered = false; // was true
    
    mFilters[ "VPATH" ].weight = weight++;
    mFilters[ "VPATH" ].label = QMakeProjectItem::tr( "Virtuals Paths" );
    mFilters[ "VPATH" ].icon = "vpath.png";
    mFilters[ "VPATH" ].filters.clear();
    mFilters[ "VPATH" ].type = DocumentFilter::Path;
    mFilters[ "VPATH" ].filtered = false; // was true
    
    //
    mFilters[ "LIBS" ].weight = weight++;
    mFilters[ "LIBS" ].label = QMakeProjectItem::tr( "Libraries Files" );
    mFilters[ "LIBS" ].icon = "libs.png";
    mFilters[ "LIBS" ].filters.clear();
    mFilters[ "LIBS" ].type = DocumentFilter::Standard;
    mFilters[ "LIBS" ].filtered = false; // was true
    
    mFilters[ "DEFINES" ].weight = weight++;
    mFilters[ "DEFINES" ].label = QMakeProjectItem::tr( "Defines" );
    mFilters[ "DEFINES" ].icon = "defines.png";
    mFilters[ "DEFINES" ].filters.clear();
    mFilters[ "DEFINES" ].type = DocumentFilter::Standard;
    mFilters[ "DEFINES" ].filtered = false; // was true
    
    MonkeyCore::projectTypesIndex()->registerType( PLUGIN_NAME, &QMakeProjectItem::staticMetaObject, mFilters );
    return true;
    
    /*
        BLD_INF_RULES
        CONFIG
        DEFINES
        DEF_FILE
        DEPENDPATH
        DEPLOYMENT
        DEPLOYMENT_PLUGIN
        DESTDIR
        DESTDIR_TARGET
        DLLDESTDIR
        DISTFILES
        DSP_TEMPLATE
        FORMS
        FORMS3
        GUID
        HEADERS
        ICON
        INCLUDEPATH
        INSTALLS
        LEXIMPLS
        LEXOBJECTS
        LEXSOURCES
        LIBS
        LITERAL_HASH
        MAKEFILE
        MAKEFILE_GENERATOR
        MMP_RULES
        MOC_DIR
        OBJECTS
        OBJECTS_DIR
        OBJMOC
        POST_TARGETDEPS
        PRE_TARGETDEPS
        PRECOMPILED_HEADER
        PWD
        OUT_PWD
        QMAKE
        QMAKESPEC
        QMAKE_APP_FLAG
        QMAKE_APP_OR_DLL
        QMAKE_AR_CMD
        QMAKE_BUNDLE_DATA
        QMAKE_BUNDLE_EXTENSION
        QMAKE_CC
        QMAKE_CFLAGS_DEBUG
        QMAKE_CFLAGS_MT
        QMAKE_CFLAGS_MT_DBG
        QMAKE_CFLAGS_MT_DLL
        QMAKE_CFLAGS_MT_DLLDBG
        QMAKE_CFLAGS_RELEASE
        QMAKE_CFLAGS_SHLIB
        QMAKE_CFLAGS_THREAD
        QMAKE_CFLAGS_WARN_OFF
        QMAKE_CFLAGS_WARN_ON
        QMAKE_CLEAN
        QMAKE_CXX
        QMAKE_CXXFLAGS
        QMAKE_CXXFLAGS_DEBUG
        QMAKE_CXXFLAGS_MT
        QMAKE_CXXFLAGS_MT_DBG
        QMAKE_CXXFLAGS_MT_DLL
        QMAKE_CXXFLAGS_MT_DLLDBG
        QMAKE_CXXFLAGS_RELEASE
        QMAKE_CXXFLAGS_SHLIB
        QMAKE_CXXFLAGS_THREAD
        QMAKE_CXXFLAGS_WARN_OFF
        QMAKE_CXXFLAGS_WARN_ON
        QMAKE_DISTCLEAN
        QMAKE_EXTENSION_SHLIB
        QMAKE_EXT_MOC
        QMAKE_EXT_UI
        QMAKE_EXT_PRL
        QMAKE_EXT_LEX
        QMAKE_EXT_YACC
        QMAKE_EXT_OBJ
        QMAKE_EXT_CPP
        QMAKE_EXT_H
        QMAKE_EXTRA_COMPILERS
        QMAKE_EXTRA_TARGETS
        QMAKE_FAILED_REQUIREMENTS
        QMAKE_FILETAGS
        QMAKE_FRAMEWORK_BUNDLE_NAME
        QMAKE_FRAMEWORK_VERSION
        QMAKE_INCDIR
        QMAKE_INCDIR_EGL
        QMAKE_INCDIR_OPENGL
        QMAKE_INCDIR_OPENGL_ES1, QMAKE_INCDIR_OPENGL_ES2
        QMAKE_INCDIR_OPENVG
        QMAKE_INCDIR_QT
        QMAKE_INCDIR_THREAD
        QMAKE_INCDIR_X11
        QMAKE_INFO_PLIST
        QMAKE_LFLAGS
        QMAKE_LFLAGS_CONSOLE
        QMAKE_LFLAGS_CONSOLE_DLL
        QMAKE_LFLAGS_DEBUG
        QMAKE_LFLAGS_PLUGIN
        QMAKE_LFLAGS_RPATH
        QMAKE_LFLAGS_QT_DLL
        QMAKE_LFLAGS_RELEASE
        QMAKE_LFLAGS_SHAPP
        QMAKE_LFLAGS_SHLIB
        QMAKE_LFLAGS_SONAME
        QMAKE_LFLAGS_THREAD
        QMAKE_LFLAGS_WINDOWS
        QMAKE_LFLAGS_WINDOWS_DLL
        QMAKE_LIBDIR
        QMAKE_LIBDIR_FLAGS
        QMAKE_LIBDIR_EGL
        QMAKE_LIBDIR_OPENGL
        QMAKE_LIBDIR_OPENVG
        QMAKE_LIBDIR_QT
        QMAKE_LIBDIR_X11
        QMAKE_LIBS
        QMAKE_LIBS_CONSOLE
        QMAKE_LIBS_EGL
        QMAKE_LIBS_OPENGL
        QMAKE_LIBS_OPENGL_QT
        QMAKE_LIBS_OPENGL_ES1, QMAKE_LIBS_OPENGL_ES2
        QMAKE_LIBS_OPENVG
        QMAKE_LIBS_QT
        QMAKE_LIBS_QT_DLL
        QMAKE_LIBS_QT_OPENGL
        QMAKE_LIBS_QT_THREAD
        QMAKE_LIBS_RT
        QMAKE_LIBS_RTMT
        QMAKE_LIBS_THREAD
        QMAKE_LIBS_WINDOWS
        QMAKE_LIBS_X11
        QMAKE_LIBS_X11SM
        QMAKE_LIB_FLAG
        QMAKE_LINK_SHLIB_CMD
        QMAKE_POST_LINK
        QMAKE_PRE_LINK
        QMAKE_LN_SHLIB
        QMAKE_MAC_SDK
        QMAKE_MACOSX_DEPLOYMENT_TARGET
        QMAKE_MAKEFILE
        QMAKE_MOC_SRC
        QMAKE_QMAKE
        QMAKE_QT_DLL
        QMAKE_RESOURCE_FLAGS
        QMAKE_RPATH
        QMAKE_RPATHDIR
        QMAKE_RUN_CC
        QMAKE_RUN_CC_IMP
        QMAKE_RUN_CXX
        QMAKE_RUN_CXX_IMP
        QMAKE_TARGET
        QMAKE_UIC
        QT
        QTPLUGIN
        QT_VERSION
        QT_MAJOR_VERSION
        QT_MINOR_VERSION
        QT_PATCH_VERSION
        RC_FILE
        RCC_DIR
        REQUIRES
        RESOURCES
        RES_FILE
        RSS_RULES
        S60_VERSION
        SIGNATURE_FILE
        SOURCES
        SRCMOC
        SUBDIRS
        SYMBIAN_VERSION
        TARGET
        TARGET.CAPABILITY
        TARGET.EPOCALLOWDLLDATA
        TARGET.EPOCHEAPSIZE
        TARGET.EPOCSTACKSIZE
        TARGET.SID
        TARGET.UID2
        TARGET.UID3
        TARGET.VID
        TARGET_EXT
        TARGET_x
        TARGET_x.y.z
        TEMPLATE
        TRANSLATIONS
        UICIMPLS
        UICOBJECTS
        UI_DIR
        UI_HEADERS_DIR
        UI_SOURCES_DIR
        VERSION
        VER_MAJ
        VER_MIN
        VER_PAT
        VPATH
        YACCIMPLS
        YACCOBJECTS
        YACCSOURCES
        _PRO_FILE_
        _PRO_FILE_PWD_
    */
}

bool QMake::uninstall()
{
    MonkeyCore::projectTypesIndex()->unRegisterType( PLUGIN_NAME );
    mFilters.clear();
    delete mQtVersionManager;
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
#endif
