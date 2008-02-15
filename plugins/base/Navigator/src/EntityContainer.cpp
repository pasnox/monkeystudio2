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
#include <QString>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

#include "pFileManager.h"
#include "EntityContainer.h"
#include "Ctags.h"
#include "Entity.h"
#include "Navigator.h"
#include "MonkeyCore.h"

EntityContainer::EntityContainer ( QWidget* parent): QTreeWidget (parent)
{
    headerItem ()->setHidden (true);
    connect (this, SIGNAL (clicked ( const QModelIndex)),this, SLOT (makeGoto()));
    Entity::initIcons ();
}

EntityContainer::~EntityContainer ()
{
    disconnect (this, SIGNAL (clicked ( const QModelIndex)),this, SLOT (makeGoto()));
}


Entity* EntityContainer::childEntity (int i)
{
    return (Entity*)topLevelItem(i);
}

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

void EntityContainer::updateFileInfo ( QString fileName )
{
    FileRecord* rd = Ctags::instance()->GetTagsForFile (fileName); 
    addTagsFromRecord(fileName, rd);//add of update existing entityes
    deleteFileInfo (fileName, rd->time);//deltete not updated  
}

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

void EntityContainer::addChild ( Entity* parEnt,tagEntryInfo* entry, QString fileName, QDateTime time )
{
    if (parEnt)
        addChildInEntity ( parEnt, entry, fileName, time );
    else
        addChildInContainer ( entry, fileName, time);

}


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

void EntityContainer::makeGoto ()
{
    Entity* activeEntity = (Entity*)currentItem();
    Q_ASSERT (activeEntity);
    MonkeyCore::fileManager()->goToLine(activeEntity->file, QPoint(0,activeEntity->line), false);
}
