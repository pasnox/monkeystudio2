/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : Navigator.cpp
** Date      : 2007-11-04T22:50:43
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QPushButton>
#include <QString>
#include<QStringList>
#include <QTabWidget>
#include <QFileInfo>
#include<QDebug>
#include<QModelIndex>

#include "Navigator.h"
#include "pDockToolBar.h"
#include "UIProjectsManager.h"
#include "ProjectsProxy.h"
#include "NavigatorSettings.h"
#include "pSettings.h"  
#include "pWorkspace.h"
#include "ProjectItem.h"
#include "UIMain.h"
#include "pFileManager.h"
#include "pSettings.h"
#include "MonkeyCore.h"

Navigator::Navigator (QObject* )
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Navigator" );
	mPluginInfos.Description = tr( "Plugin uses Exuberant Ctags library for analizing source files. It's allowing to view file structure, and quickly move cursor to needed place" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name =  "Navigator";
	mPluginInfos.Version = "0.0.1";
	mPluginInfos.Enabled = false;
	displayMask = settingsValue( "DisplayMask", 65535 ).toInt();
	expandMask = settingsValue( "ExpandMask", 32771 ).toInt();
}

bool Navigator::setEnabled (bool e)
{
	if (mPluginInfos.Enabled ==e)
		return false;
	mPluginInfos.Enabled = e;
	if (mPluginInfos.Enabled)
	{
		dockwgt = new pDockWidget( MonkeyCore::workspace());
		//dockwgt->hide ();
		dockwgt->setMinimumWidth (100);
		fileWidget = new QWidget (dockwgt);
		fileBox = new QVBoxLayout ( fileWidget);
		fileBox->setMargin( 5 );
		fileBox->setSpacing( 3 );
		currFileTreew = new EntityContainer (fileWidget);
		fileTrees.insert ( NULL, currFileTreew);
		fileBox->addWidget ( currFileTreew);
		fileLock = new QPushButton (tr("Lock view"), fileWidget);
		fileLock->setCheckable ( true );
		fileBox->addWidget (fileLock);
		dockwgt->setWidget (fileWidget);
		MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( dockwgt,  tr( "Navigator" ), QPixmap( ":/icons/redo.png" ) );
		connect ( MonkeyCore::fileManager(), SIGNAL (currentFileChanged( pAbstractChild*, const QString& )) , this, SLOT (currentFileChanged( pAbstractChild*, const QString )));
	}
	else
	{
		disconnect ( MonkeyCore::fileManager(), SIGNAL (currentFileChanged( pAbstractChild*, const QString& )) , this, SLOT (currentFileChanged( pAbstractChild*, const QString& )));
		delete dockwgt;
	}
	return true;
}

QWidget* Navigator::settingsWidget ()
{
	return new NavigatorSettings ();
}

void Navigator::setDisplayMask (int mask)
{
	displayMask = mask;
	setSettingsValue( "DisplayMask", QVariant( mask ) );
}
	
int Navigator::getDisplayMask (void)
{return displayMask;}

void Navigator::setExpandMask (int mask)
{
	expandMask = mask;
	setSettingsValue( "ExpandMask", QVariant( mask ) );	
}

int Navigator::getExpandMask (void)
{return expandMask;}

void Navigator::currentFileChanged(pAbstractChild*, const QString absPath)
{
 	if (fileLock->isChecked())
        return;  //view is locked, do not need to change
	showFile (absPath);
}

void Navigator::showFile (const QString& absPath)
{
	QStringList files (absPath); //  'files' contains list of all paths
	QFileInfo finfo (absPath);
//	QString nameWithoutDot = finfo.path()+"/"+finfo.completeBaseName ();
	//if .cpp for the .h exists
//	if ( ( absPath.endsWith (".h") and ( QFileInfo(nameWithoutDot+".cpp").exists() )))
//		files.append (nameWithoutDot+".cpp");
	//if .h for the .cpp exists
//	else if ( ( absPath.endsWith (".cpp") and ( QFileInfo(nameWithoutDot+".h").exists() )))
//		files.append (nameWithoutDot+".h");
//  	pDockToolBar* bar = mWorkspace->tabToolBar()->bar( TabToolBar::Right );
// 	if (	not bar->isTabRaised (bar->tabIndexOf (dockwgt)) )
// 		return;  //do not need do something, if tab not active
	EntityContainer* oldWidget = currFileTreew; //save current TreeView
 	currFileTreew = fileTrees [absPath]; //Try to find Treew for requested file in the cache
	if ( currFileTreew == NULL ) //not finded
	{
		currFileTreew = new EntityContainer ( NULL);
		fileTrees.insert ( absPath, currFileTreew );
	}//OK, not currFileTreew - actual for requested file
    for ( int i = 0; i< files.size(); i++)
	{
        currFileTreew->updateFileInfo ( files[i] );	
	}
	dockwgt->setWindowTitle (tr("Navigator")+": "+ QFileInfo (absPath).fileName());
	fileWidget->setUpdatesEnabled(false);
	fileBox->removeWidget (oldWidget );
	oldWidget->hide();
	fileBox->insertWidget (0,currFileTreew);
	currFileTreew->show();
/*	if ( currFileTreew->topLevelItemCount () == 0 )
		dockwgt->hide();
	else
		dockwgt->show();
 */
	fileWidget->setUpdatesEnabled(true);
}


Q_EXPORT_PLUGIN2( BaseNavigator, Navigator )
