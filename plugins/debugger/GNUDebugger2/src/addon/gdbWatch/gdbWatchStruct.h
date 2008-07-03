
#ifndef GDBWATCHSTRUCT_H
#define GDBWATCHSTRUCT_H

#include <QString>
#include <QStringList>
#include <QTreeWidgetItem>

class WatchStruct
{

private:

// extrait les membres de la  structure
// exemple : "rtf = {art = {0}, a = 5 , b = 6 , rtf  = {t = 12 , r = 34}}"
// return  :
// 1 : art = {0}
// 2 : a = 5
// 3 : b = 6
// 4 : rtf = {t = 12 , r = 34}

QStringList extractMember(QString val);


// return le type du membre de la structure
// 2 : c'est un autre structure
// 1 : membre " x= y"
// 0 : membre orphelin "0x00"
 int memberType(QString val);


// formatage des membres
 QStringList formatMember(QString val);


// recursive function
public:

	 void decompilStrut(QTreeWidgetItem *parentItem, QString val);
	 void decompilStrutUpdate(QTreeWidgetItem *parentItem, QString val);
};

#endif
