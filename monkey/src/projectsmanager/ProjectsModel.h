/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : ProjectsModel.h
** Date      : 2007-11-04T22:45:58
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
	
	inline ProjectItem* item( int r, int c = 0 ) const { return dynamic_cast<ProjectItem*>( QStandardItemModel::item( r, c ) ); }
	inline ProjectItem* itemFromIndex( const QModelIndex& i ) const { return dynamic_cast<ProjectItem*>( QStandardItemModel::itemFromIndex( i ) ); }
	inline ProjectItem* takeItem( int r, int c = 0 ) { return dynamic_cast<ProjectItem*>( QStandardItemModel::takeItem( r, c ) ); }
	inline ProjectsProxy* projectsProxy() const { return mProjects; }
	inline ProjectsProxy* scopesProxy() const { return mScopes; }
	
private:
	ProjectsProxy* mProjects;
	ProjectsProxy* mScopes;

protected slots:
	void rowsChanged( const QModelIndex&, int, int );
	
};



#endif // PROJECTSMODEL_H
