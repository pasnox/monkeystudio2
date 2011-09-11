#ifndef XUPOPENEDPROJECTSMODEL_H
#define XUPOPENEDPROJECTSMODEL_H

#include "MonkeyExport.h"

#include <QAbstractListModel>

class XUPProjectModel;

class Q_MONKEY_EXPORT XUPOpenedProjectsModel : public QAbstractListModel
{
	Q_OBJECT
	
public:
	XUPOpenedProjectsModel( QObject* parent = 0 );
	virtual ~XUPOpenedProjectsModel();
	
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	
	void clear();
	
	void addProject( XUPProjectModel* project );
	void removeProject( XUPProjectModel* project );
	
	QList<XUPProjectModel*> projects() const;
	XUPProjectModel* project( int row ) const;
	int row( XUPProjectModel* project ) const;

protected:
	QList<XUPProjectModel*> mProjects;

protected slots:
	void project_modified();
};

#endif // XUPOPENEDPROJECTSMODEL_H
