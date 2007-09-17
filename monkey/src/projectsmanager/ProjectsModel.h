#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include "MonkeyExport.h"

#include <QStandardItemModel>

class ProjectItem;
typedef QList<ProjectItem*> ProjectItemList;

class Q_MONKEY_EXPORT ProjectsModel : public QStandardItemModel
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
	
	ProjectsModel( QObject* = 0 );
	virtual ~ProjectsModel();
	
	ProjectItem* item( int, int = 0 ) const;
	ProjectItem* itemFromIndex( const QModelIndex& ) const;
	ProjectItem* takeItem( int, int = 0 );
	ProjectItemList projects( bool = true );
	
	void beginRemoveRows( const QModelIndex&, int, int );
	void endRemoveRows();
	
};

/*
	// project id
	int id() const;
	// get project by id
	static ProjectsModel* byId( int );
	// return a list of all projects
	static QHashProjects all();
	// project open flag
	virtual bool isOpen( const QModelIndex& = QModelIndex() ) const;
	// project modified flag
	virtual bool isModified( const QModelIndex& = QModelIndex() ) const;
	// project readonly flag
	virtual bool isReadOnly( const QModelIndex& = QModelIndex() ) const;
	// project lexer
	virtual QsciLexer* lexer() const;
	// project apis
	virtual QsciAPIs* apis() const;
	// the project name
	virtual QString name( const QModelIndex& = QModelIndex() ) const;
	// the project path
	virtual QString path( const QModelIndex& = QModelIndex() ) const;
	// the project file path
	virtual QString filePath( const QModelIndex& = QModelIndex() ) const;
	// a file path absolute to project path
	virtual QString filePath( const QString&, const QModelIndex& = QModelIndex() );
	// a file path relative to project path
	virtual QString relativeFilePath( const QString&, const QModelIndex& = QModelIndex() );
	// return all the projects files as absolute, recursively according to bool
	virtual QStringList absoluteFilesPath( const QModelIndex& = QModelIndex(), bool = false );
	// get all the subprojects files as absolute, recusively according to bool
	virtual QStringList absoluteSubProjectsPath( const QModelIndex& = QModelIndex(), bool = false ) const;
	// get project of index 
	virtual QModelIndex project( const QModelIndex& = QModelIndex() ) const;
	// return all subprojects index recursively according to bool
	virtual QModelIndexList subProjects( const QModelIndex& = QModelIndex(), bool = false ) const;
	// get the parent project
	virtual QModelIndex parentProject( const QModelIndex& = QModelIndex() ) const;
	// get default codec
	virtual QTextCodec* defaultCodec() const;
	// open base project
	virtual bool open() = 0;
	// the root project item
	virtual QModelIndex rootProject() const = 0;
	// gett all variable content as modelindex list for project index
	virtual QModelIndexList getIndexListValues( const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) const = 0;
	// get variable content as stringlist for project index
	virtual QStringList getListValues( const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) const = 0;
	// get variable content as string for project index
	virtual QString getStringValues( const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) const = 0;
	// set variable content as stringlist for project index
	virtual void setListValues( const QStringList&, const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) = 0;
	// get variable content as string for project index
	virtual void setStringValues( const QString&, const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) = 0;
	// add variable content as stringlist for project index
	virtual void addListValues( const QStringList&, const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) = 0;
	// add variable content as string for project index
	virtual void addStringValues( const QString&, const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) = 0;
	// get the codec used by this project and its files
	virtual QTextCodec* codec( const QModelIndex& = QModelIndex() ) const = 0;
	//
public slots:
	// set project file path
	virtual void setFilePath( const QString&, const QModelIndex& = QModelIndex() );
	// set project modified flag
	virtual void setModified( const QModelIndex& = QModelIndex(), bool = true );
	// set project readonly flag
	virtual void setReadOnly( const QModelIndex& = QModelIndex(), bool = true );
	// set project lexer
	virtual void setLexer( QsciLexer* );
	// set project lexer
	virtual void setAPIs( QsciAPIs* );
	// close project
	virtual void close( const QModelIndex& = QModelIndex() ) = 0;
	// save project
	virtual void save( const QModelIndex& = QModelIndex() ) = 0;
	// save all
	virtual void saveAll() = 0;
	// preapre completion
	virtual void prepareCompletion() = 0;
	//
protected:
	// get recursive files
	virtual QStringList getFiles( QDir, const QStringList& = QStringList() << "*", bool = false );
	//
	static int mUniqueId;
	static QHashProjects mProjectsList;
	int mId;
	QString mRootFilePath;
	QsciLexer* mLexer;
	QsciAPIs* mAPIs;
	//
signals:
	// emit when modified state has changed
	void isModifiedChanged( bool, const QModelIndex& );
	// emit when modified state has changed
	void isReadOnlyChanged( bool, const QModelIndex& );
	// emit when a project is about to close
	void aboutToClose( const QModelIndex& );
	//
};
//
*/

#endif // PROJECTSMODEL_H
