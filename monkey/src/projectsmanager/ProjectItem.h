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

class Q_MONKEY_EXPORT ProjectItem : /*public QObject,*/ public QStandardItem
{
	//Q_OBJECT
	friend class UIProjectsManager;
	
public:
	ProjectItem( ProjectsModel::NodeType = ProjectsModel::ProjectType, ProjectItem* = 0 );
virtual ~ProjectItem();

	// the plugin this item come from
	virtual QString pluginName() const { return QString::null; }
	
	// set data
	virtual void setData( const QVariant&, int = Qt::UserRole +1 );
	// type of the item
	virtual void setType( ProjectsModel::NodeType );
	virtual ProjectsModel::NodeType getType();
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
	// item indent
	virtual QString getIndent() const;
	// item eol
	virtual QString getEol() const;
	// tell if item is the first child of its parent
	virtual bool isFirst() const;
	// tell if item is the last child of its parent
	virtual bool isLast() const;
	
	// item model
	virtual ProjectsModel* model() const;
	// item parent
	virtual ProjectItem* parent() const;
	// child item
	virtual ProjectItem* child( int, int = 0 ) const;
	// get children, recursively according to bool and same project according to bool
	virtual ProjectItemList children( bool = false, bool = false ) const;
	// return a new item of type of caller
	virtual ProjectItem* clone( ProjectsModel::NodeType = ProjectsModel::ProjectType, ProjectItem* = 0 ) const;
	// append item
	virtual void appendRow( ProjectItem* );
	// insert item
	virtual void insertRow( int, ProjectItem* );
	// moving item
	virtual bool swapRow( int, int );
	virtual bool moveRowUp( int );
	virtual bool moveRowDown( int );
	virtual bool moveUp();
	virtual bool moveDown();
	// remove itself
	virtual void remove();
	
	// the project item
	virtual ProjectItem* project() const;
	// get all project items
	virtual ProjectItemList projectItems() const;
	// parent project in case it s a subproject
	virtual ProjectItem* parentProject() const;
	// get direct child project, or all according to bool
	virtual ProjectItemList childrenProjects( bool = true ) const;
	// get all scope from project
	virtual ProjectItemList projectScopes() const;
	// get all unique scope list from project
	virtual QStringList projectScopesList() const;
	// get last scope/nestedscope of this item
	virtual ProjectItem* lastScope() const { return 0; }
	// get very last project scope item
	virtual ProjectItem* lastProjectScope() const { return 0; }
	
	// get index list
	virtual ProjectItemList match( ProjectItem*, int, const QVariant& ) const;
	// check scope
	virtual QString checkScope( const QString& ) const;
	// check equals scope
	virtual bool isEqualScope( const QString&, const QString& ) const;
	// scope of item
	virtual QString scope() const;
	// get all items matching
	virtual ProjectItemList getItemList( ProjectsModel::NodeType, const QString&, const QString&, const QString& ) const;
	// get item scope, creating it if needed
	virtual ProjectItem* getItemScope( const QString&, bool ) const;
	// get all variable content as modelindex list for project index
	virtual ProjectItemList getItemListValues( const QString&, const QString&, const QString& ) const;
	// get a variable index
	virtual ProjectItem* getItemVariable( const QString&, const QString&, const QString& ) const;
	// get variable content as stringlist for project index
	virtual QStringList getListValues( const QString&, const QString& = "=", const QString& = QString::null ) const;
	// get variable content as string for project index
	virtual QString getStringValues( const QString&, const QString& = "=", const QString& = QString::null ) const;
	// set variable content as stringlist for project index
	virtual void setListValues( const QStringList&, const QString&, const QString& = "=", const QString& = QString::null );
	// get variable content as string for project index
	virtual void setStringValues( const QString&, const QString&, const QString& = "=", const QString& = QString::null );
	// add variable content as stringlist for project index
	virtual void addListValues( const QStringList&, const QString&, const QString& = "=", const QString& = QString::null );
	// add variable content as string for project index
	virtual void addStringValues( const QString&, const QString&, const QString& = "=", const QString& = QString::null );
	
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
	
	// close the project
	virtual void close() {}
	// save project, asking user according to bool
	virtual void save( bool = true ) {}
	// save project including all children projects
	virtual void saveAll( bool = true ) {}
	
protected:
	QString mBuffer;
	virtual void redoLayout( ProjectItem* = 0 ) {}
	virtual void writeProject() {}
	virtual void writeItem( ProjectItem* ) {}

};

#endif // PROJECTITEM_H
