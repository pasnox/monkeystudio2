/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox aka Azevedo Filipe <pasnox@gmail.com>
** Project   : ProjectItem
** FileName  : ProjectItem.h
** Date      : 2007-09-04T15:07:31
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QStandardItem>

#include "ProjectsModel.h"

class ProjectItem : public QStandardItem
{
public:
	ProjectItem( ProjectsModel::NodeType = ProjectsModel::ProjectType, ProjectItem* = 0 );

	virtual void setType( ProjectsModel::NodeType );
	virtual ProjectsModel::NodeType getType();

	virtual void setOperator( const QString& );
	virtual QString getOperator() const;

	virtual void setValue( const QString& );
	virtual QString getValue() const;

	virtual void setMultiLine( bool );
	virtual bool getMultiLine() const;

	virtual void setModified( bool );
	virtual bool getModified() const;

	virtual void setReadOnly( bool );
	virtual bool getReadOnly() const;

	virtual void setComment( const QString& );
	virtual QString getComment() const;

	virtual void setFilePath( const QString& );
	virtual QString getFilePath() const;
	/*
	virtual void appendRow( const QList<ProjectItem*>& );
	virtual void appendRow( ProjectItem* );
	virtual void appendRows( const QList<ProjectItem*>& );
	
	virtual void insertColumn( int, const QList<ProjectItem*>& );
	virtual void insertColumns( int, int );
	virtual void insertRow( int, const QList<ProjectItem*>& );
	virtual void insertRow( int, ProjectItem* );
	virtual void insertRows( int, const QList<ProjectItem*>& );
	virtual void insertRows( int, int );
	
	virtual void removeColumn( int );
	virtual void removeColumns( int, int );
	virtual void removeRow( int );
	virtual void removeRows( int, int );
	
	template <class T> T child( int, int = 0 ) const;
	template <class T> T clone() const;
	
	template <class T> T parent() const;
	
	virtual void setChild( int, int, ProjectItem* );
	virtual void setChild( int, ProjectItem* );
	
	template <class T> T takeChild( int, int = 0 );
	template <class T> QList<T> takeColumn( int );
	template <class T> QList<T> takeRow( int );
	*/
};

#endif // PROJECTITEM_H
