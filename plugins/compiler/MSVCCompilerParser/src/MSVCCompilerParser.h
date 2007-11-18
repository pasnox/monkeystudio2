#ifndef MSVCCOMPILERPARSER_H
#define MSVCCOMPILERPARSER_H

#include "pCommandParser.h"

#include <QList>
#include <QRegExp>

class MSVCCompilerParser : public pCommandParser
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
	MSVCCompilerParser();
	~MSVCCompilerParser();

	QString name() const;
	bool processParsing(const QByteArray&);
};

#endif // MSVCCOMPILERPARSER_H
