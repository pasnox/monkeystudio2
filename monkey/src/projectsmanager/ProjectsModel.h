#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include "MonkeyExport.h"
#include "ProjectItem.h"

#include <QStandardItemModel>

class ProjectsProxy;

class Q_MONKEY_EXPORT ProjectsModel : public QStandardItemModel
{
	Q_OBJECT
	
public:
	ProjectsModel( QObject* = 0 );
	ProjectItemList projects( bool = true );
	
	inline ProjectItem* item( int r, int c = 0 ) const
	{ return dynamic_cast<ProjectItem*>( QStandardItemModel::item( r, c ) ); }
	inline ProjectItem* itemFromIndex( const QModelIndex& i ) const
	{ return dynamic_cast<ProjectItem*>( QStandardItemModel::itemFromIndex( i ) ); }
	inline ProjectItem* takeItem( int r, int c = 0 )
	{ return dynamic_cast<ProjectItem*>( QStandardItemModel::takeItem( r, c ) ); }
	inline ProjectsProxy* projectsProxy() const
	{ return mProxy; }
	
private:
	ProjectsProxy* mProxy;
	
};



#endif // PROJECTSMODEL_H
