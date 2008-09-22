#ifndef XUPITEM_H
#define XUPITEM_H

#include <fresh.h>

#include "../consolemanager/pCommand.h"

#include <QObject>
#include <QStandardItem>

#include <QDomDocument>
#include <QDomElement>
#include <QIcon>
#include <QMultiHash>

class XUPProjectItem;

class BuilderPlugin;
class CompilerPlugin;
class DebuggerPlugin;
class InterpreterPlugin;


class ProjectItemModel;

class Q_MONKEY_EXPORT XUPItem : public QObject, public QStandardItem
{
	Q_OBJECT
	
public:
	
	// get defaut icon
	virtual QIcon getIcon( const QString&, const QString& ) const;
	
	// return item datas
	virtual QVariant data( int role = Qt::UserRole +1 ) const;
	// return child item
	virtual XUPItem* child( int, int = 0 ) const;
	// return parent item
	virtual XUPItem* parent() const;
	// return model
	virtual ProjectItemModel* model() const;
	// append row
	virtual void appendRow( XUPItem* );
	// insert row
	virtual void insertRow( int, XUPItem* );
	// get project children items, recursively according to bool and same project according to bool
	virtual QList<XUPItem*> children( bool = true, bool = true ) const;
	// remove item from parent, and change project modified flag
	virtual void remove();

	/* FIXME (AndreyK)
	   Now, as I understanding, files can be added to a item (SOURCES, HEADERS)
	   Let's it will be added to a Project, and it should define self, what to do with files
	   For me, XUPItem should not have next 5 methods
	*/
	// return all value files if item if a variable based on files
	virtual QStringList files( bool absolute = true );
	// add files to project
	virtual void addFiles( const QStringList& files, XUPItem* scope = 0, const QString& op = QString( "=" ) );
	virtual void addFile( const QString& file, XUPItem* scope = 0, const QString& op = QString( "=" ) ) { addFiles( QStringList( file ), scope, op ); };
	// remove files to project
	virtual void removeFiles( const QStringList& files, XUPItem* scope = 0, const QString& op = QString( "=" ) );
	virtual void removeFile( const QString& file, XUPItem* scope = 0, const QString& op = QString( "=" ) ) { removeFiles( QStringList( file ), scope, op ); };

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
	// tell how to read node for s
	virtual QString valueName( const QString& ) const;
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
	// return the variable values if item is a variable else nothing
	virtual QStringList variableValues() const;
	virtual QString variableValue() { return variableValues().join( " " ); }
	// set a variable value
	virtual void setVariableValues( const QStringList& values );
	virtual void setVariableValue( const QString& value ) { setVariableValues( value.isEmpty() ? QStringList() : QStringList( value ) ); }
	// add variable value
	virtual void addVariableValues( const QStringList& values );
	virtual void addVariableValue( const QString& value ) { addVariableValues( value.isEmpty() ? QStringList() : QStringList( value ) ); }
	// remove variable value
	virtual void removeVariableValues( const QStringList& values );
	virtual void removeVariableValue( const QString& value ) { removeVariableValues( value.isEmpty() ? QStringList() : QStringList( value ) ); }

	// return the project file path, ie the file u set when opening/saving the project
	virtual QString projectFilePath() const;
	// return the project path
	virtual QString projectPath() const;
	// if item is a value, and it s variable is file or path based, return the full file path of the value, else return a file path according to project path for parameter
	virtual QString filePath( const QString& = QString() ) const;
	// return relative file path of filepath according to project filepath
	virtual QString relativeFilePath( const QString& = QString() ) const;
	
	// tell if this item is a project
	virtual bool isProject() const;
	// tell if this item is type
	virtual bool isType( const QString& ) const;
	// return the project item of this item
	virtual XUPProjectItem* project() const;
	// return the top level project if available
	virtual XUPProjectItem* topLevelProject() const;
	
	// return the most approppriate parent project for includes projects
	// FIXME Do this function realy need ??? Maybe need some refactoring
	virtual XUPProjectItem* topProjectForInclude() const;
	
	// return a scope by its name from item, creating it if needed
	virtual XUPItem* scope( const QString& scopeName, XUPItem* fromScope = 0, bool create = false ) const;
	// return the first variable by its name & operator from item, creating it if needed
	virtual XUPItem* variable( const QString& variableName, const QString& operatorName = QString( "=" ), XUPItem* fromScope = 0, bool create = false ) const;
		
protected:
	
	QDomDocument mDocument;
	QDomElement mDomElement;
	QString mProjectFilePath;
	bool mModified;
	
	QMultiHash<QString, pCommand> mCommands;

signals:
	void modifiedChanged( XUPItem* item, bool modified ); // FIXME Do item must have this signal ?
};

#endif // XUPITEM_H
