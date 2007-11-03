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
	void addChild ( Entity* parEnt,sTagEntryInfo* entry, QString fileName, QDateTime time );	

	void addChildInEntity ( Entity* parEnt, sTagEntryInfo* entry, QString fileName, QDateTime time );
	//setting. If true - entity will allow to add to self only compex entityes and his members
	bool complex_only;

	Entity* findEntityInContainer ( EntityType type, QString name );
	Entity* findEntityInEntity (Entity* where, EntityType type, QString name );

	void addChildInContainer ( sTagEntryInfo* entry, QString fileName, QDateTime time );

public slots:
	void makeGoto ();
};
#endif //ENTITYCONTAINER_H 
