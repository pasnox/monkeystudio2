#include "ProjectEditorModel.h"
#include "XUPItem.h"
#include "ProjectItemModel.h"

ProjectEditorModel::ProjectEditorModel( XUPItem* project, ProjectEditorModel::ViewType viewType, QObject* owner )
	: QSortFilterProxyModel( owner )
{
	Q_ASSERT( project );
	mProject = project;
	mRoot = 0;
	mType = viewType;
	mSourceModel = mProject->model();
	setSourceModel( mSourceModel );
}

bool ProjectEditorModel::filterAcceptsRow( int sr, const QModelIndex& sp ) const
{
	QModelIndex idx = mSourceModel->index( sr, 0, sp );
	if ( XUPItem* it = mSourceModel->itemFromIndex( idx ) )
	{
		XUPItem* pit = mProject->project();
		// if project or parent project
		if ( it->isProject() )
			return it->children( true, false ).contains( pit ) || it->project() == pit;
		// if variable and filter != values
		if ( it->isType( "variable" ) && mType != ProjectEditorModel::vtValues )
			return it->project() == pit && !it->fileVariables().contains( it->defaultValue() );
		// if value and filter != variable
		if ( ( it->isType( "value" ) || it->isType( "folder" ) ) && mType != ProjectEditorModel::vtVariables )
			return true;
		// if root is set
		if ( mRoot )
			return it->children( true, false ).contains( mRoot ) || mRoot == it;
	}
	return false;
}

void ProjectEditorModel::setRootItem( XUPItem* root )
{
	mRoot = root;
	invalidateFilter();
}

XUPItem* ProjectEditorModel::rootItem() const
{ return mRoot; }
