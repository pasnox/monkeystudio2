#ifndef QMAKEPROXY_H
#define QMAKEPROXY_H

#include "ProjectsProxy.h"

class ProjectsModel;
class ProjectItem;

class QMakeProxy : public ProjectsProxy
{
	Q_OBJECT
	
public:
	QMakeProxy( ProjectsModel*, bool = true, ProjectItem* = 0 );
	
protected:
	bool filterAcceptsRow( int, const QModelIndex& ) const;
	bool mProjectViewMode;
	ProjectItem* mProject;
	ProjectItem* mParentProject;
	
public slots:
	virtual void setFiltering( bool );
	
};

#endif // QMAKEPROXY_H
