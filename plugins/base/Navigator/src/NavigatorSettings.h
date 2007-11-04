/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : NavigatorSettings.h
** Date      : 2007-11-04T22:50:45
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef NAVIGATORSETTINGS_H
#define NAVIGATORSETTINGS_H

#include <QCheckBox>

class NavigatorSettings : public QWidget
{
Q_OBJECT
public:
	NavigatorSettings ();
	~NavigatorSettings ();	
private:
	QList<QCheckBox*>  displayFlags, expandFlags;
	QStringList names;
private slots:
	void setSettings();
};

#endif //NAVIGATORSETTINGS_H
