/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox aka Azevedo Filipe <pasnox@gmail.com>
** Project   : QMakeItem
** FileName  : QMakeItem.h
** Date      : 2007-09-04T15:07:31
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QMAKEITEM_H
#define QMAKEITEM_H

#include "ProjectItem.h"

class QMakeItem : public ProjectItem
{

public:
	QMakeItem( ProjectsModel::NodeType = ProjectsModel::ProjectType, QMakeItem* = 0 );

	void setType( ProjectsModel::NodeType );
	ProjectsModel::NodeType getType();

	void setOperator( const QString& );
	QString getOperator() const;

	void setValue( const QString& );
	QString getValue() const;

	void setMultiLine( bool );
	bool getMultiLine() const;

	void setModified( bool );
	bool getModified() const;

	void setReadOnly( bool );
	bool getReadOnly() const;

	void setComment( const QString& );
	QString getComment() const;

	void setFilePath( const QString& );
	QString getFilePath() const;
	/*
	void appendRow( const QList<QMakeItem*>& );
	void appendRow( QMakeItem* );
	void appendRows( const QList<QMakeItem*>& );
	
	void insertColumn( int, const QList<QMakeItem*>& );
	void insertColumns( int, int );
	void insertRow( int, const QList<QMakeItem*>& );
	void insertRow( int, QMakeItem* );
	void insertRows( int, const QList<QMakeItem*>& );
	void insertRows( int, int );
	
	void removeColumn( int );
	void removeColumns( int, int );
	void removeRow( int );
	void removeRows( int, int );
	
	QMakeItem* child( int, int = 0 ) const;
	virtual QMakeItem* clone() const;
	
	QMakeItem* parent() const;
	
	void setChild( int, int, QMakeItem* );
	void setChild( int, QMakeItem* );
	
	QMakeItem* takeChild( int, int = 0 );
	QList<QMakeItem*> takeColumn( int );
	QList<QMakeItem*> takeRow( int );
	*/

};

#endif // QMAKEITEM_H
