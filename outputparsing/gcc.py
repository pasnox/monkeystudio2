# coding=utf8
import sys
import parsing
from string import Template

# message from filename:line,
#      from filename:line,
#      from filename:line:
fromWarning = parsing.Pattern( r"(?:^|\n)\s*((?:[^\n$]+)?\s*from\s*((?:\w+:[\\/])?[^:]+):(\d+))[,:](?:\n|$)",
                                    type = 'warning',
                                    file = '%2',
                                    line = '%3',
                                    text = '%1',
                                    hint = '%1' )

fromWarning.setComment( 'Generic warning message' )

fromWarning.test( "In file included from dependencies/lib/include/../src/encryption/CryptUtil.h:8,\n",
                        type = 'warning',
                        file = 'dependencies/lib/include/../src/encryption/CryptUtil.h',
                        line = '8',
                        text = 'In file included from dependencies/lib/include/../src/encryption/CryptUtil.h:8',
                        hint = 'In file included from dependencies/lib/include/../src/encryption/CryptUtil.h:8' )

fromWarning.test( "from dependencies/lib/include/intuisphere_encryption.h:9,\n",
                        type = 'warning',
                        file = 'dependencies/lib/include/intuisphere_encryption.h',
                        line = '9',
                        text = 'from dependencies/lib/include/intuisphere_encryption.h:9',
                        hint = 'from dependencies/lib/include/intuisphere_encryption.h:9' )

fromWarning.test( "                 from dependencies/lib/include/intuisphere_encryption.h:9,\n",
                        type = 'warning',
                        file = 'dependencies/lib/include/intuisphere_encryption.h',
                        line = '9',
                        text = 'from dependencies/lib/include/intuisphere_encryption.h:9',
                        hint = 'from dependencies/lib/include/intuisphere_encryption.h:9' )

fromWarning.test( "                 from dependencies/lib/include/intuisphere_encryption.h:9:\n",
                        type = 'warning',
                        file = 'dependencies/lib/include/intuisphere_encryption.h',
                        line = '9',
                        text = 'from dependencies/lib/include/intuisphere_encryption.h:9',
                        hint = 'from dependencies/lib/include/intuisphere_encryption.h:9' )

# filename:line: warning: message
# filename:line:column: warning: message
# filename:line: note: message
genericWarning = parsing.Pattern( r"(?:^|\n)((?:\w+:[\\/])?[^:]+):(\d+)(?::\d+)?:\s*(?:warning|note):\s*([^\n$]+)",
                                    type = 'warning',
                                    file = '%1',
                                    line = '%2',
                                    text = '%3' )

genericWarning.setComment( 'Generic warning message' )

genericWarning.test( "src/MSVCMake.cpp:122: warning: unused parameter ‘s’\n",
                        type = 'warning',
                        file = 'src/MSVCMake.cpp',
                        line = '122',
                        text = "unused parameter ‘s’",
                        hint = "src/MSVCMake.cpp:122: warning: unused parameter ‘s’" )

genericWarning.test( "qnetworkrequest.h:93: note: candidates are: QNetworkRequest::QNetworkRequest(const QNetworkRequest&)\n",
                        type = 'warning',
                        file = 'qnetworkrequest.h',
                        line = '93',
                        text = "candidates are: QNetworkRequest::QNetworkRequest(const QNetworkRequest&)",
                        hint = "qnetworkrequest.h:93: note: candidates are: QNetworkRequest::QNetworkRequest(const QNetworkRequest&)" )

genericWarning.test( "src/QMake.h:30:1: note:   because the following virtual functions are pure within ‘QMake’:",
                        type = 'warning',
                        file = 'src/QMake.h',
                        line = '30',
                        text = "because the following virtual functions are pure within ‘QMake’:",
                        hint = "src/QMake.h:30:1: note:   because the following virtual functions are pure within ‘QMake’:")

genericWarning.test( "src\CmlLineManager.cpp:11: warning: unused variable 'z'\n",
                        type = 'warning',
                        file = 'src\CmlLineManager.cpp',
                        line = '11',
                        text = "unused variable 'z'",
                        hint = "src\CmlLineManager.cpp:11: warning: unused variable 'z'" )

