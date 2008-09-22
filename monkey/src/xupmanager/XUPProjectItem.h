#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include "XUPItem.h"

class BuilderPlugin;
class CompilerPlugin;
class DebuggerPlugin;
class InterpreterPlugin;

class ProjectItemModel;

struct Q_MONKEY_EXPORT XUPProjectItemInfos
{
	XUPProjectItemInfos()
	{ Registered = false; }
	
	QStringList variablesList() const
	{
		QStringList list;
		foreach ( const QString& s, QStringList( FilteredVariables ) << FileVariables << PathVariables << VariableLabels.keys() << VariableIcons.keys() << VariableSuffixes.keys() )
			if ( !list.contains( s ) )
				list << s;
		return list;
	}

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

class Q_MONKEY_EXPORT XUPProjectItem : public XUPItem
{
	Q_OBJECT
	
public:
	
	static QString mProjectSettingsScope;

	XUPProjectItem( const QDomElement = QDomDocument().toElement(), const QString& = QString());
	
	// register item specific infos
	virtual void registerItem() = 0;
	virtual const XUPProjectItemInfos& itemInfos() const;
	
	// return model
	virtual ProjectItemModel* model() const;

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


	virtual XUPProjectItem* clone() const = 0;

	// return the project item of this item (this!!!). Implemented for overloading of XUPItem's function
	virtual XUPProjectItem* project();
	
	// check for sub project to open
	virtual void checkChildrenProjects() {};
	
	// tell if this proejct can ambedeed another projects ( qt subdirs project like )
	virtual bool isProjectContainer() const 
		{ return false; };

	// set item modified state and emit modified signal according to second parameter
	virtual void setModified( bool, bool = true );
	
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
	
	// return the project file path, ie the file u set when opening/saving the project
	virtual QString filePath() const;
	// return the project path
	virtual QString projectPath() const;

	// return relative file path of filepath according to project filepath
	virtual QString relativeFilePath( const QString& ) const;

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
	static XUPProjectItemInfos mXUPProjectItemInfos;
	
	// FIXME remove it, store project path same way, as XUPItem
	QString mProjectFilePath;

signals:
	void modifiedChanged( XUPProjectItem* item, bool modified );
	void aboutToClose( XUPProjectItem* project );
	void closed( XUPProjectItem* project );
	void installCommandRequested( const pCommand& cmd, const QString& mnu );
	void uninstallCommandRequested( const pCommand& cmd, const QString& mnu );

};

#endif // XUPPROJECTITEM_H
