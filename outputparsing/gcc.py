# coding=utf8

import sys

import parsing
from string import Template

file_name = r"[\w\d\./\\\-]+"
number = "\d+"
#compiler = Template("(gcc|g\+\+|mingw32-gcc|mingw32-g\+\+)[\-\d\.]*") # /usr/local/i386-mingw32-3.4.5/bin/i386-mingw32-gcc-4.0
compiler = Template("($file_name)?(gcc|g\+\+|mingw32-gcc|mingw32-g\+\+)[\-\d\.]*").substitute(file_name=file_name)
source_file = file_name + "\.[cpmh]{1,3}" # .cpp .c .h .m .mm

## Error
regEx = Template("^($file_name):($number):[\d:]* ((fatal )?error:|undefined reference) [^\\n]+")
error =parsing.Pattern(regEx.substitute(file_name = file_name, number = number),
								   type = 'error',
								   file = "%1",
								   line = "%2")

error.setComment('Error (usualy syntax)')

error.test("src/main.cpp:20: error: expected initializer before 's'",
				type = 'error',
				file = 'src/main.cpp',
				line = '20',
				text = "src/main.cpp:20: error: expected initializer before 's'",
				hint = "src/main.cpp:20: error: expected initializer before 's'")

error.test("/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/main.cpp:30: undefined reference to `fake()'\n",
				type = 'error',
				file = '/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/main.cpp',
				line = '30',
				text = "/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/main.cpp:30: undefined reference to `fake()'",
				hint = "/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/main.cpp:30: undefined reference to `fake()'")

error.test("src/ui/UIMonkeyProjectSettings.cpp:26: error: expected `;' before 'setWindowTitle'\n",
				type = 'error',
				file = 'src/ui/UIMonkeyProjectSettings.cpp',
				line = '26',
				text = "src/ui/UIMonkeyProjectSettings.cpp:26: error: expected `;' before 'setWindowTitle'",
				hint = "src/ui/UIMonkeyProjectSettings.cpp:26: error: expected `;' before 'setWindowTitle'")

text = "/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/xupmanager/ui/UIXUPManager.cpp:313: undefined reference to `pRecentsManager::recentProjectOpenPath() const'\n"
error.test(	text,
					type = 'error',
					file = '/home/a/code/monkeyrepos/v2/branches/dev/monkey/src/xupmanager/ui/UIXUPManager.cpp',
					line = '313',
					text = text[:-1],
					hint = text[:-1])

text = "src/views/TreeView.cpp:20: error: cannot allocate an object of abstract type 'TreeViewModel'\n"
error.test(	text,
					type = 'error',
					file = 'src/views/TreeView.cpp',
					line = '20',
					text = text[:-1],
					hint = text[:-1])

text = "src/SearchWidgetModel.h:6:35: error: AbstractSearchBackend.h: No such file or directory\n"
error.test(	text,
					type = 'error',
					file = 'src/SearchWidgetModel.h',
					line = '6',
					text = text[:-1],
					hint = text[:-1])

text = "src/gui/pIconManager.cpp:2: fatal error: core/FileSystemUtils.h: No such file or directory\n"
error.test(	text,
					type = 'error',
					file = 'src/gui/pIconManager.cpp',
					line = '2',
					text = text[:-1],
					hint = text[:-1])

## Warning or Note
regEx = Template("^($file_name):($number): (warning|note): [^\\n]+")
warning =parsing.Pattern(regEx.substitute(file_name = file_name, number = number),
								   type = 'warning',
								   file = "%1",
								   line = "%2")

warning.setComment('Warning or Note')

warning.test("src/MSVCMake.cpp:122: warning: unused parameter ‘s’\n",
				type = 'warning',
				file = 'src/MSVCMake.cpp',
				line = '122',
				text = "src/MSVCMake.cpp:122: warning: unused parameter ‘s’",
				hint = "src/MSVCMake.cpp:122: warning: unused parameter ‘s’")

