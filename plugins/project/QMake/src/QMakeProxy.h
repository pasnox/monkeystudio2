#ifndef QMAKEPROXY_H
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
