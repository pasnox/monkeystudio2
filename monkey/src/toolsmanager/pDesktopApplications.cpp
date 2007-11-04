/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pDesktopApplications.cpp
** Date      : 2007-11-04T22:46:49
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pDesktopApplications.h"

pDesktopApplications::pDesktopApplications( QObject* o )
	: QObject( o )
{}

pDesktopApplications::~pDesktopApplications()
{}

int pDesktopApplications::count( pDesktopFolder* f )
{
	if ( !f )
		return 0;
	// recursive count
	int i = 0;
	// Applications
	foreach ( pDesktopApplication* a, f->Applications )
		i++;
	// Folders
	foreach ( pDesktopFolder* d, f->Folders )
		i += count( d );
	// return result
	return i;
}

const int pDesktopApplications::count()
{
	return count( startMenu() );
}
