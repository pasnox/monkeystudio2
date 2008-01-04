# The project file for the QScintilla library.
#
# Copyright (c) 2007
# 	Phil Thompson <phil@river-bank.demon.co.uk>
# 
# This file is part of QScintilla.
# 
# This copy of QScintilla is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option) any
# later version.
# 
# QScintilla is supplied in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
# 
# You should have received a copy of the GNU General Public License along with
# QScintilla; see the file LICENSE.  If not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


!win32:VERSION = 3.0.0

TEMPLATE = lib
TARGET = qscintilla2
CONFIG += qt warn_off release dll thread
INCLUDEPATH = . ../include ../src
DEFINES = QSCINTILLA_MAKE_DLL QT SCI_LEXER

# Handle both Qt v4 and v3.
target.path = $$[QT_INSTALL_LIBS]
isEmpty(target.path) {
	target.path = $(QTDIR)/lib
}

header.path = $$[QT_INSTALL_HEADERS]
header.files = Qsci
isEmpty(header.path) {
	header.path = $(QTDIR)/include/Qsci
	header.files = Qsci/qsci*.h
}

trans.path = $$[QT_INSTALL_TRANSLATIONS]
trans.files = qscintilla_*.qm
isEmpty(trans.path) {
	trans.path = $(QTDIR)/translations
}

qsci.path = $$[QT_INSTALL_DATA]
qsci.files = ../qsci
isEmpty(qsci.path) {
	qsci.path = $(QTDIR)
}

INSTALLS += header trans qsci target

HEADERS = \
	./Qsci/qsciglobal.h \
	./Qsci/qsciscintilla.h \
	./Qsci/qsciscintillabase.h \
	./Qsci/qsciapis.h \
	./Qsci/qscicommand.h \
	./Qsci/qscicommandset.h \
	./Qsci/qscidocument.h \
	./Qsci/qscilexer.h \
	./Qsci/qscilexerbash.h \
	./Qsci/qscilexerbatch.h \
	./Qsci/qscilexercmake.h \
	./Qsci/qscilexercpp.h \
	./Qsci/qscilexercsharp.h \
	./Qsci/qscilexercss.h \
	./Qsci/qscilexerd.h \
	./Qsci/qscilexerdiff.h \
	./Qsci/qscilexerhtml.h \
	./Qsci/qscilexeridl.h \
	./Qsci/qscilexerjava.h \
	./Qsci/qscilexerjavascript.h \
	./Qsci/qscilexerlua.h \
	./Qsci/qscilexermakefile.h \
	./Qsci/qscilexerperl.h \
	./Qsci/qscilexerpov.h \
	./Qsci/qscilexerproperties.h \
	./Qsci/qscilexerpython.h \
	./Qsci/qscilexerruby.h \
	./Qsci/qscilexersql.h \
	./Qsci/qscilexertex.h \
	./Qsci/qscilexervhdl.h \
	./Qsci/qscimacro.h \
	./Qsci/qsciprinter.h \
	ListBoxQt.h \
	SciClasses.h \
	ScintillaQt.h \
	../include/Accessor.h \
	../include/KeyWords.h \
	../include/Platform.h \
	../include/PropSet.h \
	../include/SString.h \
	../include/SciLexer.h \
	../include/Scintilla.h \
	../include/ScintillaWidget.h \
	../include/WindowAccessor.h \
	../src/AutoComplete.h \
	../src/CallTip.h \
	../src/CellBuffer.h \
	../src/CharClassify.h \
	../src/ContractionState.h \
	../src/Decoration.h \
	../src/Document.h \
	../src/DocumentAccessor.h \
	../src/Editor.h \
	../src/ExternalLexer.h \
	../src/Indicator.h \
	../src/KeyMap.h \
	../src/LineMarker.h \
	../src/Partitioning.h \
	../src/PositionCache.h \
	../src/RESearch.h \
	../src/RunStyles.h \
	../src/SVector.h \
	../src/ScintillaBase.h \
	../src/SplitVector.h \
	../src/Style.h \
	../src/StyleContext.h \
	../src/ViewStyle.h \
	../src/XPM.h

