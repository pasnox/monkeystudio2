#ifndef GNUMAKEPARSER_H
#define GNUMAKEPARSER_H

#include <QProcess>
#include <QList>
#include <QRegExp>

#include "pCommandParser.h"

class GNUMakeParser : public pCommandParser
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
	GNUMakeParser();
	~GNUMakeParser();

	QString name() const;
	bool processParsing(const QByteArray&);

};

#endif
