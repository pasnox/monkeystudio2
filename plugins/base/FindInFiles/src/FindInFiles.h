/****************************************************************************
** Authors   : Mashin Engeny aka evgenM (author), Andrei Kopats (integration)
** FileName  : FindInFiles.h
** License   : GPL
** Comment   : FindInFiles plugin
** Home Page : http://www.monkeystudio.org
*/

#ifndef FINDINFILES_H
#define FINDINFILES_H

#include "BasePlugin.h"

class FindInFiles : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	FindInFiles();
	virtual ~FindInFiles();
	virtual bool setEnabled( bool );

};

#endif
