/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Compiler Plugins
** FileName  : MSVCParser.cpp
** Date      : 2008-01-14T00:53:36
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
**
****************************************************************************/
// #include "MSVCParser.h"
// 
// #include <QTextCodec>
// 
// MSVCParser::MSVCParser()
// {
// 	mName = "MSVCParser";
// 	Pattern ps[] = 
// 	{
// 		{
// 			QRegExp("^(.*/)?([^:]+):(\\d+):(\\d+:)?\\serror:\\s(.+)$"), //reg exp
// 			"Error in the file/line", //desctiption
// 			"%2", //file name
// 			"%4", //column
// 			"%3", //row
// 			pConsoleManager::stError, //type
// 			"%2:%3: %5", //text
// 			"%0", //full text
// 		},
// 		{
// 			QRegExp("^(.*/)?([^:]+):(\\d+):(\\d+:)?\\swarning:\\s(.+)$"), //reg exp
// 			"Warning in the file/line", //desctiption
// 			"%2", //file name
// 			"%4", //column
// 			"%3", //row
// 			pConsoleManager::stWarning, //type
// 			"%2:%3: %5", //text
// 			"%0" //full text
// 		},
// 		{
// 			QRegExp("^g\\+\\+\\s+\\-c.+([^\\s]+\\.cpp)"), //reg exp
// 			"Building file", //desctiption
// 			"%1", //file name
// 			"0", //column
// 			"0", //row
// 			pConsoleManager::stCompiling, //type
// 			"Compiling %1...", //text
// 			"%0" //full text
// 		},
// 		{
// 			QRegExp("^g\\+\\+.+\\-o\\s+([^\\s]+).+"), //reg exp
// 			"Linking file", //desctiption
// 			"0", //file name
// 			"0", //column
// 			"0", //row
// 			pConsoleManager::stLinking, //type
// 			"Linking %1...", //text
// 			"%0" //full text
// 		},
// 		{
// 			QRegExp("^.*(In\\sfunction\\s.*:.*:).+(\\sundefined\\sreference\\sto.+)$"), //reg exp
// 			"Undedined reference", //desctiption
// 			"", //file name
// 			"0", //column
// 			"0", //row
// 			pConsoleManager::stError, //type
// 			"%1%2", //text
// 			"%0" //full text
// 		},
// 		{QRegExp(), "", "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
// 	};
// 	for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
// 		patterns.append (ps[i]);
// }
// 
// MSVCParser::~MSVCParser()
// {}
// 
// bool MSVCParser::processParsing( const QByteArray& arr )
// {
// 	QStringList l = QTextCodec::codecForLocale()->toUnicode( arr ).split( '\n' );
// 	foreach (QString s, l)
// 	{
// 		foreach ( Pattern p, patterns)
// 		{
// 			if (p.regExp.indexIn(s) != -1)
// 			{
// 				pConsoleManager::Step m;
// 				m.mFileName = replaceWithMatch(p.regExp,p.FileName);
// 				m.mPosition = QPoint( replaceWithMatch(p.regExp,p.row).toInt(), replaceWithMatch(p.regExp,p.col).toInt());
// 				m.mType = p.Type;
// 				m.mText = replaceWithMatch(p.regExp,p.Text);
// 				m.mFullText = replaceWithMatch(p.regExp,p.FullText);
// 				// emit signal
// 				emit newStepAvailable( m );
// 				return true;
// 			}
// 			
// 		}
// 	}
// 	return false;
// }
// 
// QString MSVCParser::replaceWithMatch( QRegExp rex, QString s )
// {
// 	int i = 0;
// 	while ( (i = s.indexOf("%")) != -1)
// 		s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
// 	return s;
// }
