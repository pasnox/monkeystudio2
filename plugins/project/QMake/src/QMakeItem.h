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

#include <QStandardItem>

#include "ProjectsModel.h"

class QMakeItem : public QStandardItem
{

public:
	QMakeItem( ProjectsModel::NodeType, QMakeItem* = 0 );

	void setType( ProjectsModel::NodeType );
	ProjectsModel::NodeType type();

	void setCaption( const QString& );
	QString caption() const;

	void setOperator( const QString& );
	QString operator() const;

	void setValue( const QString& );
	QString value() const;

	void setMultiLine( bool );
	bool multiLine() const;

	void setComment( const QString& );
	QString comment() const;

	void setFilePath( const QString& );
	QString filePath() const;
	
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

};

#endif // QMAKEITEM_H
