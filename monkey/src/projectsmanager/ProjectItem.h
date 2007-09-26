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

#include <QObject>
#include <QStandardItem>

class QTextCodec;
class ProjectItem;
typedef QList<ProjectItem*> ProjectItemList;
class ProjectPlugin;
class CompilerPlugin;
class DebuggerPlugin;
class InterpreterPlugin;

class Q_MONKEY_EXPORT ProjectItem : /*public QObject,*/ public QStandardItem
{
	//Q_OBJECT
	
public:
	//ProjectItem( ProjectsModel::NodeType = ProjectsModel::ProjectType, ProjectItem* = 0 );
	
	// item type
	virtual int type() const;
	// ProjectPlugin for item
	virtual void setPlugin( ProjectPlugin* );
	virtual ProjectPlugin* plugin() const;
	// set data
	virtual void setData( const QVariant&, int = Qt::UserRole +1 );
	// type of the item
	virtual void setType( ProjectsModel::NodeType );
	virtual ProjectsModel::NodeType getType() const;
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
	// item indent
	virtual QString getIndent() const = 0;
	// item eol
	virtual QString getEol() const = 0;
	// tell if item is the first child of its parent
	virtual bool isFirst() const = 0;
	// tell if item is the last child of its parent
	virtual bool isLast() const = 0;
	// scope of item
	virtual QString scope() const = 0;
	// check scope
	virtual QString checkScope( const QString& ) const = 0;
	// check equals scope
	virtual bool isEqualScope( const QString&, const QString& ) const = 0;
	
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
	virtual void appendRow( ProjectItem* ) = 0;
	// insert item
	virtual void insertRow( int, ProjectItem* ) = 0;
	// moving item
	virtual bool swapRow( int, int ) = 0;
	virtual bool moveRowUp( int ) = 0;
	virtual bool moveRowDown( int ) = 0;
	virtual bool moveUp() = 0;
	virtual bool moveDown() = 0;
	// remove itself
	virtual void remove() = 0;
	// redo internal layout, to filter / sort items
	virtual void refresh();
	
	// the project item
	virtual ProjectItem* project() const;
	// parent project in case it s a subproject
	virtual ProjectItem* parentProject() const;
	// get direct child project, or all according to bool
	virtual ProjectItemList childrenProjects( bool = true ) const;
	// get all project items
	virtual ProjectItemList projectItems() const;
	// get all scope from project
	virtual ProjectItemList projectScopes() const;
	// get all unique scope list from project
	virtual QStringList projectScopesList() const;
	// item direct scopes, or recursively according to bool
	virtual ProjectItemList childrenScopes( bool = false) const;
	// canonical project filepath
	virtual QString canonicalFilePath() const;
	// canonical file path according to project path
	virtual QString canonicalFilePath( const QString& ) const;
	// canonical project path
	virtual QString canonicalPath() const;
	// relative path according to project path
	virtual QString canonicalPath( const QString& ) const;
	// relative file path according to project path
	virtual QString relativeFilePath( const QString& ) const;
	// filename of filepath
	virtual QString fileName( const QString& );
	// complete basename of filename
	virtual QString completeBaseName( const QString& );
	// name of project
	virtual QString name() const;
	// tell if project is open
	virtual bool isOpen() const = 0;
	// open project based on filename in getValue()
	virtual bool open() = 0;
	// open project settings dialog
	virtual void editSettings() = 0;
	// close the project
	virtual void close() = 0;
	// save project, asking user according to bool
	virtual void save( bool = true ) = 0;
	// save project including all children projects
	virtual void saveAll( bool = true ) = 0;
	// add existing files to project scope / operator
	virtual void addExistingFiles( const QStringList&, const QString&, const QString& = "=" ) = 0;
	virtual void addExistingFiles( const QStringList&, ProjectItem*, const QString& = "=" ) = 0;
	// set compiler for this project
	virtual void setCompiler( CompilerPlugin* ) = 0;
	// compiler for this project
	virtual CompilerPlugin* compiler() const = 0;
	// set debugger for this project
	virtual void setDebugger( DebuggerPlugin* ) = 0;
	// debugger for this project
	virtual DebuggerPlugin* debugger() const = 0;
	// set interpreter for this project
	virtual void setInterpreter( InterpreterPlugin* ) = 0;
	// interpreter for this project
	virtual InterpreterPlugin* interpreter() const = 0;
	// show the content of items 
	virtual void debug() = 0;
	
	// get index list
	virtual ProjectItemList match( int, const QVariant& ) const;
	// get all items matching
	virtual ProjectItemList getItemList( ProjectsModel::NodeType, const QString&, const QString&, const QString& ) const;
	// get item scope, creating it if needed
	virtual ProjectItem* getItemScope( const QString&, bool ) const = 0;
	// get all variable content as modelindex list for project index
	virtual ProjectItemList getItemListValues( const QString&, const QString&, const QString& ) const;
	// get a variable index
	virtual ProjectItem* getItemVariable( const QString&, const QString&, const QString& ) const;
	
	// get variable content as stringlist for project index
	virtual QStringList getListValues( const QString&, const QString& = "=", const QString& = QString::null ) const = 0;
	// get variable content as string for project index
	virtual QString getStringValues( const QString&, const QString& = "=", const QString& = QString::null ) const = 0;
	// set variable content as stringlist for project index
	virtual void setListValues( const QStringList&, const QString&, const QString& = "=", const QString& = QString::null ) = 0;
	// get variable content as string for project index
	virtual void setStringValues( const QString&, const QString&, const QString& = "=", const QString& = QString::null ) = 0;
	// add variable content as stringlist for project index
	virtual void addListValues( const QStringList&, const QString&, const QString& = "=", const QString& = QString::null ) = 0;
	// add variable content as string for project index
	virtual void addStringValues( const QString&, const QString&, const QString& = "=", const QString& = QString::null ) = 0;
	
protected:
	ProjectPlugin* mPlugin;
	CompilerPlugin* mCompiler;
	DebuggerPlugin* mDebugger;
	InterpreterPlugin* mInterpreter;
	bool mIsOpen;
	QString mBuffer;
	virtual void redoLayout( ProjectItem* = 0 ) = 0;
	virtual void writeProject() = 0;
	virtual void writeItem( ProjectItem* ) = 0;

};

#endif // PROJECTITEM_H
