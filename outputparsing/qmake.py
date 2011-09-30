import parsing

# WARNING: Failure to find
missingFile = parsing.Pattern( r"(?:^|\n)WARNING:\s*(Failure to find:\s*([^\n$]+))", file = '%2', type = 'error', text = '%1' )
missingFile.setComment( 'WARNING: Failure to find file' )

missingFile.test( 'WARNING: Failure to find: file.ext',
                    file = 'file.ext',
                    type = 'error',
                    text = 'Failure to find: file.ext',
                    hint = 'WARNING: Failure to find: file.ext' )

# Project MESSAGE
message = parsing.Pattern( r"(?:^|\n)Project MESSAGE:\s*([^\n$]+)", type = 'warning', text = '%1' )
message.setComment( 'Project MESSAGE' )

message.test( 'Project MESSAGE: this is my qmake project message',
                type = 'warning',
                text = 'this is my qmake project message',
                hint = 'Project MESSAGE: this is my qmake project message' )

# Reading
reading = parsing.Pattern( r"(?:^|\n)\s*(Reading\s*([^\n$]+))", file = '%2', type = 'warning', hint = '%1' )
reading.setComment( 'Reading' )

reading.test( '  Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
                file = '/home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
                type = 'warning',
                text = '  Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
                hint = 'Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro' )

# Warning: potential duplicate alias detected
duplicateAlias = parsing.Pattern( r"(?:^|\n)([^:]+):\s*Warning:\s*(potential duplicate alias detected:\s*'[^\n$]+')", file = '%1', type = 'warning', text = '%2' )
duplicateAlias.setComment( 'Warning: potential duplicate alias detected' )

duplicateAlias.test( 'src/resources/resources.qrc: Warning: potential duplicate alias detected: \'license.gpl\'',
                        file = 'src/resources/resources.qrc',
                        type = 'warning',
                        text = 'potential duplicate alias detected: \'license.gpl\'',
                        hint = 'src/resources/resources.qrc: Warning: potential duplicate alias detected: \'license.gpl\'' )

# lupdate/lrelease warning
#warning = parsing.Pattern( r"(?:^|\n)(?:lupdate|lrelease) warning:\s*([^']+'([^']+)'\.?)", file = '%2', type = 'warning', text = '%1' )
warning = parsing.Pattern( r"(?:^|\n)(?:lupdate|lrelease) warning:\s*((?:[^'\n$]+'[^'\n$]+')?[^'\n$]+'([^'\n$]+)'\.?)", file = '%2', type = 'warning', text = '%1' )
warning.setComment( 'lupdate/lrelease warning' )

warning.test( 'lupdate warning: no TS files specified. Only diagnostics will be produced for \'/home/pasnox/Temporaire/qtgui/qtgui.pro\'.',
                file = '/home/pasnox/Temporaire/qtgui/qtgui.pro',
                type = 'warning',
                text = 'no TS files specified. Only diagnostics will be produced for \'/home/pasnox/Temporaire/qtgui/qtgui.pro\'.',
                hint = 'lupdate warning: no TS files specified. Only diagnostics will be produced for \'/home/pasnox/Temporaire/qtgui/qtgui.pro\'.' )

warning.test( "lrelease warning: Met no 'TRANSLATIONS' entry in project file '/home/pasnox/Temporaire/cppqtgui/cppqtgui.pro'",
                file = '/home/pasnox/Temporaire/cppqtgui/cppqtgui.pro',
                type = 'warning',
                text = "Met no 'TRANSLATIONS' entry in project file '/home/pasnox/Temporaire/cppqtgui/cppqtgui.pro'",
                hint = "lrelease warning: Met no 'TRANSLATIONS' entry in project file '/home/pasnox/Temporaire/cppqtgui/cppqtgui.pro'" )

# cannot open
cannotOpen = parsing.Pattern( r"(?:^|\n)Cannot open [^:]+:\s*([^\n$]+)", type = 'warning', text = '%1' )
cannotOpen.setComment( 'cannot open' )

cannotOpen.test( 'Cannot open dependencies/intuisphere/core: file to open is a directory',
                type = 'warning',
                text = 'file to open is a directory',
                hint = 'Cannot open dependencies/intuisphere/core: file to open is a directory' )

