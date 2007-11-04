/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : MonkeyProjectItem.h
** Date      : 2007-11-04T22:53:36
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include "ProjectItem.h"
#include "BuilderPlugin.h"
//class ProjectPlugin;

class GenericBuilder : public BuilderPlugin //fake builder, for build menu
{
	pCommand defaultBuildCommand() const {return pCommand();};
	pCommand buildCommand() const {return pCommand();};
	void setBuildCommand( const pCommand& ) {};
	QWidget* builderSettingsWidget() {return NULL;};
	pCommandList defaultCommands() const {return pCommandList();};
	pCommandList userCommands() const {return pCommandList();};
	void setUserCommands( const pCommandList& ) const {};
	QWidget* cliToolSettingsWidget( BasePlugin* p ){ return NULL;}
	QStringList availableParsers() const {return QStringList();};
	pCommandParser* getParser( const QString& ) {return NULL;};
	void commandTriggered() {};
};

class UIMonkeyProjectSettings;
class pAction;
class QMenu;

class MonkeyProjectItem : public ProjectItem
{
Q_OBJECT
struct Target
{
	QString text;
	QString command;
	pAction* action;  //may be NULL or valid pointer
};
	friend class UIMonkeyProjectSettings;
	
public:
	MonkeyProjectItem (  ProjectItem::NodeType = ProjectItem::ProjectType, ProjectItem* = 0  );
	~MonkeyProjectItem ();
	
	void close ();
	void remove() ;
	bool open();

    void addExistingFiles(const QStringList&, const QString&, const QString& ="");
    void addExistingFiles(const QStringList&, ProjectItem*, const QString& ="");
    void addExistingFile(const QString&, const QString&, const QString& ="");
    void addExistingFile(const QString&, ProjectItem*, const QString& ="");

    void appendRow(ProjectItem*);
    void insertRow(int, ProjectItem*);

	virtual QString getIndent() const {}
	virtual QString getEol() const {}
	virtual bool isFirst() const {}
	virtual bool isLast() const {}
	virtual QString scope() const {}
	virtual QString checkScope(const QString&) const {}
	virtual bool isEqualScope(const QString&, const QString&) const {}
	virtual bool isProjectsContainer() const {}
	virtual bool swapRow(int, int) {}
	virtual bool moveRowUp(int) {}
	virtual bool moveRowDown(int) {}
	virtual bool moveUp() {}
	virtual bool moveDown() {}
	virtual bool addProject(const QString&) {}
	virtual void saveAll(bool) {}
	virtual void setCompiler(CompilerPlugin*) {}
	virtual CompilerPlugin* compiler() const {return NULL;}
	virtual void setDebugger(DebuggerPlugin*) {}
	virtual DebuggerPlugin* debugger() const {return NULL;}
	virtual void setInterpreter(InterpreterPlugin*) {}
	virtual InterpreterPlugin* interpreter() const {return NULL;}
	virtual void debug() {}
	virtual ProjectItem* getItemScope(const QString&, bool) const {}
	virtual QStringList getListValues(const QString&, const QString&, const QString&) const {}
	virtual QString getStringValues(const QString&, const QString&, const QString&) const {}
	virtual void setListValues(const QStringList&, const QString&, const QString&, const QString&) {}
	virtual void setStringValues(const QString&, const QString&, const QString&, const QString&) {}
	virtual void addListValues(const QStringList&, const QString&, const QString&, const QString&) {}
	virtual void addStringValues(const QString&, const QString&, const QString&, const QString&) {}
	virtual void redoLayout(ProjectItem*) {}
	virtual bool writeProject() {}
	virtual void writeItem(ProjectItem*) {}
	virtual bool addProject(const QString&, ProjectItem*, const QString&) {}
	
	// P@sNox, Fix complete build
	virtual void setValues(ProjectItem*, const QString&, const QString&, const QStringList&) {}
	virtual void addValue( ProjectItem*, const QString&, const QString&, const QString& ) {}
	virtual ProjectItemList getValues(ProjectItem*, const QString&, const QString&) const { return ProjectItemList(); }
	
	
public slots:
	void editSettings();
	void buildMenuTriggered ();
	void installCommands ();
	void uninstallCommands ();
	ProjectPlugin* getParentPlugin ();
	void save( bool = true );

private:
	QList<Target> targets;
};

#endif // NOPROJECTPROJECTITEM_H
