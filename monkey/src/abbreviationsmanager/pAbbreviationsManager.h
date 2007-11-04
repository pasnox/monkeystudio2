/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pAbbreviationsManager.h
** Date      : 2007-11-04T22:45:03
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
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
