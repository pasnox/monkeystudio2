#ifndef XUPITEM_H
#define XUPITEM_H

#include <QObject>
#include <QStandardItem>

#include <QDomDocument>
#include <QDomElement>
#include <QIcon>
#include <QHash>

struct XUPItemInfos
{
	XUPItemInfos()
	{ Registered = false; }

	bool Registered;
	QStringList Operators;
	QStringList FilteredVariables;
	QStringList TextTypes;
	QStringList FileVariables;
	QStringList PathVariables;
	QHash<QString, QStringList> Suffixes;
	QHash<QString, QString> VariableLabels;
	QHash<QString, QIcon> VariableIcons;
	QHash<QString, QStringList> VariableSuffixes;
};

class ProjectItemModel;

class XUPItem : public QObject, public QStandardItem
{
	Q_OBJECT
	
public:
	XUPItem( const QDomElement = QDomDocument().toElement(), const QString& = QString(), bool = false );

	// register item specific infos
	virtual void registerItem();

	// get available operators for this kind of item
	virtual QStringList operators() const;
	virtual void registerOperator( const QString& op );
	
	// the visible variables in filtered view ordered by list order
	virtual QStringList filteredVariables() const;
	virtual void registerFilteredVariables( const QString& );
	
	// tell if node is text based
	virtual QStringList textTypes() const;
	virtual void registerTextType( const QString& );
	
	// tell witch variables are file based
	virtual QStringList fileVariables() const;
	virtual void registerFileVariables( const QString& );
	
	// tell witch variables are path based
	virtual QStringList pathVariables() const;
	virtual void registerPathVariables( const QString& );
	
	// register extension that this item can manage
	virtual QHash<QString, QStringList> suffixes() const;
	virtual void registerSuffixes( const QString& label, const QStringList& suffixes );
	
	// set variables labels
	virtual QHash<QString, QString> variableLabels() const;
	virtual void registerVariableLabels( const QString&, const QString& );
	
	// set variables icons
	virtual QHash<QString, QIcon> variableIcons() const;
	virtual void registerVariableIcons( const QString&, const QIcon& );
	
	// set suffixes that some variables can handle
	virtual QHash<QString, QStringList> variableSuffixes() const;
	virtual void registerVariableSuffixes( const QString& varname, const QStringList& suffixes );
	
	// get defaut icon
	virtual QIcon getIcon( const QString&, const QString& ) const;
	
	// tell how to read node for s
	virtual QString valueName( const QString& ) const;
	
	// return all value files if item if a variable based on files
	virtual QStringList files( bool absolute = true );
	
	// update item
	virtual void updateItem();

	// return child item
	virtual XUPItem* child( int, int = 0 ) const;
	// return parent item
	virtual XUPItem* parent() const;
	// return model
	virtual ProjectItemModel* model() const;
	// return copy of this item
	virtual XUPItem* clone( bool = true ) const;
	// append row
	virtual void appendRow( XUPItem* );
	// insert row
	virtual void insertRow( int, XUPItem* );
	// get project children items, recursively according to bool and same project according to bool
	virtual QList<XUPItem*> children( bool = true, bool = true ) const;
	// remove item from parent, and change project modified flag
	virtual void remove();

	// set the item QDomElement
	virtual void setDomElement( const QDomElement& );
	virtual QDomElement domElement() const;
	// return a QDomDocument about the current item and all its children
	virtual void setDomDocument( const QDomDocument& );
	virtual QDomDocument domDocument() const;

	// interpret a variable content based on it s name, search will end if XUPItem parameter is encounteror until last item
	virtual QString interpretedVariable( const QString&, const XUPItem* = 0, const QString& = QString() ) const;
	// get the item default value and interpret it s content, ie: the value return by valueName() attribute
	virtual QString defaultInterpretedValue() const;
	// return the item value name, ie: the name of the value to read to get the item value
	virtual QString valueName() const;
	// set domelement attribute
	virtual void setValue( const QString&, const QString& );
	// get domelement attribute
	virtual QString value( const QString&, const QString& = QString() ) const;
	// get the item default value, ie: the value return by valueName() attribute
	virtual QString defaultValue( const QString& = QString() ) const;
	// return item modified state
	virtual bool modified() const;
	// set item modified state and emit modified signal according to second parameter
	virtual void setModified( bool, bool = true );

	// check for sub project to open
	virtual void checkChildrenProjects();
	// tell if this proejct can ambedeed another projects ( qt subdirs project like )
	virtual bool isProjectContainer() const;
	// open project
	virtual bool loadProject( const QString& filename = QString(), const QString& version = QString( "1.0.0" ) );
	// save project
	virtual bool saveProject( const QString& filename = QString(), const QString& version = QString( "1.0.0" ) );
	// close project
	virtual void closeProject();
	// add files to project
	virtual void addFiles( const QStringList& files, XUPItem* scope = 0, const QString& op = QString( "=" ) );
	virtual void addFile( const QString& file, XUPItem* scope = 0, const QString& op = QString( "=" ) ) { addFiles( QStringList( file ), scope, op ); };
	// remove files to project
	virtual void removeFiles( const QStringList& files, XUPItem* scope= 0, const QString& op = QString( "=" ) );
	virtual void removeFile( const QString& file, XUPItem* scope = 0, const QString& op = QString( "=" ) ) { removeFiles( QStringList( file ), scope, op ); };

	// return the project file path, ie the file u set when opening/saving the project
	virtual QString projectFilePath() const;
	// return the project path
	virtual QString projectPath() const;
	// if item is a value, and it s variable is file or path based, return the full file path of the value, else return a file path according to project path for parameter
	virtual QString filePath( const QString& = QString() );
	// return relative file path of filepath according to project filepath
	virtual QString relativeFilePath( const QString& = QString() );
	
	// tell if this item is a project
	virtual bool isProject() const;
	// tell if this item is type
	virtual bool isType( const QString& ) const;
	// return the project item of this item
	virtual XUPItem* project() const;
	// return the most approppriate parent project for includes projects
	virtual XUPItem* topProjectForInclude() const;
	
protected:
	static XUPItemInfos mXUPItemInfos;

	//XUPItem* mBuddy;
	QDomDocument mDocument;
	QDomElement mDomElement;
	QString mProjectFilePath;
	bool mModified;

signals:
	void modifiedChanged( XUPItem*, bool );
	void aboutToClose( XUPItem* );
	void closed( XUPItem* );
};

#endif // XUPITEM_H