warning.test("qnetworkrequest.h:93: note: candidates are: QNetworkRequest::QNetworkRequest(const QNetworkRequest&)\n",
				type = 'warning',
				file = 'qnetworkrequest.h',
				line = '93',
				text = "qnetworkrequest.h:93: note: candidates are: QNetworkRequest::QNetworkRequest(const QNetworkRequest&)",
				hint = "qnetworkrequest.h:93: note: candidates are: QNetworkRequest::QNetworkRequest(const QNetworkRequest&)")

# windows specific
warning.test("src\CmlLineManager.cpp:11: warning: unused variable 'z'\n",
					 type = 'warning',
					 file = 'src\CmlLineManager.cpp',
					 line = '11',
					 text = "src\CmlLineManager.cpp:11: warning: unused variable 'z'",
					 hint = "src\CmlLineManager.cpp:11: warning: unused variable 'z'")

## Multiply definition

regEx = Template("^($file_name):($number): (multiple definition of [^\\n]+)")
multipleDefinition = parsing.Pattern(regEx.substitute(file_name = file_name, number = number),
									 type = 'error',
									 file = "%1",
									 line = "%2",
									 text = "%3")
multipleDefinition.setComment('#warning preprocessor dirrective')
multipleDefinition.test("/home/pasnox/Development/Qt4/bodeasy/src/exercisesdialog.h:9: multiple definition of `toto(int, int)'\n",
						type = 'error',
						text = "multiple definition of `toto(int, int)'",
						file = '/home/pasnox/Development/Qt4/bodeasy/src/exercisesdialog.h',
						line = '9',
						hint = "/home/pasnox/Development/Qt4/bodeasy/src/exercisesdialog.h:9: multiple definition of `toto(int, int)'")

## Linking failed, probably undefined reference
regEx = "^collect2: ld returned 1 exit status"
link_failed =parsing.Pattern(regEx,
									type = 'error',
									text = 'Linking failed. Check Output dock for more info')

link_failed.setComment('Linking failed')
link_failed.test('collect2: ld returned 1 exit status\n',
				 type = 'error',
				 text = 'Linking failed. Check Output dock for more info',
				 hint = 'collect2: ld returned 1 exit status')

## Building file
regEx = Template("^$compiler[^\\n]+ ($source_file)")
compiling =parsing.Pattern(regEx.substitute(compiler=compiler, source_file=source_file),
										   type = 'compiling',
										   text = 'Compiling %3...',
										   file = '%3')

compiling.setComment('Compiling')

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
					type = 'compiling',
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
					type = 'compiling',
					text = 'Compiling tools.c...',
					hint = text[:-1],
					file = 'tools.c')
#g++
text = "g++ -c -pipe -Wall -g -Wall -W -D_REENTRANT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Ieditor-src -Ieditor-src/branche1 -Ieditor-src -I/branche2 -Ibuild/debug/.moc -Ibuild/debug/.ui -o build/debug/.obj/unix/SourceFile.o editor-src/branche2/SourceFile.cpp"
compiling.test(text,
					type = 'compiling',
					text = 'Compiling editor-src/branche2/SourceFile.cpp...',
					hint = text,
					file = 'editor-src/branche2/SourceFile.cpp')
#gcc
text = "gcc -c -pipe -Wall -g -Wall -W -D_REENTRANT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Ieditor-src -Ieditor-src/branche1 -Ieditor-src -I/branche2 -Ibuild/debug/.moc -Ibuild/debug/.ui -o build/debug/.obj/unix/SourceFile.o editor-src/branche2/SourceFile.cpp"
compiling.test(text,
					type = 'compiling',
					text = 'Compiling editor-src/branche2/SourceFile.cpp...',
					hint = text,
					file = 'editor-src/branche2/SourceFile.cpp')

