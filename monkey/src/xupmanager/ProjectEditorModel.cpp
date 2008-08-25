#include "ProjectEditorModel.h"
#include "XUPItem.h"
#include "XUPProjectItem.h"
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
		XUPProjectItem* pit = mProject->project();
		// if project or parent project
		if ( it->isProject() )
			return it->children( true, false ).contains( pit ) || it->project() == pit;
		// if variable and filter != values
		if ( it->isType( "variable" ) && mType != ProjectEditorModel::vtValues )
			return it->project() == pit && !it->project()->fileVariables().contains( it->defaultValue() );
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

QVariant ProjectEditorModel::data( const QModelIndex& index, int role ) const
{
	// if tooltip role
	if ( role == Qt::ToolTipRole )
	{
		// if item
		if ( XUPItem* it = mSourceModel->itemFromIndex( mapToSource( index ) ) )
		{
			QString tooltip;
			if ( it->isType( "variable" ) )
				return QVariant( tr( "<b>Variable</b><br />%1 (%2, %3)" ).arg( it->defaultValue() ).arg( it->value( "operator", "=" ) ).arg( QVariant( it->value( "multiline", "false" ) ).toBool() ? tr( "multiline" ) : tr( "singleline" ) ) );
			else if ( it->isType( "value" ) )
			{
				QString tooltip = QString( "<b>Value</b><br />%1" ).arg( it->defaultValue() );
				if ( !it->value( "comment" ).isEmpty() )
					tooltip += QString( "<b>Comment</b><br />%1" ).arg( it->value( "comment" ) );
				return QVariant( tooltip );
			}
		}
	}
	// defaut values
	return QSortFilterProxyModel::data( index, role );
}
