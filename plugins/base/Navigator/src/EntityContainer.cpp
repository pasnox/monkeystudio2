/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : EntityContainer.cpp
** Date      : 2008-01-14T00:40:06
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
	\file EntityContainer.h
	\date 2008-01-14T00:40:08
	\author Andrei Kopats
	\brief Header file for EntityContainer class
*/
#include <QString>
#include <QDateTime>
#include <QDebug>

#include <workspacemanager.h>

#include "EntityContainer.h"
#include "Entity.h"
#include "Navigator.h"

/*!
	Constructor of class
	
	Creates object, launches initialisation of icons of Entity class
	\param parent Parent widget
*/
EntityContainer::EntityContainer ( QWidget* parent, Navigator* navigator): 
    QTreeWidget (parent),
    mNavigator (navigator)
{
	headerItem ()->setHidden (true);
	connect (this, SIGNAL (clicked ( const QModelIndex)),this, SLOT (makeGoto()));
	Entity::initIcons ();
}

/*!
	Destuctor of class
*/
EntityContainer::~EntityContainer ()
{
	disconnect (this, SIGNAL (clicked ( const QModelIndex)),this, SLOT (makeGoto()));
}

/*!
	Return child as pointer to Entity
	\param i Child number
	\return Pointer to child
*/
Entity* EntityContainer::childEntity (int i)
{
	return (Entity*)topLevelItem(i);
}

/*!
	Remove old information about file

	Using for update info without recreation of all tree. New information are
	adding, than old removing.
	\param file file, for which information should be removed
	\param olderThan Time stamp. Information removing, if it older than this
		stamp
*/
void EntityContainer::deleteFileInfo ( QString file, QDateTime olderThan )
{
	Entity* chld;
	for ( int i = 0; i < topLevelItemCount (); i++)
	{
		chld = childEntity(i);
		chld->deleteFileInfo ( file, olderThan );
		if ( chld->file == file && 
			(  chld->updateTime < olderThan || olderThan.isNull() )) //need to delete this entity
		{
			//if entity was have childs, that must be deleted, it's had deleted by chld->deleteFileInfo (... )
			//if entity have childs now, it must not be deleted.
			if ( chld->childCount() == 0 )
			{
				delete ( chld);
				i--;
			}
			else //we can't destroy item, having childs, because childs must have parent
			{
				file = QString::null;
				chld->line = 0;//will not try to show null line
				chld->updateTime = QDateTime (); //null time
				chld->signature = QString::null; //without signature
			}
		}
	}
}

/*!
	Process record for file and add info from it to self
	\param fileName Name of file, for which information is reading
	\param fileRecord Record, containing information about file
*/
void EntityContainer::addTagsFromRecord (QString fileName, FileRecord*  fileRecord)
{
	TagEntryListItem* item = fileRecord->firstTagEntry;
	int displayMask = Navigator::instance()->getDisplayMask();
	while ( item != NULL )
	{
		tagEntryInfo* entry = &item->tag;    
		item = item->next ;
		EntityType entType = Entity::getEntityType (entry->kind);
		if ( ! (entType & displayMask) )
				continue; // if mask not set for it's entity - ignore it 
			Entity* parEnt = getScopeEntity ( entry->extensionFields.scope[0], entry->extensionFields.scope[1]);
			addChild ( parEnt, entry,fileName,fileRecord->time );
	}
};

/*!
	Update information about file (recieve newest information)
	\param fileName Full path of file
*/
void EntityContainer::updateFileInfo ( QString fileName )
{
	FileRecord* rd = Ctags::instance()->GetTagsForFile (fileName); 
	addTagsFromRecord(fileName, rd);//add of update existing entityes
	deleteFileInfo (fileName, rd->time);//deltete not updated  
}

/*!
	Search entity, which according to scopes 

	For example - search entity, according to class, if scope is class
	If entity is not finded - new one with incomplete info will be created
	\param scope0 Scope value from Ctags
	\param scope1 Scope value from Ctags
	\return Pointer to Entity
*/
Entity*EntityContainer::getScopeEntity ( QString scope0, QString scope1)
{
	//qDebug ( qPrintable ("trying to find scope| " + scope0 +"|"+ scope1 +"| in " ));
	if    ( scope0 == NULL && scope1 == NULL )
		return NULL;
	
	EntityType ttype = Entity::getScopeType ( scope0 );
	Entity* scopeEntity = NULL;

	QStringList scopes = scope1.split ("::"); // Ctags::FileEntry ->  [Ctags, FileEntry]
	//step 1.   To find first level scope in this
	for ( int i = 0; i < topLevelItemCount(); i++)
	{
		if ( /* (childEntity (i)->type == ttype   or scopes.size() >1)  //we do not know type of not top level items
				and*/ childEntity (i)->name == scopes[0] )// FIXME uncomment scope type checking if will have problems, and will have another problems;)
		{
			scopeEntity = childEntity (i);
			break;//finded
		}
	}
	if ( scopeEntity == NULL) //if not finded - need to create
	{
		scopeEntity = new Entity ( UNKNOWN, scopes[0]);
		addTopLevelItem ( scopeEntity);
		scopeEntity->setExpanded (Navigator::instance()->getExpandMask() & scopeEntity->type);
	}
	scopes.removeAt (0);
	
	//step 2.  To find not first and not last scopes with unknown type
	Entity* findResult = NULL;
	while ( scopes.size() > 0)
	{
		//qDebug ( qPrintable ("while for parent "+name ));
		findResult = NULL;
		for ( int i = 0; i< scopeEntity->childCount(); i++) //find in the childrens
			if ( scopeEntity->child(i)->name == scopes[0] )  //check self
			{
				//qDebug ( qPrintable ("finded"+tname+" . It self" + name ));    
				findResult = scopeEntity->child(i);
				break;
			}
		if ( findResult == NULL )
		{
			//qDebug ( qPrintable ("Not finded parent scope for "+scope0+'|'+scope1 ));
			findResult = new Entity ( ttype, scopes[0]);
			scopeEntity->addChild ( findResult);
		}
		//qDebug ( qPrintable ("Finded parent scope for "+scopes[0]+" in "+name+ " it's "+findResult->name ));
		scopeEntity = findResult;
		scopes.removeAt (0);//this scope finded, will find next level scope
	}
	return scopeEntity;
}

