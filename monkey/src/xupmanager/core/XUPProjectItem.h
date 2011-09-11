#ifndef XUPPROJECTITEM_H
#define XUPPROJECTITEM_H

#include <QObject>
#include <QFileInfo>

#include "MonkeyExport.h"

#include "XUPItem.h"
#include "xupmanager/core/ProjectTypesIndex.h"

class QAction;

class XUPPlugin;
class XUPProjectItemCacheBackend;
class XUPProjectItemCache;
class UIXUPEditor;
class pCommand;

typedef QList<class XUPProjectItem*> XUPProjectItemList;

#define XUP_VERSION "1.1.0"

class Q_MONKEY_EXPORT XUPProjectItem : public QObject, public XUPItem
{
	Q_OBJECT
    friend class XUPProjectItemCache;
	friend class DebugDockWidget;
	
public:
	// target type
	enum TargetType { NoTarget = 0, DefaultTarget, DebugTarget, ReleaseTarget };
	
	// ctor
	XUPProjectItem();
	// dtor
	virtual ~XUPProjectItem();
	
	// set last encounter error
	void showError( const QString& error );
	
	// return the project absolute filename
	QString fileName() const;
	// return the project absolute path
	QString path() const;
	// return an absolute file name according to project path
	QString filePath( const QString& filePath ) const;
	// return a filepath relative to project path
	QString relativeFilePath( const QString& filePath ) const;
	
	/* return the list of all source files for this project
	 * Defautl implementation returns empty list
	 */
	QStringList sourceFiles() const;
	// return the list of all source files for all projects from the root project
	QStringList topLevelProjectSourceFiles() const;
	
	// return the xup plugin associated with this project
	XUPPlugin* driver() const;
	
	/* When project is activated (selected as current), some plugins can be also enabled.
	 * for example - PHP-Qt project will activate PHP interpreter plugin.
	 * When project deselected - plugin will be disabled
	 */
	QStringList autoActivatePlugins() const;
	
	/* Add files to the project. 
	 * Optional argument 'scope' allows to add files to the particular part of the project, 
	 * not to the project root.
	 * It allows, for example, to add files to the particular scope in the QMake projects (win32, !unix ...)
	 */
	virtual void addFiles( const QStringList& files, XUPItem* scope = 0 );
	// Remove file, subproject, or other item
	virtual void removeValue( XUPItem* item, bool deleteFiles );
	
	virtual QString quoteString() const;
	virtual QString defaultOperator() const;
	
	// return the direct parent proejct if one, else return itself
	XUPProjectItem* parentProject() const;
	// return the most toplevel project ( ie: the model root project )
	XUPProjectItem* topLevelProject() const;
	// return the parent project for include project ( recursive parent project for include project, else return project itself )
	XUPProjectItem* rootIncludeProject() const;
	// return children project recursively according to bool
	XUPProjectItemList childrenProjects( bool recursive ) const;
	
	// return a list of QFileInfo having corresponding partial file path
	virtual QFileInfoList findFile( const QString& partialFilePath ) const;
	// return all variable items named variableName until caller is found ( if define ) or until the complete tree is scanned
	// if recursive is true, then the scan recurse in each item, else not
	virtual XUPItemList getVariables( const XUPItem* root, const QString& variableName, bool recursive = true, const XUPItem* caller = 0 ) const;
	// return first found variable with name. 0 returned, if not found
	virtual XUPItem* getVariable( const XUPItem* root, const QString& variableName ) const;
	// return the project datas as qstring
	virtual QString toXml() const;
	virtual QString toNativeString() const;
	
	// return the project type id
	virtual QString projectType() const = 0;
	// open a project with codec
	virtual bool open( const QString& fileName, const QString& codec );
	// save the project
	virtual bool save();
	// return the project target file, ie the binary / library / file path, user might be asked for it via dialog
	virtual QString targetFilePath( XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget );
	
	// install custom project actions in menus
	virtual void installCommands();
	// uninstall custom project actions in menus
	virtual void uninstallCommands();
	// return the text codec used by this project
	QString codec() const;
	
	// show project settings dialog
	bool edit();
	
	// show project settings files editor dialog
	bool editProjectFiles();
	
	// return project document filters
	const DocumentFilterMap& documentFilters() const;
	
	/** Add pCommand console commands to the main menu. When menu item triggered - console command executed.
	 * If more than one command set for the menu - commands will be executed one by one
	 * XUPProjectItem remembers create QAction's and deletes it by uninstallCommands()
	 */
	void addSeparator( const QString& mnu );
	void addCommands( const QString& mnu, const QList<pCommand>& cmds );
	pCommand command( int id ) const;
	pCommand command( QAction* action ) const;
	void executeCommand( int id );
	virtual void addCommand( const QString& mnu, const pCommand& cmd );
	
	// return the cache backend used by  cache()
	virtual XUPProjectItemCacheBackend* cacheBackend() const;
	// return the variable value
	QStringList cachedVariableValues( const QString& variableName ) const;
	QString cachedVariableValue( const QString& variableName ) const;
	static XUPProjectItemCache* cache();
	
	QString quotedValue( const QString& value ) const;
	QString unquotedValue( const QString& value ) const;
	
protected:
	QDomDocument mDocument;
	QString mCodec;
	QString mFileName;
	// Action pointers stored here for delete it, when current project changed
	QHash<int, QAction*> mInstalledActions;
	QHash<int, pCommand> mCommands; // project installed commands by id pCommand::id()
	static XUPProjectItemCache mProjectsCache;
	static XUPProjectItemCacheBackend mProjectsCacheBackend;
	
	virtual UIXUPEditor* newEditDialog() const;
	
protected slots:
	// Common handler for actions, which execute pCommand. Does few checks, then executes pCommand
	// Can be overrided if needed.
	virtual void projectCustomActionTriggered();
};

#endif // XUPPROJECTITEM_H
