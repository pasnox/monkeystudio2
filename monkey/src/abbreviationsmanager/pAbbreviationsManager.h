/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox <pasnox@gmail.com>
** Project   : pAbbreviationsManager
** FileName  : pAbbreviationsManager.h
** Date      : sam. août 11 19:31:08 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PABBREVIATIONSMANAGER_H
#define PABBREVIATIONSMANAGER_H

#include "MonkeyExport.h"

#include <QString>
#include <QList>

class pEditor;

struct Q_MONKEY_EXPORT pAbbreviation
{
	pAbbreviation() {}
	pAbbreviation( const QString& t, const QString& d, const QString& l, const QString& c )
		: Template( t ), Description( d ), Language( l ), Code( c ) {}

	QString Template;
	QString Description;
	QString Language;
	QString Code;

};

namespace pAbbreviationsManager
{
	const QList<pAbbreviation> defaultAbbreviations();
	const QList<pAbbreviation> availableAbbreviations();
	void expandAbbreviation( pEditor* );

};

#endif // PABBREVIATIONSMANAGER_H
