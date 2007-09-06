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

	QString pluginName() const { return "QMake"; }

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

};

#endif // QMAKEITEM_H