genericWarning.test( "c:\src\CmlLineManager.cpp:11: warning: unused variable 'z'\n",
                        type = 'warning',
                        file = 'c:\src\CmlLineManager.cpp',
                        line = '11',
                        text = "unused variable 'z'",
                        hint = "c:\src\CmlLineManager.cpp:11: warning: unused variable 'z'" )

genericWarning.test( 'test.c:4:2: warning: #warning This is bad code!!!\n',
                        type = 'warning',
                        file = 'test.c',
                        line = '4',
                        text = '#warning This is bad code!!!',
                        hint = 'test.c:4:2: warning: #warning This is bad code!!!' )

# filename:line: error: message
# filename:line:column: error: message
# filename:line: fatal error: message
genericError = parsing.Pattern( r"(?:^|\n)((?:\w+:[\\/])?[^:]+):(\d+)(?::\d+)?:\s*(?:fatal)?\s*(?:error):\s*([^\n$]+)",
                                type = 'error',
                                file = '%1',
                                line = '%2',
                                text = '%3' )

genericError.setComment( 'Generic error message' )

genericError.test( "src/main.cpp:20: error: expected initializer before 's'\n",
                    type = 'error',
                    file = 'src/main.cpp',
                    line = '20',
                    text = "expected initializer before 's'",
                    hint = "src/main.cpp:20: error: expected initializer before 's'" )

genericError.test( "src/ui/UIMonkeyProjectSettings.cpp:26: error: expected `;' before 'setWindowTitle'\n",
                    type = 'error',
                    file = 'src/ui/UIMonkeyProjectSettings.cpp',
                    line = '26',
                    text = "expected `;' before 'setWindowTitle'",
                    hint = "src/ui/UIMonkeyProjectSettings.cpp:26: error: expected `;' before 'setWindowTitle'" )

genericError.test( "src/views/TreeView.cpp:20: error: cannot allocate an object of abstract type 'TreeViewModel'\n",
                    type = 'error',
                    file = 'src/views/TreeView.cpp',
                    line = '20',
                    text = "cannot allocate an object of abstract type 'TreeViewModel'",
                    hint = "src/views/TreeView.cpp:20: error: cannot allocate an object of abstract type 'TreeViewModel'" )

genericError.test( "src/SearchWidgetModel.h:6:35: error: AbstractSearchBackend.h: No such file or directory\n",
                    type = 'error',
                    file = 'src/SearchWidgetModel.h',
                    line = '6',
                    text = "AbstractSearchBackend.h: No such file or directory",
                    hint = "src/SearchWidgetModel.h:6:35: error: AbstractSearchBackend.h: No such file or directory" )

genericError.test( "src/gui/pIconManager.cpp:2: fatal error: core/FileSystemUtils.h: No such file or directory\n",
                    type = 'error',
                    file = 'src/gui/pIconManager.cpp',
                    line = '2',
                    text = "core/FileSystemUtils.h: No such file or directory",
                    hint = "src/gui/pIconManager.cpp:2: fatal error: core/FileSystemUtils.h: No such file or directory" )

genericError.test( "z:\src\gui\pIconManager.cpp:2: fatal error: core/FileSystemUtils.h: No such file or directory\n",
                    type = 'error',
                    file = 'z:\src\gui\pIconManager.cpp',
                    line = '2',
                    text = "core/FileSystemUtils.h: No such file or directory",
                    hint = "z:\src\gui\pIconManager.cpp:2: fatal error: core/FileSystemUtils.h: No such file or directory" )

# filename: message
genericWarningNoLine = parsing.Pattern( r"(?:^|\n)(?!(?:mingw32-)?make)((?:\w+:[\\/])?[^:\*\n]+):\s*([^\n$]+)",
                                    type = 'warning',
                                    file = '%1',
                                    text = '%2' )

genericWarningNoLine.setComment( 'Generic warning message' )

genericWarningNoLine.test( "SessionIconDelegate.cpp: In member function 'void SessionIconDelegate::drawExercise(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const':",
                        type = 'warning',
                        file = 'SessionIconDelegate.cpp',
                        text = "In member function 'void SessionIconDelegate::drawExercise(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const':",
                        hint = "SessionIconDelegate.cpp: In member function 'void SessionIconDelegate::drawExercise(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const':" )

# filename:line: message (maybe error aka undefined reference, multiple definition)
genericMessage = parsing.Pattern( r"(?:^|\n)((?:\w+:[\\/])?[^:]+):(\d+)(?::\d+)?:\s*([^\n$]+)",
                                type = 'error',
                                file = '%1',
                                line = '%2',
                                text = '%3' )

