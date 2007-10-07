/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : MonkeyProject project type 
** FileName  : MonkeyProjectItem.h
** Date      : 2007-09-28
** License   : GPL
** Comment   : MonkeyProject project  class
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include "ProjectItem.h"

//class ProjectPlugin;
class UIMonkeyProjectSettings;
class QAction;
class QMenu;

class MonkeyProjectItem : public ProjectItem
{
Q_OBJECT
struct Target
{
	QString text;
	QString command;
	QAction* action;  //may be NULL or valid pointer
};
	friend class UIMonkeyProjectSettings;
	
public:
	MonkeyProjectItem (  ProjectItem::NodeType = ProjectItem::ProjectType, ProjectItem* = 0  );
	~MonkeyProjectItem ();
	
	void close ();
	void remove() ;
	bool open();

	virtual QString getIndent() const {}
	virtual QString getEol() const {}
	virtual bool isFirst() const {}
	virtual bool isLast() const {}
	virtual QString scope() const {}
	virtual QString checkScope(const QString&) const {}
	virtual bool isEqualScope(const QString&, const QString&) const {}
	virtual bool isProjectsContainer() const {}
	virtual void appendRow(ProjectItem*) {}
	virtual void insertRow(int, ProjectItem*) {}
	virtual bool swapRow(int, int) {}
	virtual bool moveRowUp(int) {}
	virtual bool moveRowDown(int) {}
	virtual bool moveUp() {}
	virtual bool moveDown() {}
	virtual bool addProject(const QString&) {}
	virtual void saveAll(bool) {}
	virtual void addExistingFiles(const QStringList&, const QString&, const QString&) {}
	virtual void addExistingFiles(const QStringList&, ProjectItem*, const QString&) {}
	virtual void addExistingFile(const QString&, const QString&, const QString&) {}
	virtual void addExistingFile(const QString&, ProjectItem*, const QString&) {}
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

	
public slots:
	void editSettings();
	void buildMenuTriggered ();
	void installCommands ();
	void uninstallCommands ();
	ProjectPlugin* getParentPlugin ();
	void save( bool = true );

private:
	QList<Target> targets;
	QString projectPath;
};

#endif // NOPROJECTPROJECTITEM_H
