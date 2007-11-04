/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pFormatterSettings.cpp
** Date      : 2007-11-04T22:50:31
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "pFormatterSettings.h"
#include "pSettings.h"

void pFormatterSettings::applyTo( astyle::ASFormatter& f )
{
	QSettings* s = pSettings::instance();
	s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
	int style = s->value( "style", 0 ).toInt();
	switch( style )
	{
	case 0: // ansi
		f.bracketIndent = false;
		f.indentLength = 4;
		f.indentString = "    ";
		f.bracketFormatMode = astyle::BREAK_MODE;
		f.classIndent = false;
		f.switchIndent = false;
		f.namespaceIndent = true;
		f.blockIndent = false;
		f.breakBlocks = false;
		f.breakElseIfs = false;
		f.padOperators = false;
		f.padParen = false;
		f.breakOneLineStatements = true;
		f.breakOneLineBlocks = true;
		break;
	case 1: // K&R
		f.bracketIndent = false;
		f.indentLength = 4;
		f.indentString = "    ";
		f.bracketFormatMode = astyle::ATTACH_MODE;
		f.classIndent = false;
		f.switchIndent = false;
		f.namespaceIndent = true;
		f.blockIndent = false;
		f.breakBlocks = false;
		f.breakElseIfs = false;
		f.padOperators = false;
		f.padParen = false;
		f.breakOneLineStatements = true;
		f.breakOneLineBlocks = true;
		break;
	case 2: // Linux
		f.bracketIndent = false;
		f.indentLength = 8;
		f.indentString = "        ";
		f.bracketFormatMode = astyle::BDAC_MODE;
		f.classIndent = false;
		f.switchIndent = false;
		f.namespaceIndent = true;
		f.blockIndent = false;
		f.breakBlocks = false;
		f.breakElseIfs = false;
		f.padOperators = false;
		f.padParen = false;
		f.breakOneLineStatements = true;
		f.breakOneLineBlocks = true;
		break;
	case 3: // GNU
		f.blockIndent = true;
		f.bracketIndent = false;
		f.indentLength = 2;
		f.indentString = "  ";
		f.bracketFormatMode = astyle::BREAK_MODE;
		f.classIndent = false;
		f.switchIndent = false;
		f.namespaceIndent = false;
		f.breakBlocks = false;
		f.breakElseIfs = false;
		f.padOperators = false;
		f.padParen = false;
		f.breakOneLineStatements = true;
		f.breakOneLineBlocks = true;
		break;
	case 4: // Java
		f.sourceStyle = astyle::STYLE_JAVA;
		f.modeSetManually = true;
		f.bracketIndent = false;
		f.indentLength = 4;
		f.indentString = "    ";
		f.bracketFormatMode = astyle::ATTACH_MODE;
		f.switchIndent = false;
		f.blockIndent = false;
		f.breakBlocks = false;
		f.breakElseIfs = false;
		f.padOperators = false;
		f.padParen = false;
		f.breakOneLineStatements = true;
		f.breakOneLineBlocks = true;
		break;
	case 5: //Runtime
		f.blockIndent = false;
		f.bracketIndent = false;
		f.indentLength = 4;
		f.indentString = '\t';
		f.switchIndent = true;
		f.caseIndent = false;
		f.bracketFormatMode = astyle::NONE_MODE;
		break;
	default: // Custom
		{
			int spaceNum = s->value( "indentation", 4 ).toInt();
			//
			f.modeSetManually = false;
			f.indentLength = spaceNum;
			//
			f.indentString = '\t';
			if ( !s->value( "use_tabs", false ).toBool() )
				f.indentString = string( spaceNum, ' ' );
			//
			f.forceTabIndent = false;
			if ( s->value( "force_tabs", false ).toBool() )
			{
				f.indentString = '\t';
				f.forceTabIndent = true;
			}
			//
			f.convertTabs2Space = s->value( "convert_tabs", false ).toBool();
			f.emptyLineIndent = s->value( "fill_empty_lines", false ).toBool();
			f.classIndent = s->value( "indent_classes", false ).toBool();
			f.switchIndent = s->value( "indent_switches", false ).toBool();
			f.caseIndent = s->value( "indent_case", false ).toBool();
			f.bracketIndent = s->value( "indent_brackets", false ).toBool();
			f.blockIndent = s->value( "indent_blocks", false ).toBool();
			f.namespaceIndent = s->value( "indent_namespaces", false ).toBool();
			f.labelIndent = s->value( "indent_labels", false ).toBool();
			f.preprocessorIndent = s->value( "indent_preprocessor", false ).toBool();
			//
			int breakType = s->value( "break_type", 0 ).toInt();
			switch( breakType )
			{
			case 1: //break
				f.bracketFormatMode = astyle::BREAK_MODE;
				break;
			case 2: //attach
				f.bracketFormatMode = astyle::ATTACH_MODE;
				break;
			case 3: //linux
				f.bracketFormatMode = astyle::BDAC_MODE;
				break;
			default: //none
				f.bracketFormatMode = astyle::NONE_MODE;
				break;
			}
			//
			f.breakBlocks = s->value( "break_blocks", false ).toBool();
			f.breakElseIfs = s->value( "break_elseifs", false ).toBool();
			f.padOperators = s->value( "pad_operators", false ).toBool();
			f.padParen = s->value( "pad_parentheses", false ).toBool();
			f.breakOneLineStatements = !s->value( "keep_complex", false ).toBool();
			f.breakOneLineBlocks = !s->value( "keep_blocks", false ).toBool();
			break;
		}
	}
	s->endGroup();
}
