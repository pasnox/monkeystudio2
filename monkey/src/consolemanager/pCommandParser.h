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
	void newStepAvailable( const pConsoleManager::Step );

};

#endif // PCOMMANDPARSER_H
