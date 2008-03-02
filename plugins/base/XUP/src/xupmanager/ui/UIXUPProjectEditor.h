#ifndef UIXUPPROJECTEDITOR_H
#define UIXUPPROJECTEDITOR_H

#include "ui_UIXUPProjectEditor.h"

#include <QSortFilterProxyModel>
#include <QDebug>

class XUPItem;

#include "FilteredProjectItemModel.h"
class FilesProjectModel : public QSortFilterProxyModel
{
public:
	FilesProjectModel( FilteredProjectItem* project, QObject* owner = 0 )
		: QSortFilterProxyModel( owner )
	{
		Q_ASSERT( project );
		mProject = project;
		mSourceModel = qobject_cast<FilteredProjectItemModel*>( mProject->model() );
		setSourceModel( mSourceModel );
	}

protected:
	FilteredProjectItem* mProject;
	FilteredProjectItemModel* mSourceModel;

	virtual bool filterAcceptsRow( int sr, const QModelIndex& sp ) const
	{
		QModelIndex idx = mSourceModel->index( sr, 0, sp );
		if ( FilteredProjectItem* fit = mSourceModel->itemFromIndex( idx ) )
		{
			XUPItem* it = fit->item();
			XUPItem* pit = mProject->project();
			bool b = false;
			if ( it->isProject() )
				b = it->children( true, false ).contains( pit ) || it->project() == pit;
			else if ( it->isType( "variable" ) )
				b = it->project() == pit && it->fileVariables().contains( it->defaultValue() );
			else if ( it->isType( "value" ) || it->isType( "folder" ) )
				b = true;
			return b;
		}
		return false;
	}
};

class UIXUPProjectEditor : public QDialog, public Ui::UIXUPProjectEditor
{
	Q_OBJECT

public:
	UIXUPProjectEditor( XUPItem* project, QWidget* parent = 0 );

protected:
	XUPItem* mProject;

protected slots:
	void on_pbAddProjectFiles_clicked();
};

#endif // UIXUPPROJECTEDITOR_H
