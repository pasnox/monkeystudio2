/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : ProjectItem.h
** Date      : 2008-01-14T00:37:03
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
#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "MonkeyExport.h"

#include <QObject>
#include <QStandardItem>

class ProjectsModel;
class QTextCodec;
class ProjectPlugin;
class BuilderPlugin;
class CompilerPlugin;
class DebuggerPlugin;
class InterpreterPlugin;

class ProjectItem;
typedef QList<ProjectItem*> ProjectItemList;

class Q_MONKEY_EXPORT ProjectItem : public QObject, public QStandardItem
{
	Q_OBJECT
	Q_ENUMS( NodeRole NodeType )
	
public:
	enum NodeRole
	{
		FirstRole = Qt::UserRole, // first role
		TypeRole, // the item type
		OperatorRole, // the item operator
		ValueRole, // the item value
		MultiLineRole, // bool telling if variable content is single ou multi line
		CommentRole, // the item comment
		ModifiedRole, // tell if item/file is modified
		ReadOnlyRole, // tell if item/file is readonly
		FilteredViewRole, // filter role when filtering view
		OriginalViewRole, // filter role when not filtering view
		FilePathRole, // the filepath of the current value if it s a file/path
		LastRole // last role
	};
	
	enum NodeType
	{
		FirstType = 0, // first type
		EmptyType, // empty line
		FolderType, // a folder container
		CommentType, // comment line
		NestedScopeType, // single line scope
		ScopeType, // multi line scope
		ScopeEndType, // end of a multi scope line
		VariableType, // variable
		ValueType, // value
		FunctionType, // function
		IncludeType, // include
		ProjectType, // project
		LastType // last type
	};
	
	ProjectItem( ProjectItem::NodeType = ProjectItem::ProjectType, ProjectItem* = 0 );
	
	// item type
	virtual int type() const;
	// ProjectPlugin for item
	virtual void setPlugin( ProjectPlugin* );
	virtual ProjectPlugin* plugin() const;
	// set data
	virtual void setData( const QVariant&, int = Qt::UserRole +1 );
	// type of the item
	virtual void setType( ProjectItem::NodeType );
	virtual ProjectItem::NodeType getType() const;
	// item operator, only variable
	virtual void setOperator( const QString& );
	virtual QString getOperator() const;
	// item value
	virtual void setValue( const QString& );
	virtual QString getValue() const;
	// item is multiline, only variable
	virtual void setMultiLine( bool );
	virtual bool getMultiLine() const;
	// item content
	virtual void setComment( const QString& );
	virtual QString getComment() const;
	// item filepath
	virtual void setFilePath( const QString& );
	virtual QString getFilePath() const;
	// filtered mode sort
	virtual void setFilteredView( int );
	virtual int getFilteredView() const;
	// original mode sort
	virtual void setOriginalView( int );
	virtual int getOriginalView() const;
	// item modified flag
	virtual void setModified( bool );
	virtual bool getModified() const;
	// item read only, only project
	virtual void setReadOnly( bool );
	virtual bool getReadOnly() const;
	// tell if item is empty type
	virtual bool isEmpty() const;
	// tell if item is folder type
	virtual bool isFolder() const;
	// tell if item is comment type
	virtual bool isComment() const;
	// tell if item is a nested scope type
	virtual bool isNested() const;
	// tell if item is a scope type, accepting nested according to bool
	virtual bool isScope( bool = true ) const;
	// tell if item is a scope end type
	virtual bool isScopeEnd() const;
	// tell if item is a variable type
	virtual bool isVariable() const;
	// tell if item is a value type
	virtual bool isValue() const;
	// tell if item is a function type
	virtual bool isFunction() const;
	// tell if item is a include type
	virtual bool isInclude() const;
	// tell if item is a project type
	virtual bool isProject() const;
	// get a quoted string if neede, ie file starting and finishing by " is string contains spacePressed
	virtual QString quotedString( const QString& ) const; // MOVEME to pMonkeyStudio
	// item indent
	virtual QString getIndent() const { return QString(); }
	// tell if item is the first child of its parent
	virtual bool isFirst() const = 0;
	// tell if item is the last child of its parent
	virtual bool isLast() const = 0;
	// tell if this proejct is a container, ie it can contains subprojects
	virtual bool isProjectsContainer() const = 0;
	
	// item model
	virtual ProjectsModel* model() const;
	// item parent
	virtual ProjectItem* parent() const;
	// parents number
	virtual int parentCount() const;
	// child item
	virtual ProjectItem* child( int, int = 0 ) const;
	// get children, recursively according to bool and same project according to bool
	virtual ProjectItemList children( bool = false, bool = false ) const;
	// append item
	virtual void appendRow( ProjectItem* i );
	// insert item
	virtual void insertRow( int, ProjectItem* ) = 0;
	// moving item
	virtual bool swapRow( int, int );
	virtual bool moveRowUp( int );
	virtual bool moveRowDown( int );
	virtual bool moveUp();
	virtual bool moveDown();
	// remove itself
	virtual void remove();
	// redo internal layout, to filter / sort items
	virtual void refresh();
	
