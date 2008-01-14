/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Compiler Plugins
** FileName  : Parser.h
** Date      : 2008-01-14T00:53:27
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

#ifndef PARSER_H
#define PARSER_H

#include "pCommandParser.h"

class Parser : public pCommandParser
{
Q_OBJECT
public:
	Parser(QObject* parent ) :pCommandParser (parent)
	{
		mName = PLUGIN_NAME;
		Pattern ps[] = 
		{
			{
				//Error in the file/line
				QRegExp("^([\\w\\./]+\\.\\w+: In [\\w\\s]+ '.+':\\n)?(([^\\n]+/)?([\\w.]+)):(\\d+):(\\d+:)?\\serror:\\s([^\\n]+)\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%2", //file name
				"%6", //column
				"%5", //row
				pConsoleManager::stError, //type
				"%4:%5: %7", //text
				"%0", //full text
			},
			{
				//Warning in the file/line
				QRegExp("^([\\w\\./]+\\.\\w+: In [\\w\\s]+ '.+':\\n)?(([^\\n]+/)?([\\w.]+)):(\\d+):(\\d+:)?\\swarning:\\s([^\\n]+)\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%2", //file name
				"%6", //column
				"%5", //row
				pConsoleManager::stWarning, //type
				"%4:%5: %7", //text
				"%0" //full text
			},
			{
				//Building file
				QRegExp("^[gc]\\+\\+ [^\\n]+ ([^\\n]+\\.\\w+)\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%1", //file name
				"0", //column
				"0", //row
				pConsoleManager::stCompiling, //type
				"Compiling %1...", //text
				"%0" //full text
			},
			{
				//Linking file
				QRegExp("^[gc]\\+\\+\\w+\\-o\\s+([^\\s]+)[^\\n]+\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"0", //file name
				"0", //column
				"0", //row
				pConsoleManager::stLinking, //type
				"Linking %1...", //text
				"%0" //full text
			},
			{
				//Undedined reference 
				QRegExp("^[\\w\\./]+\\.o: (In function `.+':)\\n[\\w\\./]*/([\\w\\.]+):(\\d+): (undefined reference to `.+')\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%2", //file name
				"%3", //column
				"0", //row
				pConsoleManager::stError, //type
				"%1 %4", //text
				"%0" //full text
			},
			{
				//Missing library
				QRegExp("^/[\\w:/]+ld: cannot find -l(\\w+)\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"", //file name
				"", //column
				"", //row
				pConsoleManager::stError, //type
				"%1 library not finded", //text
				"%0" //full text
			},
			{  //FIXME It's moc's error
				//Class declaration lacks Q_OBJECT macro.
				QRegExp("^(\\w+\\.\\w){1,3}:(\\d+): Error: Class declarations lacks Q_OBJECT macro\\.\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%1", //file name
				"", //column
				"%2", //row
				pConsoleManager::stError, //type
				"%0", //text
				"%0" //full text
			},
			{QRegExp(), "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
		};
		for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
			patterns.append (ps[i]);
		
	};

};

#endif
