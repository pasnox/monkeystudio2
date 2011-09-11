import parsing

# parser add <name> <regular expression> <file name> <column> <row> <pattern type> <pattern text> <full text>
# def __init__(self, pattern, file = '', line = '-1', column = '-1', type = 'error', text = '%0', hint = '%0'):

# WARNING: Failure to find
missingFile = parsing.Pattern( r"^WARNING:\s+(Failure to find:\s+([^\n]+))", file = '%2', type = 'error', text = '%1' )
missingFile.setComment( 'WARNING: Failure to find file' )

missingFile.test( 'WARNING: Failure to find: file.ext',
					file = 'file.ext',
					type = 'error',
					text = 'Failure to find: file.ext',
					hint = 'WARNING: Failure to find: file.ext' )

# Project MESSAGE
message = parsing.Pattern( r"^Project MESSAGE:\s+([^\n]+)", type = 'warning', text = '%1' )
message.setComment( 'Project MESSAGE' )

message.test( 'Project MESSAGE: this is my qmake project message',
				type = 'warning',
				text = 'this is my qmake project message',
				hint = 'Project MESSAGE: this is my qmake project message' )

# Reading
reading = parsing.Pattern( r"^\s*Reading\s+([^\n]+)", file = '%1', type = 'warning' )
reading.setComment( 'Reading' )

reading.test( 'Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
				file = '/home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
				type = 'warning',
				text = 'Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
				hint = 'Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro' )

# Warning: potential duplicate alias detected
duplicateAlias = parsing.Pattern( r"^([^\n]+):\s+Warning:\s+(potential duplicate alias detected:\s+'[^\n]+')", file = '%1', type = 'warning', text = '%2' )
duplicateAlias.setComment( 'Warning: potential duplicate alias detected' )

duplicateAlias.test( 'src/resources/resources.qrc: Warning: potential duplicate alias detected: \'license.gpl\'',
						file = 'src/resources/resources.qrc',
						type = 'warning',
						text = 'potential duplicate alias detected: \'license.gpl\'',
						hint = 'src/resources/resources.qrc: Warning: potential duplicate alias detected: \'license.gpl\'' )

# lupdate warning: no TS files specified.
noTS = parsing.Pattern( r"^lupdate warning:\s+(no TS files specified\. Only diagnostics will be produced for '([^\n]+)')\.", file = '%2', type = 'warning', text = '%1' )
noTS.setComment( 'lupdate warning: no TS files specified' )

noTS.test( 'lupdate warning: no TS files specified. Only diagnostics will be produced for \'/home/pasnox/Temporaire/qtgui/qtgui.pro\'.',
				file = '/home/pasnox/Temporaire/qtgui/qtgui.pro',
				type = 'warning',
				text = 'no TS files specified. Only diagnostics will be produced for \'/home/pasnox/Temporaire/qtgui/qtgui.pro\'',
				hint = 'lupdate warning: no TS files specified. Only diagnostics will be produced for \'/home/pasnox/Temporaire/qtgui/qtgui.pro\'.' )

# Function is not implemented
noFunction = parsing.Pattern( r"^([^\n]+)\((\d+)\):Function '[^\n]+' is not implemented", file = '%1', type = 'warning', line = '%2' )
noFunction.setComment( 'Function is not implemented' )

noFunction.test( '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):Function \'fromfile\' is not implemented',
				file = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri',
				type = 'warning',
				line = '7',
				text = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):Function \'fromfile\' is not implemented',
				hint = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):Function \'fromfile\' is not implemented' )

# Test function is not a recognized
noTestFunction = parsing.Pattern( r"^([^\n]+)\((\d+)\):'[^\n]+' is not a recognized test function", file = '%1', type = 'warning', line = '%2' )
noTestFunction.setComment( 'Test function is not recognized' )

noTestFunction.test( '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):\'!contains\' is not a recognized test function',
						file = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri',
						type = 'warning',
						line = '7',
						text = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):\'!contains\' is not a recognized test function',
						hint = '/usr/share/qt4/mkspecs/modules/qt_phonon.pri(7):\'!contains\' is not a recognized test function' )

# Circular inclusion
circularInclusion = parsing.Pattern( r"^([^\n]+):(\d+): circular inclusion of [^\n]+", file = '%1', type = 'warning', line = '%2' )
circularInclusion.setComment( 'Circular inclusion' )

circularInclusion.test( '/usr/include/qt4/QtCore/qstringbuilder.h:45: circular inclusion of /usr/include/qt4/QtCore/qstring.h',
							file = '/usr/include/qt4/QtCore/qstringbuilder.h',
							type = 'warning',
							line = '45',
							text = '/usr/include/qt4/QtCore/qstringbuilder.h:45: circular inclusion of /usr/include/qt4/QtCore/qstring.h',
							hint = '/usr/include/qt4/QtCore/qstringbuilder.h:45: circular inclusion of /usr/include/qt4/QtCore/qstring.h' )


# Generation of script file
print '# It is a machine generated file. Do not edit it manualy!'
print ''
print missingFile.generateMkSScript( 'QMake' )
print message.generateMkSScript( 'QMake' )
print reading.generateMkSScript( 'QMake' )
print duplicateAlias.generateMkSScript( 'QMake' )
print noTS.generateMkSScript( 'QMake' )
print noFunction.generateMkSScript( 'QMake' )
print noTestFunction.generateMkSScript( 'QMake' )
print circularInclusion.generateMkSScript( 'QMake' )