genericMessage.setComment( 'Generic message mostly error as i can see' )

genericMessage.test( "/monkey/src/main.cpp:30: undefined reference to `fake()'\n",
                        type = 'error',
                        file = '/monkey/src/main.cpp',
                        line = '30',
                        text = "undefined reference to `fake()'",
                        hint = "/monkey/src/main.cpp:30: undefined reference to `fake()'" )

genericMessage.test( "z:\monkey\src\main.cpp:30: undefined reference to `fake()'\n",
                        type = 'error',
                        file = 'z:\monkey\src\main.cpp',
                        line = '30',
                        text = "undefined reference to `fake()'",
                        hint = "z:\monkey\src\main.cpp:30: undefined reference to `fake()'" )

genericMessage.test( "/xupmanager/ui/UIXUPManager.cpp:313: undefined reference to `pRecentsManager::recentProjectOpenPath() const'\n",
                        type = 'error',
                        file = '/xupmanager/ui/UIXUPManager.cpp',
                        line = '313',
                        text = "undefined reference to `pRecentsManager::recentProjectOpenPath() const'",
                        hint = "/xupmanager/ui/UIXUPManager.cpp:313: undefined reference to `pRecentsManager::recentProjectOpenPath() const'" )

genericMessage.test( "/src/exercisesdialog.h:9: multiple definition of `toto(int, int)'\n",
                        type = 'error',
                        text = "multiple definition of `toto(int, int)'",
                        file = '/src/exercisesdialog.h',
                        line = '9',
                        hint = "/src/exercisesdialog.h:9: multiple definition of `toto(int, int)'" )

# Linking failed, probably undefined reference
link_failed = parsing.Pattern( r"(?:^|\n)collect\d+:\s*ld returned \d+ exit[^\n$]+",
                                type = 'error',
                                text = 'Linking failed. Check Output dock for more info' )

link_failed.setComment( 'Linking failed' )

link_failed.test( 'collect2: ld returned 1 exit status\n',
                    type = 'error',
                    text = 'Linking failed. Check Output dock for more info',
                    hint = 'collect2: ld returned 1 exit status' )

# Cannot find library
no_lib = parsing.Pattern( r"(?:^|\n)(?:\w+:[\\/])?[^:]+:\s*cannot find -l([^\n$]+)",
                            type = 'error',
                            text = 'Cannot find library "%1"' )

no_lib.setComment( 'Linker cannot find library' )

no_lib.test( '/usr/bin/ld: cannot find -lqscintilla2d\n',
                type = 'error',
                text = 'Cannot find library "qscintilla2d"',
                hint = '/usr/bin/ld: cannot find -lqscintilla2d' )

no_lib.test( 'c:\usr\bin\ld.exe: cannot find -lqscintilla2d\n',
                type = 'error',
                text = 'Cannot find library "qscintilla2d"',
                hint = 'c:\usr\bin\ld.exe: cannot find -lqscintilla2d' )

# linking command line
linking = parsing.Pattern( r"(?:^|\n)(?:(?:\w+:[\\/])?[^\s\n]+)?(?:[gc]\+\+|gcc)(?:-[\d\.]+)?\s+-o\s+([^\s\n$]+)\s+[^\n$]+",
                                type = 'action',
                                file = '%1',
                                text = 'Linking %1...' )

linking.setComment( "Linking command line" )

linking.test( 'g++  -o /media/ramdisk/bodeasy/tst_bodeasyd /media/ramdisk/bodeasy/debug/obj/SqlHelper.o /media/ramdisk/bodeasy/debug/obj/sqlkeywords.o -L/usr/lib -lQtTest -L/usr/local/qwt-6.0.1/lib -lqwt -lQtSql -lQtGui -lQtCore -lpthread',
                type = 'action',
                file = '/media/ramdisk/bodeasy/tst_bodeasyd',
                text = 'Linking /media/ramdisk/bodeasy/tst_bodeasyd...',
                hint = 'g++  -o /media/ramdisk/bodeasy/tst_bodeasyd /media/ramdisk/bodeasy/debug/obj/SqlHelper.o /media/ramdisk/bodeasy/debug/obj/sqlkeywords.o -L/usr/lib -lQtTest -L/usr/local/qwt-6.0.1/lib -lqwt -lQtSql -lQtGui -lQtCore -lpthread' )

