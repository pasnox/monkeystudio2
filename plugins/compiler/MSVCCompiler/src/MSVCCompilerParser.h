/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : MSVCCompilerParser.h
** Date      : 2007-11-04T22:53:32
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
