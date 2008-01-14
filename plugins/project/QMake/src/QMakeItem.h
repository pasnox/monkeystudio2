/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : QMakeItem.h
** Date      : 2008-01-14T00:54:11
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