# Updating TS/QM file
tsUpdate = parsing.Pattern( r"(?:^|\n)Updating\s*'([^']+)'[^\n$]+", file = '%1', type = 'warning' )
tsUpdate.setComment( 'Updating TS/QM file' )

tsUpdate.test( "Updating 'datas/languages/references/numento_en.ts'...",
                    file = 'datas/languages/references/numento_en.ts',
                    type = 'warning',
                    text = "Updating 'datas/languages/references/numento_en.ts'...",
                    hint = "Updating 'datas/languages/references/numento_en.ts'..." )

tsUpdate.test( "Updating 'datas/languages/references/numento_dependencies_it.qm'...",
                    file = 'datas/languages/references/numento_dependencies_it.qm',
                    type = 'warning',
                    text = "Updating 'datas/languages/references/numento_dependencies_it.qm'...",
                    hint = "Updating 'datas/languages/references/numento_dependencies_it.qm'..." )

# Updating TS/QM file info
tsUpdateInfo = parsing.Pattern( r"(?:^|\n)\s*((?:Found|Generated|Kept|Same-text)(?:\s*\d+)?[^\n$]+)", type = 'warning', hint = '%1' )
tsUpdateInfo.setComment( 'Updating TS/QM file info' )

tsUpdateInfo.test( '    Found 680 source text(s) (0 new and 680 already existing)',
                    type = 'warning',
                    text = '    Found 680 source text(s) (0 new and 680 already existing)',
                    hint = 'Found 680 source text(s) (0 new and 680 already existing)' )

tsUpdateInfo.test( '    Generated 123 translation(s) (123 finished and 0 unfinished)',
                    type = 'warning',
                    text = '    Generated 123 translation(s) (123 finished and 0 unfinished)',
                    hint = 'Generated 123 translation(s) (123 finished and 0 unfinished)' )

tsUpdateInfo.test( '    Kept 118 obsolete entries',
                        type = 'warning',
                        text = '    Kept 118 obsolete entries',
                        hint = 'Kept 118 obsolete entries' )

tsUpdateInfo.test( '    Same-text heuristic provided 9 translation(s)',
                        type = 'warning',
                        text = '    Same-text heuristic provided 9 translation(s)',
                        hint = 'Same-text heuristic provided 9 translation(s)' )

# generic filename + line + message
fileNameLine = parsing.Pattern( r"(?:^|\n)([^:(]+):?\(?(\d+)\)?:\s*([^\n$]+)", file = '%1', type = 'warning', line = '%2', text ='%3' )
fileNameLine.setComment( 'Generic filename + line + message' )

fileNameLine.test( '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):Function \'fromfile\' is not implemented',
                file = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri',
                type = 'warning',
                line = '7',
                text = 'Function \'fromfile\' is not implemented',
                hint = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):Function \'fromfile\' is not implemented' )

fileNameLine.test( '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):\'!contains\' is not a recognized test function',
                file = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri',
                type = 'warning',
                line = '7',
                text = '\'!contains\' is not a recognized test function',
                hint = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):\'!contains\' is not a recognized test function' )

fileNameLine.test( '/usr/include/qt4/QtCore/qstringbuilder.h:45: circular inclusion of /usr/include/qt4/QtCore/qstring.h',
                file = '/usr/include/qt4/QtCore/qstringbuilder.h',
                type = 'warning',
                line = '45',
                text = 'circular inclusion of /usr/include/qt4/QtCore/qstring.h',
                hint = '/usr/include/qt4/QtCore/qstringbuilder.h:45: circular inclusion of /usr/include/qt4/QtCore/qstring.h' )

fileNameLine.test( 'Qt4/qscilexerbash.cpp:209: Qualifying with unknown namespace/class ::QsciLexerBash',
                file = 'Qt4/qscilexerbash.cpp',
                type = 'warning',
                line = '209',
                text = 'Qualifying with unknown namespace/class ::QsciLexerBash',
                hint = 'Qt4/qscilexerbash.cpp:209: Qualifying with unknown namespace/class ::QsciLexerBash' )

fileNameLine.test( 'src/LexEScript.cpp:179: Discarding unconsumed meta data',
                file = 'src/LexEScript.cpp',
                type = 'warning',
                line = '179',
                text = 'Discarding unconsumed meta data',
                hint = 'src/LexEScript.cpp:179: Discarding unconsumed meta data' )

