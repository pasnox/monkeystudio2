#ifndef GCCPARSER_H
#define GCCPARSER_H

#include "pCommandParser.h"

#include <QList>
#include <QRegExp>

class GccParser : public pCommandParser
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
	QRegExp rxErrWarn, rxBuild, rxUndefRef, rxNoRule;
	QList <Pattern> patterns;
	QString replaceWithMatch(QRegExp, QString);
public:
	GccParser();
	~GccParser();

	QString name() const;
	bool processParsing(const QByteArray&);

};

#endif
