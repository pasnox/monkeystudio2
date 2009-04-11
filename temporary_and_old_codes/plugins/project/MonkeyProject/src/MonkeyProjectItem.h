/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : MonkeyProjectItem.h
** Date      : 2008-01-14T00:54:07
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#ifndef NOPROJECTPROJECTITEM_H
#define NOPROJECTPROJECTITEM_H

#include <pluginsmanager.h>

class GenericBuilder : public BuilderPlugin //fake builder, for build menu
{
	pCommand defaultBuildCommand() const {return pCommand();};
	pCommand buildCommand() const {return pCommand();};
	void setBuildCommand( const pCommand& ) {};
	QWidget* builderSettingsWidget() {return NULL;};
	pCommandList defaultCommands() const {return pCommandList();};
	pCommandList userCommands() const {return pCommandList();};
	void setUserCommands( const pCommandList& ) const {};
	QWidget* cliToolSettingsWidget( BasePlugin* ){ return NULL;}
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
	Target( const QString& t, const QString& c, pAction* a )
		: text( t ), command( c ), action( a )
	{}
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

	virtual QString getIndent() const { return QString(); }
	virtual QString getEol() const { return QString(); }
	virtual bool isFirst() const { return false; }
	virtual bool isLast() const { return false; }
	virtual QString scope() const { return QString(); }
	virtual QString checkScope(const QString& s) const { return s; }
	virtual bool isEqualScope(const QString&, const QString&) const { return true;}
	virtual bool isProjectsContainer() const { return false; }
	virtual bool swapRow(int, int) { return true; }
	virtual bool moveRowUp(int) { return true; }
	virtual bool moveRowDown(int) { return true; }
	virtual bool moveUp() { return true; }
	virtual bool moveDown() { return true; }
	virtual bool addProject(const QString&) { return false; }
	virtual void saveAll(bool) {}
	virtual void setCompiler(CompilerPlugin*) {}
	virtual CompilerPlugin* compiler() const {return NULL;}
	virtual void setDebugger(DebuggerPlugin*) {}
	virtual DebuggerPlugin* debugger() const {return NULL;}
	virtual void setInterpreter(InterpreterPlugin*) {}
	virtual InterpreterPlugin* interpreter() const {return NULL;}
	virtual void debug() {}
	virtual ProjectItem* getItemScope(const QString&, bool) const { return 0; }
	virtual QStringList getListValues(const QString&, const QString&, const QString&) const { return QStringList(); }
	virtual QString getStringValues(const QString&, const QString&, const QString&) const { return QString(); }
	virtual void setListValues(const QStringList&, const QString&, const QString&, const QString&) {}
	virtual void setStringValues(const QString&, const QString&, const QString&, const QString&) {}
	virtual void addListValues(const QStringList&, const QString&, const QString&, const QString&) {}
	virtual void addStringValues(const QString&, const QString&, const QString&, const QString&) {}
	virtual void redoLayout(ProjectItem*) {}
	virtual bool writeProject() { return true; }
	virtual void writeItem(ProjectItem*) {}
	virtual bool addProject(const QString&, ProjectItem*, const QString&) { return false; }
	
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
