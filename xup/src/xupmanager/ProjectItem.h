#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QObject>
#include <QStandardItem>

#include <QDomDocument>
#include <QDomElement>

class ProjectItemModel;

class ProjectItem : public QObject, public QStandardItem
{
	Q_OBJECT
	
public:
	ProjectItem( const QDomElement = QDomElement(), const QString& = QString(), bool = false, ProjectItem* = 0 );

	// register item specific infos
	static void registerItem() { qWarning( qPrintable( tr( "ProjectItem Registered" ) ) ); }
	// the visible variables in filtered view ordered by list order
	virtual QStringList filteredVariables() const { return QStringList(); }

	// return child item
	virtual ProjectItem* child( int, int = 0 ) const;
	// return parent item
	virtual ProjectItem* parent() const;
	// return model
	virtual ProjectItemModel* model() const;
	// return copy of this item
	virtual ProjectItem* clone( bool = true ) const;
	// append row
	virtual void appendRow( ProjectItem* );
	// insert row
	virtual void insertRow( int, ProjectItem* );
	// get project children items, recursively according to bool and same project according to bool
	virtual QList<ProjectItem*> children( bool = true, bool = true ) const;

	// set the item QDomElement
	virtual void setDomElement( const QDomElement& );
	virtual QDomElement domElement() const;
	// return a QDomDocument about the current item and all its children
	virtual QDomDocument toDomDocument();

	// interpret a variable content based on it s name, search will end if ProjectItem parameter is encounteror until last item
	virtual QString interpretedVariable( const QString&, const ProjectItem* = 0, const QString& = QString() ) const;
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
	void setModified( bool, bool = true );
	/*
	// return buddy item
	virtual ProjectItem* buddy() const;
	// set buddy item
	virtual void setBuddy( ProjectItem* );
	// return data, take care of buddy if need
	virtual QVariant data( int = Qt::UserRole +1 ) const;
	*/

	// check for sub project to open
	virtual void checkChildrenProjects();
	// open project
	virtual bool loadProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	// save project
	virtual bool saveProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	// close project
	virtual void closeProject();

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
	virtual ProjectItem* project() const;
	// return the top level project
	virtual ProjectItem* topLevelProject() const;
	
protected:
	//ProjectItem* mBuddy;
	QDomElement mDomElement;
	QString mProjectFilePath;
	bool mModified;

signals:
	void modifiedChanged( ProjectItem*, bool );
};

#endif // PROJECTITEM_H
