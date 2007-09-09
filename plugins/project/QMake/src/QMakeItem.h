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
	virtual void setValue( const QString& );
	virtual void setFilePath( const QString& );
	
protected:
	virtual void redoLayout( ProjectItem* );

};

#endif // QMAKEITEM_H