# building command line
# g++, gcc, mingw32-g++, mingw32-gcc, i386-mingw32-gcc, i386-mingw32-g++, gcc-4.0.0, c++-4.0.0
# g++ .... -o *.o filename
#compiling = parsing.Pattern( r"^(?:[\w\d]+-)?(?:[gc]\+\+|gcc)(?:-[\d\.]+)?\s+[^\n]+\s+-o\s+[^\n]+\s+([^\n]+)",
compiling = parsing.Pattern( r"(?:^|\n)(?:(?:\w+:[\\/])?[^\s\n$]+)?(?:[gc]\+\+|gcc)(?:-[\d\.]+)?\s+[^\n$]+\s+-o\s+[^\n$]+\.o(?:[bjects]+)?\s+([^\n$]+)",
                                type = 'action',
                                text = 'Compiling %1...',
                                file = '%1' )

compiling.setComment( "Building command line" )

text = \
'g++ -c -pipe -g -D_REENTRANT -Wall -W -fPIC -DQT_SHARED -DPLUGIN_NAME="\"MSVCMake\"" -DQT_PLUGIN -DQT_GUI_LIB -DQT_CORE_LIB' + \
' -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui' + \
' -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I../../../qscintilla/QScintilla-gpl-2-snapshot-20071024/Qt4' + \
' -I../../../qscintilla/QScintilla-gpl-2-snapshot-20071024/Qt4/Qsci -I../../../qscintilla/QScintilla-gpl-2-snapshot-20071024/include' + \
' -I../../../qscintilla/QScintilla-gpl-2-snapshot-20071024/src -I../../../fresh -I../../../fresh/objects -I../../../fresh/widgets' + \
' -I../../../ctags/ctags -I../../../monkey -I../../../monkey -I../../../monkey/src -I. -Isrc -I../../../monkey/src/maininterface' + \
' -I../../../monkey/src/maininterface/ui -I../../../monkey/src/workspace -I../../../monkey/src/recentsmanager' + \
' -I../../../monkey/src/qscintillamanager -I../../../monkey/src/qscintillamanager/ui -I../../../monkey/src/ctags manager' + \
' -I../../../monkey/src/variablesmanager -I../../../monkey/src/templatesmanager -I../../../monkey/src/templatesmanager/ui' + \
' -I../../../monkey/src/abbreviationsmanager -I../../../monkey/src/abbreviationsmanager/ui -I../../../monkey/src/toolsmanager' + \
' -I../../../monkey/src/toolsmanager/ui -I../../../monkey/src/consolemanager -I/home/a/code/monkeyrepos/v2/trunk/monkey/src/consolemanager/ui' + \
' -I../../../monkey/src/pluginsmanager -I../../../monkey/src/pluginsmanager/ui -I../../../monkey/src/projectsmanager' + \
' -I../../../monkey/src/projectsmanager/ui -I/usr/X11R6/include -I../../../build/debug/.moc -I../../../build/debug/.ui' + \
' -o ../../../build/debug/.obj/unix/MSVCMake.o src/MSVCMake.cpp\n'
compiling.test(text,
                    type = 'action',
                    text = 'Compiling src/MSVCMake.cpp...',
                    hint = text[:-1],
                    file = 'src/MSVCMake.cpp')
text = \
'/usr/local/i386-mingw32-3.4.5/bin/i386-mingw32-gcc -c -O2 -Wall -DUNICODE -DQT_LARGEFILE_SUPPORT -DLARGE_FILES '+ \
'-D_FILE_OFFSET_BITS=64 -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -DQT_NO_DYNAMIC_CAST ' + \
'-I"/usr/local/Trolltech/win32/4.6.0/include/QtCore" -I"/usr/local/Trolltech/win32/4.6.0/include/QtGui" ' + \
'-I"/usr/local/Trolltech/win32/4.6.0/include" -I"." -I"/Users/pasnox/Win32Libraries/include" -I"libwbfs" ' + \
'-I"/usr/local/Trolltech/win32/4.6.0/include/ActiveQt" -I"../build/release/moc" ' + \
'-I"/usr/local/i386-mingw32-3.4.5/i386-mingw32/include" -I"../../../intuisphere/trunk/mkspecs/4.6.x/win32-osx-g++" ' + \
'-o ../build/release/obj/win32/tools.o tools.c\n'
compiling.test(text,
                    type = 'action',
                    text = 'Compiling tools.c...',
                    hint = text[:-1],
                    file = 'tools.c')
