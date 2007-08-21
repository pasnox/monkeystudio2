/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox
** Project   : pDesktopApplications
** FileName  : pDesktopApplications.cpp
** Date      : ven. août 17 21:19:02 2007
** License   : GPL
** Comment   : Your comment here
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
