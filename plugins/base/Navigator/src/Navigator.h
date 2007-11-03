#ifndef NAVIGATOR_H
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
