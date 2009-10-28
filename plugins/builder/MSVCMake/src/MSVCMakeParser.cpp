/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Builder Plugins
** FileName  : MSVCMakeParser.cpp
** Date      : 2008-01-14T00:52:27
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
// #include "MSVCMakeParser.h"
// 
// #include <QTextCodec>
// 
// MSVCMakeParser::MSVCMakeParser()
// {
// 	Pattern ps[] = 
// 	{
// 		{
// 			QRegExp("^make: \\*\\*\\* (No rule to make target `.+', needed by `.+')\\.  Stop\\.$"), //reg exp
// 			"No rule for make target", //desctiption
// 			"", //file name
// 			"0", //column
// 			"0", //row
// 			pConsoleManager::stError, //type
// 			"%1", //text
// 			"%0" //full text
// 		},
//         {
//             QRegExp("make\\[\\d\\]: Entering directory\\s`([^\\n]*)'"), //reg exp
//             "Entering dirrectory", //desctiption
//             "", //file name
//             "0", //column
//             "0", //row
//             pConsoleManager::stCompiling, //type
//             "make: Building %1", //text
//             "%0" //full text
//         },
// 		{QRegExp(), "", "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
// 	};
// 	for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
// 		patterns.append (ps[i]);
// 
// }
// 
// MSVCMakeParser::~MSVCMakeParser()
// {
// }
// 
// bool MSVCMakeParser::processParsing( const QByteArray& arr )
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
// 				m.mPosition = QPoint( replaceWithMatch(p.regExp,p.col).toInt(), replaceWithMatch(p.regExp,p.row).toInt());
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
// return false;
// }
// 
// QString MSVCMakeParser::name() const
// { return PLUGIN_NAME; }
// 
// QString MSVCMakeParser::replaceWithMatch(QRegExp rex, QString s)
// {
// 	int i = 0;
// 	while ( (i = s.indexOf("%")) != -1)
// 		s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
// 	return s;
// }