#g++
text = "g++ -c -pipe -Wall -g -Wall -W -D_REENTRANT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Ieditor-src -Ieditor-src/branche1 -Ieditor-src -I/branche2 -Ibuild/debug/.moc -Ibuild/debug/.ui -o build/debug/.obj/unix/SourceFile.o editor-src/branche2/SourceFile.cpp"
compiling.test(text,
                    type = 'action',
                    text = 'Compiling editor-src/branche2/SourceFile.cpp...',
                    hint = text,
                    file = 'editor-src/branche2/SourceFile.cpp')
#gcc
text = "gcc -c -pipe -Wall -g -Wall -W -D_REENTRANT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Ieditor-src -Ieditor-src/branche1 -Ieditor-src -I/branche2 -Ibuild/debug/.moc -Ibuild/debug/.ui -o build/debug/.obj/unix/SourceFile.o editor-src/branche2/SourceFile.cpp"
compiling.test(text,
                    type = 'action',
                    text = 'Compiling editor-src/branche2/SourceFile.cpp...',
                    hint = text,
                    file = 'editor-src/branche2/SourceFile.cpp')

# it is windows specific
text = 'g++ -c -g -Wall -frtti -fexceptions -mthreads -DPACKAGE_NAME="\"Mirrorad\"" -DQT_DLL -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include" -I"src" -I"src\ie" -I"src\ff" -I"src\ui" -I"src\opera" -I"c:\Development\Qt\4.4.3\include\ActiveQt" -I"build\debug\.moc" -I"build\debug\.ui" -I"..\..\Qt\4.4.3\mkspecs\win32-g++" -o build\debug\.obj\win32\main.o src\main.cpp\n'
compiling.test( text,
                        type = 'action',
                        text = 'Compiling src\main.cpp...',
                        file = 'src\main.cpp',
                        hint = text[:-1])
# it is windows specific
text = 'mingw32-gcc -c -g -Wall -frtti -fexceptions -mthreads -DPACKAGE_NAME="\"Mirrorad\"" -DQT_DLL -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include" -I"src" -I"src\ie" -I"src\ff" -I"src\ui" -I"src\opera" -I"c:\Development\Qt\4.4.3\include\ActiveQt" -I"build\debug\.moc" -I"build\debug\.ui" -I"..\..\Qt\4.4.3\mkspecs\win32-g++" -o build\debug\.obj\win32\main.o src\main.cpp\n'
compiling.test( text,
                        type = 'action',
                        text = 'Compiling src\main.cpp...',
                        file = 'src\main.cpp',
                        hint = text[:-1])
# Mac, compiling file
text = 'g++-4.0 -c -pipe -g -gdwarf-2 -arch i386 -Wall -W -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/local/Trolltech/Qt-4.6.0-universal/mkspecs/macx-g++ -I. -I/usr/local/Trolltech/Qt-4.6.0-universal/lib/QtCore.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.6.0-universal/include/QtCore -I/usr/local/Trolltech/Qt-4.6.0-universal/lib/QtGui.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.6.0-universal/include/QtGui -I/usr/local/Trolltech/Qt-4.6.0-universal/include -Ibuild/debug/.moc -Ibuild/debug/.ui -F/usr/local/Trolltech/Qt-4.6.0-universal/lib -o build/debug/.obj/mac/UIMarketOptions.o src/UIMarketOptions.cpp'
compiling.test( text,
                        type = 'action',
                        text = 'Compiling src/UIMarketOptions.cpp...',
                        file = 'src/UIMarketOptions.cpp',
                        hint = text)

# Generation of script file
print '# It is a machine generated file. Do not edit it manualy!'
print ''
print fromWarning.generateMkSScript( 'GCC' )
print genericWarning.generateMkSScript( 'GCC' )
print genericError.generateMkSScript( 'GCC' )
print genericWarningNoLine.generateMkSScript( 'GCC' )
print genericMessage.generateMkSScript( 'GCC' )
print link_failed.generateMkSScript( 'GCC' )
print no_lib.generateMkSScript( 'GCC' )
print compiling.generateMkSScript( 'GCC' )
print linking.generateMkSScript( 'GCC' )