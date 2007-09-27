#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include "MonkeyExport.h"

#include <QStandardItemModel>

class ProjectItem;
typedef QList<ProjectItem*> ProjectItemList;

class Q_MONKEY_EXPORT ProjectsModel : public QStandardItemModel
{
	Q_OBJECT
	Q_ENUMS( NodeRole NodeType )
	
public:
	enum NodeRole
	{
		FirstRole = Qt::UserRole, // first role
		TypeRole, // the item type
		OperatorRole, // the item operator
		ValueRole, // the item value
		MultiLineRole, // bool telling if variable content is single ou multi line
		CommentRole, // the item comment
		ModifiedRole, // tell if item/file is modified
		ReadOnlyRole, // tell if item/file is readonly
		FilteredViewRole, // filter role when filtering view
		OriginalViewRole, // filter role when not filtering view
		FilePathRole, // the filepath of the current value if it s a file/path
		LastRole // last role
	};
	
	enum NodeType
	{
		FirstType = 0, // first type
		EmptyType, // empty line
		FolderType, // a folder container
		CommentType, // comment line
		NestedScopeType, // single line scope
		ScopeType, // multi line scope
		ScopeEndType, // end of a multi scope line
		VariableType, // variable
		ValueType, // value
		FunctionType, // function
		IncludeType, // include
		ProjectType, // project
		LastType // last type
	};
	
	ProjectsModel( QObject* = 0 );
	
	ProjectItem* item( int, int = 0 ) const;
	ProjectItem* itemFromIndex( const QModelIndex& ) const;
	ProjectItem* takeItem( int, int = 0 );
	ProjectItemList projects( bool = true );
	
};

#endif // PROJECTSMODEL_H
