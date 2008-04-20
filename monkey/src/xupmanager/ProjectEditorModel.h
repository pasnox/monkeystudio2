#ifndef PROJECTEDITORMODEL_H
#define PROJECTEDITORMODEL_H

#include <fresh.h>

#include <QSortFilterProxyModel>

class XUPItem;
class ProjectItemModel;

class Q_MONKEY_EXPORT ProjectEditorModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	enum ViewType { vtAll = 0, vtVariables, vtValues };
	ProjectEditorModel( XUPItem* project, ProjectEditorModel::ViewType viewType = ProjectEditorModel::vtAll, QObject* owner = 0 );

	void setRootItem( XUPItem* root );
	XUPItem* rootItem() const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

protected:
	XUPItem* mProject;
	XUPItem* mRoot;
	ProjectEditorModel::ViewType mType;
	ProjectItemModel* mSourceModel;

	virtual bool filterAcceptsRow( int sr, const QModelIndex& sp ) const;
};

#endif // PROJECTEDITORMODEL_H
