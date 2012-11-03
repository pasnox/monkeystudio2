# Monkey Studio Install Project File

include( config.pri )

!build_pass {
    cb_win32:message( "Crossbuilding for Windows" )
    cb_mac:message( "Crossbuilding for Darwin" )
    isEqual( SYSTEM_QSCINTILLA, 1 ):message( "Using system QScintilla library" )
    else:message( "Using integrated QScintilla library" )
    message( "You can change qscintilla link type by giving the qmake parameter variable: ('qmake -set system_qscintilla 0' or qmake -set system_qscintilla 1')" )
    message( "Defining system_qscintilla=1 will use system wide qscintilla installation, else the integrated one." )
    message( "MkS binary will be installed to : $$PACKAGE_PREFIX" )
    message( "MkS plugins will be installed to : $$PACKAGE_PLUGINS" )
    message( "MkS datas will be installed to : $$PACKAGE_DATAS" )
    message( "MkS docs will be installed to : $$PACKAGE_DOCS" )
    message( "You can change this by giving qmake parameters variables: prefix, plugins, datas" )
    
    # path list - used for removing hidden files and crappy things
    install_folders = 
    
    mac {
        dataFolder  = macos
    } else:win32 {
        dataFolder = windows
    } else {
        dataFolder = unix
    }
    
    toolsFolder = ../../tools
    !exists( $${toolsFolder} ):toolsFolder    = ../tools
    !exists( $${toolsFolder} ):toolsFolder    = tools
    toolsDataFolder = $${toolsFolder}/data/$${dataFolder}
    
    # licenses, readme, changelog
    monkey_docs.path = $${PACKAGE_DOCS}
    monkey_docs.files = GPL-2 \
        GPL-3 \
        LGPL-3 \
        dev-readme \
        readme.txt \
        ChangeLog
    install_folders *= $${monkey_docs.path}

    # datas
    monkey_datas.path   = $${PACKAGE_DATAS}
    monkey_datas.files  = datas/*
    monkey_datas.CONFIG *= no_check_exist
    install_folders *= $${monkey_datas.path}

    # debugger know_list_and_id
    #monkey_debuggerIniFile.path    = $${PACKAGE_PLUGINS}/GNUdbg
    #monkey_debuggerIniFile.files   = plugins/debugger/GNUDebugger/file
    #monkey_debuggerIniFile.CONFIG *= no_check_exist
    #install_folders *= $${monkey_debuggerIniFile.path}

    INSTALLS    = monkey_datas monkey_docs

    unix:!mac {
        # plugins
        monkey_plugins.path = $${PACKAGE_PLUGINS}
        monkey_plugins.files    = bin/plugins/*
        monkey_plugins.CONFIG *= no_check_exist
        install_folders *= $${monkey_plugins.path}

        # binary
        monkey_target.path  = $${PACKAGE_PREFIX}
        monkey_target.files = bin/$${PACKAGE_TARGET}
        monkey_target.CONFIG *= no_check_exist
        install_folders *= $${monkey_target.path}

        # desktop file
        monkey_desktop.path = $${prefix}/share/applications
        monkey_desktop.files    = $${toolsDataFolder}/monkeystudio.desktop
        install_folders *= $${monkey_desktop.path}

        # desktop icon file
        monkey_desktopicon.path = $${prefix}/share/pixmaps
        monkey_desktopicon.files    = $${toolsDataFolder}/monkeystudio.png
        install_folders *= $${monkey_desktopicon.path}

        INSTALLS    *= monkey_plugins monkey_target monkey_desktop monkey_desktopicon
    }
    
    mac|win32 {
        qtDeployFolder = $${PACKAGE_DATAS}/qt
        confFile = $${toolsDataFolder}/qt.conf
        
        qt_conf.path = $${PACKAGE_DATAS}
        qt_conf.files = $${confFile}
        install_folders *= $${qt_conf.path}
        
        qt_documentation.path = $${qtDeployFolder}/doc/qch
        qt_documentation.files = $$[QT_INSTALL_DOCS]/qch/assistant*.qch
        qt_documentation.files *= $$[QT_INSTALL_DOCS]/qch/designer*.qch
        qt_documentation.files *= $$[QT_INSTALL_DOCS]/qch/linguist*.qch
        qt_documentation.files *= $$[QT_INSTALL_DOCS]/qch/qmake*.qch
        qt_documentation.files *= $$[QT_INSTALL_DOCS]/qch/qml*.qch
        qt_documentation.files *= $$[QT_INSTALL_DOCS]/qch/qt*.qch
        install_folders *= $${qt_documentation.path}
        
        qt_translations.path = $${qtDeployFolder}/translations
        qt_translations.files = $$[QT_INSTALL_TRANSLATIONS]/assistant*.qm
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/designer*.qm
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/linguist*.qm
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/qscintilla*.qm
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/qt*.qm
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/qvfb*.qm
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/assistant*.ts
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/designer*.ts
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/linguist*.ts
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/qscintilla*.ts
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/qt*.ts
        qt_translations.files *= $$[QT_INSTALL_TRANSLATIONS]/qvfb*.ts
        install_folders *= $${qt_translations.path}
        
        INSTALLS *= qt_conf qt_documentation qt_translations
    }
    
    mac {
        # copy required qt cocoa menu nib file for cocoa qt builds
        QT_MENU_NIB = "$${QMAKE_LIBDIR_QT}/QtGui.framework/Resources/qt_menu.nib"
        
        exists( $${QT_MENU_NIB} ) {
            qt_menu_nib.path = $${PACKAGE_DATAS}
            qt_menu_nib.files = $${QT_MENU_NIB}
            
            INSTALLS *= qt_menu_nib
        }
    }
    
    # kleen commands to be called after make install so possible crappy / hidden files are deleted
    
    # don't show executed commands
    win32:!cb_win32:kleen.commands += "@echo off &"
    
    for( folder, install_folders ) {
        win32:!cb_win32 {
            folder = $$replace( folder, $${Q_SLASH}, $${Q_BACK_SLASH} )
            kleen.commands += "( @for /f %%i in ('dir /ad /b /s \"$${folder}\\*CVS\" \"$${folder}\\*.svn\" 2^> nul') do @rd /s /q \"%%i\" ) &" \
                "( @for /f %%i in ('dir /a-d /b /s \"$${folder}\\*.a\" \"$${folder}\\*.lib\" 2^> nul') do @del /f \"%%i\" ) &"
        } else {
            kleen.commands += "find \"$${folder}\" -type d -name \"CVS\" -o -name \".svn\" | xargs -d \"\\n\" rm -fr; " \
                "find \"$${folder}\" -type f -name \"*.a\" -o -name \"*.lib\" | xargs -d \"\\n\" rm -f; "
        }
    }
    
    # silently exit for non existing folders
    win32:!cb_win32:kleen.commands += "@exit 0"
    
    QMAKE_EXTRA_TARGETS *= kleen
}
