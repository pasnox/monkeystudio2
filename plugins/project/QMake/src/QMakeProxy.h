/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QMakeProxy.h
** Date      : 2007-11-04T22:54:27
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef QMAKEPROXY_H
#define QMAKEPROXY_H

#include "ProjectsProxy.h"

class ProjectsModel;
class ProjectItem;

class QMakeProxy : public ProjectsProxy
{
	Q_OBJECT
	
public:
	QMakeProxy( ProjectsModel*, ProjectItem* );
	
protected:
	bool filterAcceptsRow( int, const QModelIndex& ) const;
	ProjectItem* mProject;
	
public slots:
	virtual void setFiltering( bool );
	
};

#endif // QMAKEPROXY_H
