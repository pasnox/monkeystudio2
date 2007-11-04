/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : GNUMakeParser.h
** Date      : 2007-11-04T22:50:52
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef GNUMAKEPARSER_H
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
