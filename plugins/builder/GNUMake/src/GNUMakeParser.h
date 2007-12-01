#ifndef GNUMAKEPARSER_H
#define GNUMAKEPARSER_H

#include "pCommandParser.h"

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
			QRegExp("\\nmake: \\*\\*\\* (No rule to make target `\\w+', needed by `\\w+')\\.  Stop\\.\n"), //reg exp
			//No rule for make target
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stError, //type
			"%1", //text
			"%0" //full text
		},
		{
			QRegExp("\\nmake\\[\\d\\]: Entering directory\\s`([^\\n]*)'"), //reg exp
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
