/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pFormatterSettings.h
** Date      : 2007-11-04T22:50:32
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PFORMATTERSETTINGS_H
#define PFORMATTERSETTINGS_H

#include "astyle.h"

class pFormatterSettings
{
public:
	void applyTo( astyle::ASFormatter& );
	
};

#endif // PFORMATTERSETTINGS_H
