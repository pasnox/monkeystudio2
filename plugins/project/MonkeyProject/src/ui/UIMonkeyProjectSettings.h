/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : MonkeyProject project plugin
** FileName  : MonkeyProjectSettings.h
** Date      : 2007-09-28
** License   : GPL
** Comment   : MonkeyProject project settings
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UINOPROJECTPROJECTSETTINGS_H
#define UINOPROJECTPROJECTSETTINGS_H

#include <QHash>

#include "ui_UIMonkeyProjectSettings.h"

#include "MonkeyProjectItem.h"

class UIMonkeyProjectSettings : public QDialog, Ui_UIMonkeyProjectSettings
{
	Q_OBJECT
public:
	UIMonkeyProjectSettings (MonkeyProjectItem* project);

	QList<MonkeyProjectItem::Target> targets;
private slots:
	void accept ();
	void selectedRowChanged(int);
	void actionNameEdited(QString name);
	void addAction ();
	void removeAction ();
	void actionCommandEdited ();
private:
	MonkeyProjectItem* project;
};

#endif // UINOPROJECTPROJECTSETTINGS_H
