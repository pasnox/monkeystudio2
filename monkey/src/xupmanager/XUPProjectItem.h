#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include "XUPItem.h"

class Q_MONKEY_EXPORT XUPProjectItem : public XUPItem
{
	Q_OBJECT
	
public:
	
	static QString mProjectSettingsScope;

	XUPProjectItem( const QDomElement = QDomDocument().toElement(), const QString& = QString(), bool = false );
	
	// return copy of this item
	virtual XUPProjectItem* clone( bool = true ) const;
	
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

signals:
	void modifiedChanged( XUPProjectItem* item, bool modified );
	void aboutToClose( XUPProjectItem* project );
	void closed( XUPProjectItem* project );
	void installCommandRequested( const pCommand& cmd, const QString& mnu );
	void uninstallCommandRequested( const pCommand& cmd, const QString& mnu );

};

#endif // XUPPROJECTITEM_H
