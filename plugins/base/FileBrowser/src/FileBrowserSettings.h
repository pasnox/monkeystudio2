/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : FileBrowserSettings.h
** Date      : 2007-11-04T22:50:45
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef FILEBROWSERSETTINGS_H
#define FILEBROWSERSETTINGS_H

#include <QWidget>

class pStringListEditor;

class FileBrowserSettings : public QWidget
{
	Q_OBJECT
	
public:
	FileBrowserSettings();

private:
	pStringListEditor* mEditor;

private slots:
	void setSettings();

};

#endif // FILEBROWSERSETTINGS_H
