#ifndef UIXUPPROJECTEDITOR_H
#define UIXUPPROJECTEDITOR_H

#include "ui_UIXUPProjectEditor.h"

#include <QSortFilterProxyModel>
#include <QDebug>

class XUPItem;
class ScopedProjectItemModel;
class FilteredProjectItemModel;
class ProjectEditorModel;

#include "FilteredProjectItemModel.h"
class FilesProjectModel : public QSortFilterProxyModel
{
	Q_OBJECT

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
	ProjectItemModel* mProjectModel;
	ScopedProjectItemModel* mScopedModel;
	FilteredProjectItemModel* mFilteredModel;
	ProjectEditorModel* mVariablesModel;
	ProjectEditorModel* mValuesModel;

	XUPItem* currentScope() const;
	XUPItem* currentVariable() const;
	XUPItem* currentValue() const;

protected slots:
	// scope
	void on_cbScope_currentChanged( const QModelIndex& index );
	// operator
	void on_cbOperator_currentIndexChanged( const QString& text );
	// project files
	void on_pbAddProjectFiles_clicked();
	void on_pbRemoveProjectFile_clicked();
	void on_pbModifyProjectFile_clicked();
	// others variables
	void lvOthersVariables_currentChanged( const QModelIndex& current, const QModelIndex& previous );
	// variables
	void on_tbOthersVariablesAdd_clicked();
	void on_tbOthersVariablesEdit_clicked();
	void on_tbOthersVariablesRemove_clicked();
	// others values
	void lvOthersValues_currentChanged( const QModelIndex& current, const QModelIndex& previous );
	// values
	void on_tbOthersValuesAdd_clicked();
	void on_tbOthersValuesEdit_clicked();
	void on_tbOthersValuesRemove_clicked();
	void on_tbOthersValuesClear_clicked();
};

#endif // UIXUPPROJECTEDITOR_H
