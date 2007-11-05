/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : EntityContainer.h
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
#ifndef ENTITYCONTAINER_H
#define ENTITYCONTAINER_H
#include <QTreeWidget>
#include "Entity.h"

class EntityContainer  : public QTreeWidget 
{
Q_OBJECT
public:
	EntityContainer ( QWidget* parent);
	~EntityContainer ();
	void updateFileInfo ( QString fileName );	
	void deleteFileInfo ( QString file, QDateTime olderThan );

private:
	void addTagsFromRecord (QString fileName, FileRecord*  fileRecord);

	Entity* childEntity (int);

	//will return pointer to the finded entity, or NULL.
	Entity* findEntity ( EntityType ttype, QString tname);
	//will return poiner to the entity, which are for this scope, or NULL, if child have no scope
	Entity* getScopeEntity ( QString scope0, QString scope1);

	//parEnt may be == NULL or Entity*.  result of findScopeEntity function;
	void addChild ( Entity* parEnt,tagEntryInfo* entry, QString fileName, QDateTime time );	

	void addChildInEntity ( Entity* parEnt, tagEntryInfo* entry, QString fileName, QDateTime time );
	//setting. If true - entity will allow to add to self only compex entityes and his members
	bool complex_only;

	Entity* findEntityInContainer ( EntityType type, QString name );
	Entity* findEntityInEntity (Entity* where, EntityType type, QString name );

	void addChildInContainer ( tagEntryInfo* entry, QString fileName, QDateTime time );

public slots:
	void makeGoto ();
};
#endif //ENTITYCONTAINER_H 
