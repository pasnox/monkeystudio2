/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Andrei Kopats aka hlamer <hlamer at tut by>
** Project   : Monkey Studio Base Plugins
** FileName  : Navigator.cpp
** Date      : 2008-01-14T00:40:08
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
/*!
	\file Navigator.cpp
	\date 2008-01-14T00:40:08
	\author Andrei Kopats
	\brief Implementation of main class of MessageBox plugin
*/

#include <QPushButton>
#include <QString>
#include<QStringList>
#include <QTabWidget>
#include <QFileInfo>
#include<QDebug>
#include<QModelIndex>

#include <coremanager.h>
#include <workspacemanager.h>
#include <maininterface.h>

#include "Navigator.h"
#include "NavigatorSettings.h"

/*!
	Class constructor
*/
Navigator::Navigator (QObject* )
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Navigator" );
	mPluginInfos.Description = tr( "Plugin uses Exuberant Ctags library for analizing source files. It's allowing to view file structure, and quickly move cursor to needed place" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name =  PLUGIN_NAME;
	mPluginInfos.Version = "0.0.1";
	mPluginInfos.Enabled = false;
	displayMask = settingsValue( "DisplayMask", 65535 ).toInt();
	expandMask = settingsValue( "ExpandMask", 32771 ).toInt();
}

/*!
	Enable/disable plugin
	\param e Flag. Enable = true, Disable = false
	\return Status of process 
	\retval true Successfully enabled
	\retval false Some error ocurred
*/
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

/*!
	Get settings widget for configuring plugin
	\return Pointer to widget
*/
QWidget* Navigator::settingsWidget ()
{
	return new NavigatorSettings ();
}

/*!
	Set display mask for displaying items in the tree. 
	Mask will be stored in the settings
	
	\param mask Mask should contain logical OR of types of entityes, which should be 
		displayed in the tree. See EntityType for description of entity types
*/
void Navigator::setDisplayMask (int mask)
{
	displayMask = mask;
	setSettingsValue( "DisplayMask", QVariant( mask ) );
}
	
/*!
	Get display mask of plugin.
	\return Display mask
	\retval Logical OR of types on Entityes, which should be displayed
*/
int Navigator::getDisplayMask (void)
{return displayMask;}

/*!
	Set expand mask for automatical expanding some nodes in the tree. 
	Mask will be stored in the settings
	
	\param mask Mask should contain logical OR of types of entityes, which should be 
		automatically expanded. See EntityType for description of entity types
*/
void Navigator::setExpandMask (int mask)
{
	expandMask = mask;
	setSettingsValue( "ExpandMask", QVariant( mask ) );	
}

/*!
	Get expand mask of plugin.
	\return Expand mask
	\retval Logical OR of types on Entityes, which should be automatically expanded
*/
int Navigator::getExpandMask (void)
{return expandMask;}

/*!
	Signal handler, which switches current file in the view, according with 
	current editing file
	\param absPath Absolute file path of new file
*/
void Navigator::currentFileChanged(pAbstractChild*, const QString absPath)
{
	if (fileLock->isChecked())
		return;  //view is locked, do not need to change
	showFile (absPath);
}

/*!
	External interface for display file in the Navigator
	\return Display absPath Absolute path of file, which should be displayed
*/
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
