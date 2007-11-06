# Coverage meter

CoverageMeter {
	# Configuration for code coverage using 
	# CoverageMeter (http://www.coveragemeter.com)
	QMAKE_CXX	= cs$$QMAKE_CXX
	QMAKE_LINK	= cs$$QMAKE_LINK
	QMAKE_CC	= cs$$QMAKE_CC
}

INCLUDEPATH	+= coveragemeter \
	coveragemeter/ui