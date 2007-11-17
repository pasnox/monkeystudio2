#ifndef PARSER_H
#define PARSER_H

#include "pCommandParser.h"

#include <QList>
#include <QRegExp>

class Parser : public pCommandParser
{
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

Q_OBJECT
protected:
	QList <Pattern> patterns;
	QString replaceWithMatch(QRegExp, QString);
public:
	Parser();
	~Parser();

	QString name() const;
	bool processParsing(const QByteArray&);
};

#endif
