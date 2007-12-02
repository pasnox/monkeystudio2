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
				QRegExp("(\\n[\\w\\./]+\\.\\w+: In [\\w\\s]+ '.+':)?\\n(([[^\\n]]+/)?([\\w.]+)):(\\d+):(\\d+:)?\\serror:\\s([^\\n]+)\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%2", //file name
				"%6", //column
				"%7", //row
				pConsoleManager::stError, //type
				"%4:%5: %7", //text
				"%0", //full text
			},
			{
				//Warning in the file/line
				QRegExp("(\\n[\\w\\./]+\\.\\w+: In [\\w\\s]+ '.+':)?\\n(([^\\n]+/)?([\\w.]+)):(\\d+):(\\d+:)?\\swarning:\\s([^\\n]+)\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%2", //file name
				"%6", //column
				"%5", //row
				pConsoleManager::stWarning, //type
				"%4:%5: %7", //text
				"%0" //full text
			},
			{
				//Building file
				QRegExp("\\n[gc]\\+\\+ .+ (.+\\.\\w+)\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%1", //file name
				"0", //column
				"0", //row
				pConsoleManager::stCompiling, //type
				"Compiling %1...", //text
				"%0" //full text
			},
			{
				//Linking file
				QRegExp("\\ng\\+\\+\\w+\\-o\\s+([^\\s]+)[^\\n]+\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"0", //file name
				"0", //column
				"0", //row
				pConsoleManager::stLinking, //type
				"Linking %1...", //text
				"%0" //full text
			},
			{
				//Undedined reference 
				QRegExp("\\n[\\w\\./]+\\.o: (In function `.+':)\\n[\\w\\./]*/([\\w\\.]+):(\\d+): (undefined reference to `.+'\\n)", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"%2", //file name
				"%3", //column
				"0", //row
				pConsoleManager::stError, //type
				"%1 %4", //text
				"%0" //full text
			},
			{
				//LD has no permissions for write file.  Not tested with unix output.
				QRegExp("\\n\\w*(ld(\\.exe)?)(: cannot open output file [^:]+: Permission denied)\\n", Qt::CaseSensitive, QRegExp::RegExp2), //reg exp
				"", //file name
				"0", //column
				"0", //row
				pConsoleManager::stError, //type
				"%1%3", //text
				"%0" //full text
			},
			{QRegExp(), "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
		};
		for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
			patterns.append (ps[i]);
		
	};

};

#endif
