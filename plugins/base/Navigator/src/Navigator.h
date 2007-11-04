/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : Navigator.h
** Date      : 2007-11-04T22:50:44
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef NAVIGATOR_H
#define NAVIGATOR_H
//
#include <QDockWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHash>
#include <QString>
#include <QTabWidget>
#include <QTreeWidget>

#include "BasePlugin.h"
#include "Ctags.h"
#include "EntityContainer.h"
#include "ProjectsModel.h"
#include "pWorkspace.h"
#include "QSingleton.h"

class ProjectItem;
class pAbstractChild;
// I am not sure with the English language terminology, if you ( reader) know it better, correct it

class Navigator : public BasePlugin, public QSingleton<Navigator>
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	//
private:
	QHash <ProjectItem* , EntityContainer*> projectTrees;
	QHash <QString , EntityContainer*> fileTrees;
	QDockWidget* dockwgt;
	QWidget* fileWidget ;
	QVBoxLayout* fileBox;
	EntityContainer* currFileTreew;
	QPushButton* fileLock;
	int displayMask;
	int expandMask;

public:
	Navigator ();
	bool setEnabled (bool e);
	QWidget* settingsWidget ();

	void setDisplayMask (int mask);
	int getDisplayMask (void);
	void setExpandMask (int mask);
	int getExpandMask (void);

public slots:
	
    void showFile (const QString&);
	void currentFileChanged( pAbstractChild*, const QString);
};
//

#endif //  NAVIGATOR_H
