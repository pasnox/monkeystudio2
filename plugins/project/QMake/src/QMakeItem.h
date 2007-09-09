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

	virtual ProjectItem* clone( ProjectsModel::NodeType = ProjectsModel::ProjectType, ProjectItem* = 0 ) const;

	virtual QString pluginName() const { return "QMake"; }

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
	
protected:
	virtual void redoLayout( ProjectItem* );

};

#endif // QMAKEITEM_H
