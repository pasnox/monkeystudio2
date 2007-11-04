/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIMonkeyProjectSettings.h
** Date      : 2007-11-04T22:53:33
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
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
