/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Andrei Kopats aka hlamer <hlamer at tut by>
** Project   : Monkey Studio Base Plugins
** FileName  : Navigator.h
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
	\file Navigator.h
	\date 2008-01-14T00:40:08
	\author Andrei Kopats
	\brief Header file for Navigator plugin
*/
#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QHash>
#include <QString>
#include <QTabWidget>
#include <QTreeWidget>

#include <pluginsmanager.h>

#include "EntityContainer.h"

class ProjectItem;
class pAbstractChild;
// I am not sure with the English language terminology, if you ( reader) know it better, correct it

/*!
	\brief plugin main class

	Navigator class implements interface of plugin, GUI, stores settings of plugin
*/
class Navigator : public BasePlugin, public QSingleton<Navigator>
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	friend class QSingleton<Navigator>;
	//
protected:
	pDockWidget* dockwgt;
	QHash <ProjectItem* , EntityContainer*> projectTrees;
	QHash <QString , EntityContainer*> fileTrees;
	QWidget* fileWidget ;
	QVBoxLayout* fileBox;
	EntityContainer* currFileTreew;
	QPushButton* fileLock;
	int displayMask;
	int expandMask;
    
    // Setting. Hide dock, when user selected something
    bool mAutoHide;

public:
	Navigator (QObject* parent = NULL);

	bool setEnabled (bool e);
	QWidget* settingsWidget ();

	void setDisplayMask (int mask);
	int getDisplayMask (void);
	void setExpandMask (int mask);
	int getExpandMask (void);
	void setAutoHide (bool);
	bool getAutoHide (void);
    
    void setDockVisible (bool);
    
public slots:
	
	void showFile (const QString&);
	void currentFileChanged( pAbstractChild*, const QString);
};
//

#endif //  NAVIGATOR_H
