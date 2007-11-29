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
				QRegExp("((.*/)?([^:]+)):(\\d+):(\\d+:)?\\serror:\\s([^\n]+)\n"), //reg exp
				"%1", //file name
				"%5", //column
				"%6", //row
				pConsoleManager::stError, //type
				"%3:%4: %6", //text
				"%0", //full text
			},
			{
				//Warning in the file/line
				QRegExp("((.*/)?([^:]+)):(\\d+):(\\d+:)?\\swarning:\\s([^\n]+)\n"), //reg exp
				"%1", //file name
				"%5", //column
				"%4", //row
				pConsoleManager::stWarning, //type
				"%3:%4: %6", //text
				"%0" //full text
			},
			{
				//Building file
				QRegExp("g\\+\\+\\s+\\-c.+([^\\s]+\\.cpp)\n"), //reg exp
				"%1", //file name
				"0", //column
				"0", //row
				pConsoleManager::stCompiling, //type
				"Compiling %1...", //text
				"%0" //full text
			},
			{
				//Linking file
				QRegExp("g\\+\\+.+\\-o\\s+([^\\s]+)[^\n]+\n"), //reg exp
				"0", //file name
				"0", //column
				"0", //row
				pConsoleManager::stLinking, //type
				"Linking %1...", //text
				"%0" //full text
			},
			{
				//Undedined reference in the function
				QRegExp(".*(In\\sfunction\\s.*:.*:).+(\\sundefined\\sreference\\sto[^\n]+)\n"), //reg exp
				"", //file name
				"0", //column
				"0", //row
				pConsoleManager::stError, //type
				"%1%2", //text
				"%0" //full text
			},
			{
				//Undedined reference in the module
				QRegExp("(\\w+)\\.o:\\([^\\)]+\\): undefined reference to `(\\w+)'\n"), //reg exp
				"", //file name
				"0", //column
				"0", //row
				pConsoleManager::stError, //type
				"%0", //text
				"%0" //full text
			},
			{
				//LD has no permissions for write file.  Not tested with unix output.
				QRegExp(".*(ld(\\.exe)?)(: cannot open output file [^:]+: Permission denied)\n"), //reg exp
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
