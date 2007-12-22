#ifndef PROJECTITEMMODEL_H
#define PROJECTITEMMODEL_H

#include <QStandardItemModel>

class ProjectItem;

class ProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	
public:
	ProjectItemModel( QObject* = 0 );
	~ProjectItemModel();
	
	// return item for row / column
	ProjectItem* item( int, int = 0 ) const;
	// return item from index
	ProjectItem* itemFromIndex( const QModelIndex& ) const;
	// append row item
	void appendRow( ProjectItem* );

	// tell if this item is a project
	bool isProject( const ProjectItem* ) const;
	bool isProject( const QModelIndex& ) const;
	// tell if this item is type
	bool isType( const ProjectItem*, const QString& ) const;
	bool isType( const QModelIndex&, const QString& ) const;
	// return the project item of this item
	ProjectItem* project( const ProjectItem* ) const;
	ProjectItem* project( const QModelIndex& ) const;
	// return the top level project
	ProjectItem* topLevelProject( const ProjectItem* ) const;
	ProjectItem* topLevelProject( const QModelIndex& ) const;
	// return all top level proejcts for column
	QList<ProjectItem*> topLevelProjects( int = 0 ) const;

	// close project
	void closeProject( const ProjectItem* );
	void closeProject( const QModelIndex& );

};

#endif // PROJECTITEMMODEL_H
