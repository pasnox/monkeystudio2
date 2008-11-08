/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Builder Plugins
** FileName  : GNUMakeParser.h
** Date      : 2008-01-14T00:52:24
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
#ifndef GNUMAKEPARSER_H
#define GNUMAKEPARSER_H

#include <pConsoleManager.h>
#include <pCommandParser.h>

class GNUMakeParser : public pCommandParser
{
Q_OBJECT
public:
	GNUMakeParser(QObject* parent ) :pCommandParser (parent)
	{
		mName = PLUGIN_NAME;
		Pattern ps[] = 
		{
		{
			QRegExp("^make: \\*\\*\\* (No rule to make target `\\w+', needed by `\\w+')\\.  Stop\\.\\n"), //reg exp
			//No rule for make target
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stError, //type
			"%1", //text
			"%0" //full text
		},
		{
			QRegExp("^make\\[\\d\\]: Entering directory\\s`([^\\n]*)'\\n"), //reg exp
			//Entering dirrectory,
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stCompiling, //type
			"make: Building %1", //text
			"%0" //full text
		},
		{QRegExp(), "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
		};
		for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
			patterns.append (ps[i]);
	};

};

#endif // GNUMAKEPARSER_H