	// the project item
	virtual ProjectItem* project() const;
	// parent project in case it s a subproject
	virtual ProjectItem* parentProject() const;
	// get direct child project, or all according to bool
	virtual ProjectItemList childrenProjects( bool = true ) const;
	// get all project items
	virtual ProjectItemList projectItems( bool = true ) const;
	// canonical project filepath
	virtual QString canonicalFilePath() const;
	// canonical file path according to project path
	virtual QString canonicalFilePath( const QString& ) const;
	// canonical project path
	virtual QString canonicalPath() const;
	// canonical path according to project path
	virtual QString canonicalPath( const QString& ) const;
	// relative file path according to project path
	virtual QString relativeFilePath( const QString& ) const;
	// relative path according to project path
	virtual QString relativePath( const QString& ) const;
	// filename of filepath
	virtual QString fileName( const QString& );
	// complete basename of filename
	virtual QString completeBaseName( const QString& );
	// name of project
	virtual QString name() const;
	// open project based on filename in getValue()
	virtual bool open() = 0;
	// add a subproject to this projects container
	virtual bool addProject( const QString&, ProjectItem*, const QString& = "=" ) = 0;
	// open project settings dialog
	virtual void editSettings() = 0;
	// close the project
	virtual void close();
	// save project, asking user according to bool
	virtual void save( bool = true ) = 0;
	// save project including all children projects
	virtual void saveAll( bool = true );
	// add existing files to project scope / operator
	virtual void addExistingFiles( const QStringList&, ProjectItem*, const QString& = "=" ) = 0;
	virtual void addExistingFile( const QString&, ProjectItem*, const QString& = "=" );
	// set builder for this project
	virtual void setBuilder( BuilderPlugin* ) {}
	// builder for this project
	virtual BuilderPlugin* builder() const { return mBuilder; }
	// set compiler for this project
	virtual void setCompiler( CompilerPlugin* ) {}
	// compiler for this project
	virtual CompilerPlugin* compiler() const { return mCompiler; }
	// set debugger for this project
	virtual void setDebugger( DebuggerPlugin* ) {}
	// debugger for this project
	virtual DebuggerPlugin* debugger() const { return mDebugger; }
	// set interpreter for this project
	virtual void setInterpreter( InterpreterPlugin* ) {}
	// interpreter for this project
	virtual InterpreterPlugin* interpreter() const { return mInterpreter; }
	// install own command for builder/compiler/debugger/interpreter to correct menu
	virtual void installCommands() {}
	// uninstall own command for builder/debugger/interpreter to correct menu
	virtual void uninstallCommands() {}
	
	// get index list
	virtual ProjectItemList match( int, const QVariant&, bool = true ) const;
	// set values for scope, variable and operator
	virtual void setValues( ProjectItem*, const QString&, const QString&, const QStringList& ) = 0;
	// add value for scope, variable and operator
	virtual void addValue( ProjectItem*, const QString&, const QString&, const QString& ) = 0;
	// get valueslist as items
	virtual ProjectItemList getValues( ProjectItem*, const QString&, const QString& ) const = 0;
	// get valueslist as stringlist
	virtual QStringList getStringValues( ProjectItem* s, const QString& v, const QString& o ) const
	{
		QStringList l;
		foreach ( ProjectItem* it, getValues( s, v, o ) )
			l << it->getValue();
		return l;
	}
	// get first value for variable, operator
	virtual ProjectItem* getFirstValue( ProjectItem* s, const QString& v, const QString& o ) const
	{ return getValues( s, v, o ).value( 0 ); }
	// get items for scope by type, name, operator
	virtual ProjectItemList getItemList( ProjectItem*, ProjectItem::NodeType, const QString&, const QString& ) const;
	// get all items for type, name, operator
	virtual ProjectItemList getAllItemList( ProjectItem::NodeType, const QString&, const QString& ) const;
	
protected:
	ProjectPlugin* mPlugin;
	BuilderPlugin* mBuilder;
	CompilerPlugin* mCompiler;
	DebuggerPlugin* mDebugger;
	InterpreterPlugin* mInterpreter;
	QByteArray mBuffer;
	virtual void init();
	virtual void redoLayout( ProjectItem* = 0 ) = 0;
	virtual bool writeProject() = 0;
	virtual void writeItem( ProjectItem* ) = 0;

signals:
	void aboutToClose();
	void closed();
	void modifiedChanged( bool );

};

#endif // PROJECTITEM_H
