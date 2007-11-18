#ifndef GNUMAKEPARSER_H
#define GNUMAKEPARSER_H

#include <QProcess>
#include <QList>
#include <QRegExp>

#include "pCommandParser.h"

class GNUMakeParser : public pCommandParser
{
	Q_OBJECT
	
	struct Pattern
	{
		QRegExp regExp;
		QString about;
		QString FileName;
		QString col;
		QString row;
		pConsoleManager::StepType Type;
		QString Text;
		QString FullText;
	};

protected:
	QRegExp rxErrWarn, rxBuild, rxUndefRef, rxNoRule;
	QList <Pattern> patterns;
	QString replaceWithMatch(QRegExp, QString);

public:
	GNUMakeParser();
	~GNUMakeParser();

	bool processParsing( const QByteArray& );

};

#endif