# it is windows specific
text = 'g++ -c -g -Wall -frtti -fexceptions -mthreads -DPACKAGE_NAME="\"Mirrorad\"" -DQT_DLL -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include" -I"src" -I"src\ie" -I"src\ff" -I"src\ui" -I"src\opera" -I"c:\Development\Qt\4.4.3\include\ActiveQt" -I"build\debug\.moc" -I"build\debug\.ui" -I"..\..\Qt\4.4.3\mkspecs\win32-g++" -o build\debug\.obj\win32\main.o src\main.cpp\n'
compiling.test(	text,
						type = 'compiling',
						text = 'Compiling src\main.cpp...',
						file = 'src\main.cpp',
						hint = text[:-1])
# it is windows specific
text = 'mingw32-gcc -c -g -Wall -frtti -fexceptions -mthreads -DPACKAGE_NAME="\"Mirrorad\"" -DQT_DLL -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include\QtCore" -I"..\..\Qt\4.4.3\include" -I"src" -I"src\ie" -I"src\ff" -I"src\ui" -I"src\opera" -I"c:\Development\Qt\4.4.3\include\ActiveQt" -I"build\debug\.moc" -I"build\debug\.ui" -I"..\..\Qt\4.4.3\mkspecs\win32-g++" -o build\debug\.obj\win32\main.o src\main.cpp\n'
compiling.test(	text,
						type = 'compiling',
						text = 'Compiling src\main.cpp...',
						file = 'src\main.cpp',
						hint = text[:-1])
# Mac, compiling file
text = 'g++-4.0 -c -pipe -g -gdwarf-2 -arch i386 -Wall -W -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/local/Trolltech/Qt-4.6.0-universal/mkspecs/macx-g++ -I. -I/usr/local/Trolltech/Qt-4.6.0-universal/lib/QtCore.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.6.0-universal/include/QtCore -I/usr/local/Trolltech/Qt-4.6.0-universal/lib/QtGui.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.6.0-universal/include/QtGui -I/usr/local/Trolltech/Qt-4.6.0-universal/include -Ibuild/debug/.moc -Ibuild/debug/.ui -F/usr/local/Trolltech/Qt-4.6.0-universal/lib -o build/debug/.obj/mac/UIMarketOptions.o src/UIMarketOptions.cpp'
compiling.test(	text,
						type = 'compiling',
						text = 'Compiling src/UIMarketOptions.cpp...',
						file = 'src/UIMarketOptions.cpp',
						hint = text)


# Cannot find library
regEx = "^%sld: cannot find -l([\w]+)" % file_name
no_lib =parsing.Pattern(regEx,
									type = 'error',
									text = 'Cannot find library "%1"')

no_lib.setComment('Linker cannot find library')
no_lib.test('/usr/bin/ld: cannot find -lqscintilla2d\n',
				 type = 'error',
				 text = 'Cannot find library "qscintilla2d"',
				 hint = '/usr/bin/ld: cannot find -lqscintilla2d')


#   #warning blablabla...
regEx = "^(%s):(\d+):\d+: warning: #warning ([^\\n]*)" % file_name
preprocessor_warning = parsing.Pattern(regEx,
									type = 'warning',
									file = '%1',
									line = '%2',
									text = '%3')

preprocessor_warning.setComment('#warning preprocessor dirrective')
preprocessor_warning.test('test.c:4:2: warning: #warning This is bad code!!!\n',
				 type = 'warning',
				 text = 'This is bad code!!!',
				 file = 'test.c',
				 line = '4',
				 hint = 'test.c:4:2: warning: #warning This is bad code!!!')

print '# It is a machine generated file. Do not edit it manualy!\n'

print error.generateMkSScript('GCC')
print warning.generateMkSScript('GCC')
print multipleDefinition.generateMkSScript('GCC')
print compiling.generateMkSScript('GCC')
print no_lib.generateMkSScript('GCC')
print link_failed.generateMkSScript('GCC')
print preprocessor_warning.generateMkSScript('GCC')
