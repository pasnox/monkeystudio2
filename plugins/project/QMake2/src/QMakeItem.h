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
#include "pCommand.h"

class QMakeItem : public ProjectItem
{
	Q_OBJECT
	
public:
	QMakeItem( ProjectItem::NodeType = ProjectItem::ProjectType, ProjectItem* = 0 );
	
	virtual void setType( ProjectItem::NodeType );
	virtual void setValue( const QString& );
	virtual void setFilePath( const QString& );
	virtual QString getIndent() const;
	virtual QString getEol() const;
	virtual bool isFirst() const;
	virtual bool isLast() const;
	virtual bool isProjectsContainer() const;
	
	virtual void insertRow( int, ProjectItem* );
	
	virtual bool open();
	virtual bool addProject( const QString&, ProjectItem*, const QString& = "=" );
	virtual void editSettings();
	virtual void save( bool = true );
	virtual void addExistingFiles( const QStringList&, ProjectItem*, const QString& = "=" );
	virtual void setBuilder( BuilderPlugin* );
	virtual BuilderPlugin* builder() const;
	virtual void setCompiler( CompilerPlugin* );
	virtual CompilerPlugin* compiler() const;
	virtual void setDebugger( DebuggerPlugin* );
	virtual DebuggerPlugin* debugger() const;
	virtual void addCommand( const pCommand&, const QString& );
	virtual void installCommands();
	virtual void uninstallCommands();
	
protected:
	virtual void redoLayout( ProjectItem* );
	virtual bool writeProject();
	virtual void writeItem( ProjectItem* );

	pCommandList mCommands;

private slots:
	virtual void commandTriggered();

};

#endif // QMAKEITEM_H
