/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : P@sNox
** Project   : pCommandParser
** FileName  : pCommandParser.h
** Date      : jeu. 23. août 16:18:37 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PCOMMANDPARSER_H
#define PCOMMANDPARSER_H

#include "MonkeyExport.h"
#include "pConsoleManager.h"

#include <QObject>

class ProjectItem;

class Q_MONKEY_EXPORT pCommandParser : public QObject
{
	Q_OBJECT

protected:
struct Pattern
{
	QRegExp regExp;
	QString FileName;
	QString col;
	QString row;
	pConsoleManager::StepType Type;
	QString Text;
	QString FullText;
};

	QString mName;
	QList <Pattern> patterns;
	
	QString replaceWithMatch(QRegExp&, QString);
public:
	pCommandParser (QObject* p) :QObject (p) {};
	virtual ~pCommandParser();

	virtual QString name() const;

public slots:
	virtual bool processParsing(QString&);

signals:
	void newStepAvailable( const pConsoleManager::Step& );

};

typedef QList<pCommandParser*> pCommandParserList;

#endif // PCOMMANDPARSER_H