fileNameLine.test( 'sqlite3_unicode.c:2743: Unbalanced opening brace in C++ code (or abuse of the C++ preprocessor)',
                file = 'sqlite3_unicode.c',
                type = 'warning',
                line = '2743',
                text = 'Unbalanced opening brace in C++ code (or abuse of the C++ preprocessor)',
                hint = 'sqlite3_unicode.c:2743: Unbalanced opening brace in C++ code (or abuse of the C++ preprocessor)' )

# generic filename + message
fileName = parsing.Pattern( r"(?:^|\n)([^:]+):\s*(?:[^\n:]+):\s*([^\n$]+)", file = '%1', type = 'warning', text ='%2' )
fileName.setComment( 'Generic filename + message' )

fileName.test( "src/settings/UISettings.ui: Warning: Z-order assignment: 'horizontalLayoutWidget' is not a valid widget.",
                file = 'src/settings/UISettings.ui',
                type = 'warning',
                text = "Z-order assignment: 'horizontalLayoutWidget' is not a valid widget.",
                hint = "src/settings/UISettings.ui: Warning: Z-order assignment: 'horizontalLayoutWidget' is not a valid widget." )


# uic call
uic = parsing.Pattern( r"(?:^|\n)(?:[^\n$]+)?uic(?:-[\w\d\.]+)?\s+[^\n$]+\s+-o\s+([^\n$]+)",
                        type = 'compiling',
                        file = '%1',
                        text = 'Generating %1...' )

uic.setComment( "uic command line" )

uic.test( '/usr/bin/uic-qt4 src/usersdialog.ui -o /media/ramdisk/bodeasy/debug/ui/ui_usersdialog.h',
                    type = 'compiling',
                    file = '/media/ramdisk/bodeasy/debug/ui/ui_usersdialog.h',
                    text = 'Generating /media/ramdisk/bodeasy/debug/ui/ui_usersdialog.h...',
                    hint = '/usr/bin/uic-qt4 src/usersdialog.ui -o /media/ramdisk/bodeasy/debug/ui/ui_usersdialog.h' )

# moc call
moc = parsing.Pattern( r"(?:^|\n)(?:[^\n$]+)?moc(?:-[\w\d\.]+)?\s+[^\n$]+\s+-o\s+([^\n$]+)",
                        type = 'compiling',
                        file = '%1',
                        text = 'Generating %1...' )

moc.setComment( "moc command line" )

moc.test( '/usr/bin/moc-qt4 -DQT_SQL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtSql -I/usr/include/qt4 -I/usr/include/qt4/QtTest -I. -Isrc -I/usr/local/qwt-6.0.1/include -I/media/ramdisk/bodeasy/debug/moc -I/media/ramdisk/bodeasy/debug/ui src/sql/models/exercisesmodel.h -o /media/ramdisk/bodeasy/debug/moc/moc_exercisesmodel.cpp',
                    type = 'compiling',
                    file = '/media/ramdisk/bodeasy/debug/moc/moc_exercisesmodel.cpp',
                    text = 'Generating /media/ramdisk/bodeasy/debug/moc/moc_exercisesmodel.cpp...',
                    hint = '/usr/bin/moc-qt4 -DQT_SQL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtSql -I/usr/include/qt4 -I/usr/include/qt4/QtTest -I. -Isrc -I/usr/local/qwt-6.0.1/include -I/media/ramdisk/bodeasy/debug/moc -I/media/ramdisk/bodeasy/debug/ui src/sql/models/exercisesmodel.h -o /media/ramdisk/bodeasy/debug/moc/moc_exercisesmodel.cpp' )

# Generation of script file
print '# It is a machine generated file. Do not edit it manualy!'
print ''
print missingFile.generateMkSScript( 'QMake' )
print message.generateMkSScript( 'QMake' )
print reading.generateMkSScript( 'QMake' )
print duplicateAlias.generateMkSScript( 'QMake' )
print warning.generateMkSScript( 'QMake' )
print cannotOpen.generateMkSScript( 'QMake' )
print tsUpdate.generateMkSScript( 'QMake' )
print tsUpdateInfo.generateMkSScript( 'QMake' )
print fileNameLine.generateMkSScript( 'QMake' )
print fileName.generateMkSScript( 'QMake' )
print uic.generateMkSScript( 'QMake' )
print moc.generateMkSScript( 'QMake' )