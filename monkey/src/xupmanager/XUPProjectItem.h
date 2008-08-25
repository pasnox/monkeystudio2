#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include "XUPItem.h"

class Q_MONKEY_EXPORT XUPProjectItem : public XUPItem
{
	Q_OBJECT
	
public:
	
	static QString mProjectSettingsScope;

	XUPProjectItem( const QDomElement = QDomDocument().toElement(), const QString& = QString(), bool = false );
	
	// register item specific infos
	virtual void registerItem() = 0;
	virtual const XUPItemInfos& itemInfos() const;

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


	// return copy of this item
	virtual XUPProjectItem* clone( bool = true ) const = 0;
	
	// return a project settings value
	virtual QStringList projectSettingsValues( const QString& variable, const QStringList& defaultValues = QStringList() ) const;
	virtual QString projectSettingsValue( const QString& variable, const QString& defaultValue = QString() ) const 
		{ return projectSettingsValues( variable, defaultValue.isEmpty() ? QStringList() : QStringList( defaultValue ) ).join( " " ); }
	// set a project setting value
	virtual void setProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void setProjectSettingsValue( const QString& variable, const QString& value ) 
		{ setProjectSettingsValues( variable, value.isEmpty() ? QStringList() : QStringList( value ) ); }
	// add project setting value
	virtual void addProjectSettingsValues( const QString& variable, const QStringList& values );
	virtual void addProjectSettingsValue( const QString& variable, const QString& value ) 
		{ addProjectSettingsValues( variable, value.isEmpty() ? QStringList() : QStringList( value ) ); }
	
	// open project
	virtual bool loadProject( const QString& filename = QString(), const QString& version = QString( "1.0.0" ) );
	// save project
	virtual bool saveProject( const QString& filename = QString(), const QString& version = QString( "1.0.0" ) );

	// close project
	virtual void closeProject();
	
	// return plugin associated with the project
	virtual BuilderPlugin* builder( const QString& plugin = QString() ) const;
	virtual CompilerPlugin* compiler( const QString& plugin = QString() ) const;
	virtual DebuggerPlugin* debugger( const QString& plugin = QString() ) const;
	virtual InterpreterPlugin* interpreter( const QString& plugin = QString() ) const;

	// add a pCommand in menu
	virtual void addCommand( const pCommand& cmd, const QString& mnu );
	// install custom project actions in menus
	virtual void installCommands();
	// uninstall custom project actions in menus
	virtual void uninstallCommands();

protected:
	static XUPItemInfos mXUPItemInfos;

signals:
	void modifiedChanged( XUPProjectItem* item, bool modified );
	void aboutToClose( XUPProjectItem* project );
	void closed( XUPProjectItem* project );
	void installCommandRequested( const pCommand& cmd, const QString& mnu );
	void uninstallCommandRequested( const pCommand& cmd, const QString& mnu );

};

#endif // XUPPROJECTITEM_H