/*!
	Search entity by name, type and signature in the all container
	\param type Entity type for searching
	\param name Name of Entity
	\param signature Signature of Entity
	\return Pointer to Entity
	\retval NULL 
*/
Entity* EntityContainer::findEntityInContainer ( EntityType type, const QString& name, const QString& signature)
{
	for ( int i = 0; i < topLevelItemCount (); i++)
	{
		if (  (childEntity (i)->type == type || type == UNKNOWN ) 
				&& childEntity (i)->name == name && childEntity(i)->signature == signature)
		{
			return childEntity (i);
		}
	}
	return NULL; //not finded
}

/*!
	Search entity by name, type and signature in the another Entity
	(search in the childs)
	\param where parent Entity, in childs of which child Entity should be searched
	\param type Entity type for searching
	\param name Name of Entity
	\param signature Signature of Entity
	\return Pointer to Entity
	\retval NULL 
*/
Entity* EntityContainer::findEntityInEntity (Entity* where, EntityType type,const QString& name, const QString& signature )
{
	for ( int i = 0; i < where->childCount(); i++)
	{
		if (  (where->child(i)->type == type || type == UNKNOWN ) 
				&& where->child(i)->name == name && where->child(i)->signature == signature)
		{
			return where->child(i);
		}
	}
	return NULL; //not finded
}

/*!
	Create new Entity in the self, or in the some from Entityes
	\param parEnt Pointer to parent entity. Can be NULL
	\param entry Entry, from which Entity should be created
	\param fileName Name of file, where are Entity
	\param time Time stamp of information
*/
void EntityContainer::addChild ( Entity* parEnt,tagEntryInfo* entry, QString fileName, QDateTime time )
{
	if (parEnt)
		addChildInEntity ( parEnt, entry, fileName, time );
	else
		addChildInContainer ( entry, fileName, time);

}

/*!
	Create new Entity in the self
	\param entry Entry, from which Entity should be created
	\param fileName Name of file, where are Entity
	\param time Time stamp of information
*/
void EntityContainer::addChildInContainer ( tagEntryInfo* entry, QString fileName, QDateTime time )
{
	Entity* existing = findEntityInContainer ( Entity::getEntityType ( entry->kind), entry->name, entry->extensionFields.signature);
	if ( ! existing )
	{
		Entity* entity = new Entity ( entry, fileName, time);
		addTopLevelItem (entity);
		entity->setExpanded (Navigator::instance()->getExpandMask() & entity->type);
	}
	else
		existing->updateSelf (entry, fileName, time);
}

/*!
	Create new Entity in the existing Entity
	\param parEnt Pointer to parent entity. Can be NULL
	\param entry Entry, from which Entity should be created
	\param fileName Name of file, where are Entity
	\param time Time stamp of information
*/
void EntityContainer::addChildInEntity ( Entity* parEnt, tagEntryInfo* entry, QString fileName, QDateTime time )
{
	Entity* existing = findEntityInEntity( parEnt,Entity::getEntityType ( entry->kind), entry->name, entry->extensionFields.signature);
	if ( !existing )
	{
		Entity* entity = new Entity ( entry, fileName, time);
		parEnt->addChild (entity);
		entity->setExpanded (Navigator::instance()->getExpandMask() & entity->type);
	}
	else
		existing->updateSelf ( entry, fileName, time);
}

/*!
	Open file/line in the Editor

	Using wheh user clicks on some item in the tree
	Current item will be readed from UI
*/
void EntityContainer::makeGoto ()
{
	Entity* activeEntity = (Entity*)currentItem();
	Q_ASSERT (activeEntity);
	MonkeyCore::fileManager()->goToLine(activeEntity->file, QPoint(0,activeEntity->line), false);
    
    if (mNavigator && mNavigator->getAutoHide())
    {
        mNavigator->setDockVisible (false);
    }
}
