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
	ProjectItem( const QDomElement& = QDomElement(), const QString& = QString(), bool = false );

	// register item specific infos
	static void registerItem() { qWarning( qPrintable( tr( "ProjectItem Registered" ) ) ); }

	// return child item
	virtual ProjectItem* child( int, int = 0 ) const;
	// return parent item
	virtual ProjectItem* parent() const;
	// return model
	virtual ProjectItemModel* model() const;
	// return copy of this item
	virtual ProjectItem* clone() const;
	// append row
	virtual void appendRow( ProjectItem* );
	// get project children items, recursively according to bool and same project according to bool
	virtual QList<ProjectItem*> children( bool = true, bool = true ) const;
	// check for sub proejct to open
	virtual void checkChildrenProjects();

	// set the item QDomElement
	void setDomElement( const QDomElement& );
	QDomElement domElement() const;
	// return a QDomDocument about the current item and all its children
	QDomDocument toDomDocument();

	// return the item value name, ie: the name of the value to read to get the item value
	QString valueName() const;
	// set domelement attribute
	void setValue( const QString&, const QString& );
	// get domelement attribute
	QString value( const QString&, const QString& = QString() ) const;
	// get the item default value, ie: the value return by valueName() attribute
	QString defaultValue( const QString& = QString() ) const;
	
	// return item modified state
	bool modified() const;
	// set item modified state and emit modified signal according to second parameter
	void setModified( bool, bool = true );

	// open project
	virtual bool loadProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );
	// save project
	virtual bool saveProject( const QString& = QString(), const QString& = QString( "1.0.0" ) );

	// return the project file path, ie the file u set when opening/saving the project
	QString projectFilePath() const;
	// return the project path
	QString projectPath() const;
	// if item is a value, and it s variable is file or path based, return the full file path of the value, else return a file path according to project path for parameter
	QString filePath( const QString& = QString() );
	
protected:
	QDomElement mDomElement;
	QString mProjectFilePath;
	bool mModified;

signals:
	void modifiedChanged( ProjectItem*, bool );

};

#endif // PROJECTITEM_H