SOURCES = \
	qsciscintilla.cpp \
	qsciscintillabase.cpp \
	qsciapis.cpp \
	qscicommand.cpp \
	qscicommandset.cpp \
	qscidocument.cpp \
	qscilexer.cpp \
	qscilexerbash.cpp \
	qscilexerbatch.cpp \
	qscilexercmake.cpp \
	qscilexercpp.cpp \
	qscilexercsharp.cpp \
	qscilexercss.cpp \
	qscilexerd.cpp \
	qscilexerdiff.cpp \
	qscilexerhtml.cpp \
	qscilexeridl.cpp \
	qscilexerjava.cpp \
	qscilexerjavascript.cpp \
	qscilexerlua.cpp \
	qscilexermakefile.cpp \
	qscilexerperl.cpp \
	qscilexerpov.cpp \
	qscilexerproperties.cpp \
	qscilexerpython.cpp \
	qscilexerruby.cpp \
	qscilexersql.cpp \
	qscilexertex.cpp \
	qscilexervhdl.cpp \
	qscimacro.cpp \
	qsciprinter.cpp \
	SciClasses.cpp \
	ListBoxQt.cpp \
	PlatQt.cpp \
	ScintillaQt.cpp \
	../src/AutoComplete.cpp \
	../src/CallTip.cpp \
	../src/CellBuffer.cpp \
	../src/CharClassify.cpp \
	../src/ContractionState.cpp \
	../src/Decoration.cpp \
	../src/Document.cpp \
	../src/DocumentAccessor.cpp \
	../src/Editor.cpp \
	../src/ExternalLexer.cpp \
	../src/Indicator.cpp \
        ../src/KeyMap.cpp \
	../src/KeyWords.cpp \
	../src/LexAbaqus.cpp \
	../src/LexAPDL.cpp \
	../src/LexAU3.cpp \
	../src/LexAVE.cpp \
	../src/LexAda.cpp \
	../src/LexAsm.cpp \
	../src/LexAsn1.cpp \
	../src/LexASY.cpp \
	../src/LexBaan.cpp \
	../src/LexBash.cpp \
	../src/LexBasic.cpp \
	../src/LexBullant.cpp \
	../src/LexCLW.cpp \
	../src/LexCPP.cpp \
	../src/LexCSS.cpp \
	../src/LexCaml.cpp \
	../src/LexCmake.cpp \
	../src/LexConf.cpp \
	../src/LexCrontab.cpp \
	../src/LexCsound.cpp \
	../src/LexD.cpp \
	../src/LexEScript.cpp \
	../src/LexEiffel.cpp \
	../src/LexErlang.cpp \
	../src/LexFlagship.cpp \
	../src/LexForth.cpp \
	../src/LexFortran.cpp \
	../src/LexGAP.cpp \
	../src/LexGui4Cli.cpp \
	../src/LexHTML.cpp \
	../src/LexHaskell.cpp \
	../src/LexInno.cpp \
	../src/LexKix.cpp \
	../src/LexLisp.cpp \
	../src/LexLout.cpp \
	../src/LexLua.cpp \
	../src/LexMMIXAL.cpp \
	../src/LexMPT.cpp \
	../src/LexMSSQL.cpp \
	../src/LexMatlab.cpp \
	../src/LexMetapost.cpp \
	../src/LexNsis.cpp \
	../src/LexOpal.cpp \
	../src/LexOthers.cpp \
	../src/LexPB.cpp \
	../src/LexPLM.cpp \
	../src/LexPOV.cpp \
	../src/LexProgress.cpp \
	../src/LexPS.cpp \
	../src/LexPascal.cpp \
	../src/LexPerl.cpp \
	../src/LexPython.cpp \
	../src/LexR.cpp \
	../src/LexRebol.cpp \
	../src/LexRuby.cpp \
	../src/LexSQL.cpp \
	../src/LexScriptol.cpp \
	../src/LexSmalltalk.cpp \
	../src/LexSpecman.cpp \
	../src/LexSpice.cpp \
	../src/LexTADS3.cpp \
	../src/LexTCL.cpp \
	../src/LexTeX.cpp \
	../src/LexVB.cpp \
	../src/LexVHDL.cpp \
	../src/LexVerilog.cpp \
	../src/LexYAML.cpp \
	../src/LineMarker.cpp \
	../src/PositionCache.cpp \
	../src/PropSet.cpp \
        ../src/RESearch.cpp \
	../src/RunStyles.cpp \
        ../src/ScintillaBase.cpp \
	../src/Style.cpp \
	../src/StyleContext.cpp \
	../src/ViewStyle.cpp \
	../src/WindowAccessor.cpp \
	../src/XPM.cpp

TRANSLATIONS = \
	qscintilla_cs.ts \
	qscintilla_de.ts \
	qscintilla_fr.ts \
	qscintilla_pt_br.ts \
	qscintilla_ru.ts
