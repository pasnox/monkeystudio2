/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pCommandParser.h
** Date      : 2007-11-04T22:45:04
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
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
	QString mName;

public:
	pCommandParser( QObject* o = 0 ) : QObject( o ) {}
	virtual ~pCommandParser() {}

	virtual QString name() const { return mName; }

public slots:
	virtual bool processParsing( const QByteArray& ) = 0;

signals:
	void newStepAvailable( const pConsoleManager::Step& );

};

typedef QList<pCommandParser*> pCommandParserList;

#endif // PCOMMANDPARSER_H
