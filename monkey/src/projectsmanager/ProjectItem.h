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

#include "MonkeyExport.h"
#include "ProjectsModel.h"

#include <QStandardItem>

class Q_MONKEY_EXPORT ProjectItem : public QStandardItem
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
	
	void appendRow( ProjectItem* );
	void insertRow( int, ProjectItem* );
	
	ProjectItem* parent() const;
	
	QString canonicalFilePath() const;
	QString canonicalFilePath( const QString& ) const;
	QString canonicalPath() const;
	
	QString fileName( const QString& );
	QString completeBaseName( const QString& );

};

#endif // PROJECTITEM_H
