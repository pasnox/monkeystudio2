/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : Entity.h
** Date      : 2007-11-04T22:50:41
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef ENTITY_H
#define ENTITY_H
#include <QString>
#include <QList>
#include <QTreeWidget>
#include <QObject>
#include <QDateTime>
#include <QIcon>

#include "Ctags.h"

enum EntityType { 
    NAMESPACE = 0x0001,
    CLASS = 0x0002,
    CONSTRUCTOR = 0x0004,
    DESTRUCTOR = 0x0008,
    ENUM = 0x0010,
    ENUMERATOR = 0x0020,
    EXTERN = 0x0040,
    FUNCTION = 0x0080,
    MACRO = 0x0100,
    MEMBER = 0x0200,
    PROTOTYPE = 0x0400,
    STRUCT = 0x0800,
    TYPEDEF = 0x1000,
    UNION = 0x2000,
    VARIABLE = 0x4000,

    ROOT_ENTITY,  //15  //top level entiry, possible file, or project

    UNKNOWN = 0x8000 //16
};

class Entity : public QTreeWidgetItem
{
private:
    static QIcon iNAMESPACE;
    static QIcon iCLASS;
    static QIcon iENUM;
    static QIcon iFUNCTION;
    static QIcon iMEMBER;
    static QIcon iSTRUCT;
    static QIcon iTYPEDEF;
    
public:
    Entity ( tagEntryInfo* entry, QString fileName, QDateTime time);
    Entity (EntityType ttype, QString tname);
    QDateTime updateTime ;
    QString name;
    EntityType type;
    QString file;
    int line;
    QString signature;

    Entity* child (int i);
    
    //function will create or update info about file in the entity

    //convert string kindName to the EntityType
    static EntityType getScopeType ( QString kindName );
    //convert TagEntry.kind  to the EntiryType
    static EntityType getEntityType ( char kind );
    static void initIcons ();
    void deleteFileInfo ( QString file, QDateTime olderThan );
    void updateSelf (tagEntryInfo* entry, QString fileName, QDateTime time);
    void updateGUI ();
};

#endif //ENTITY_H
