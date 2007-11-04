/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QMakeItem.h
** Date      : 2007-11-04T22:53:41
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
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
	virtual void addCommand( const pCommand&, const QString& );
	virtual void installCommands();
	virtual void uninstallCommands();
	virtual void setValues( ProjectItem*, const QString&, const QString&, const QStringList& );
	virtual void addValue( ProjectItem*, const QString&, const QString&, const QString& );
	virtual ProjectItemList getValues( ProjectItem*, const QString&, const QString& ) const;
	
	virtual void setBuilder( BuilderPlugin* );
	virtual BuilderPlugin* builder() const;
	virtual void setCompiler( CompilerPlugin* );
	virtual CompilerPlugin* compiler() const;
	virtual void setDebugger( DebuggerPlugin* );
	virtual DebuggerPlugin* debugger() const;
	virtual void setInterpreter( InterpreterPlugin* );
	virtual InterpreterPlugin* interpreter() const;
	
protected:
	virtual void redoLayout( ProjectItem* );
	virtual bool writeProject();
	virtual void writeItem( ProjectItem* );

	pCommandList mCommands;

private slots:
	virtual void commandTriggered();

};

#endif // QMAKEITEM_H
