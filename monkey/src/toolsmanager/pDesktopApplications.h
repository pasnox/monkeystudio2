/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pDesktopApplications.h
** Date      : 2007-11-04T22:46:50
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PDESKTOPAPPLICATIONS_H
#define PDESKTOPAPPLICATIONS_H

#include <QObject>
#include <QStringList>
#include <QHash>

class QTreeWidget;

struct pDesktopApplication
{
	QString Name;
	QString Icon;
	QString GenericName;
	QString Comment;
	QStringList Categories;
};

struct pDesktopFolder
{
	~pDesktopFolder()
	{ qDeleteAll( Applications ); qDeleteAll( Folders ); }
	
	QString Path;
	QString Icon;
	QHash<QString, pDesktopApplication*> Applications;
	QHash<QString, pDesktopFolder*> Folders;
};

class pDesktopApplications : public QObject
{
	Q_OBJECT
	friend class UIDesktopTools;

public:
	enum DataType { dtType = Qt::UserRole, dtApplication, dtFolder, dtIcon, dtCategories, dtPointer, dtInUse };

	pDesktopApplications( QObject* = 0 );
	~pDesktopApplications();

	const int count();
	QStringList startMenuPaths() const;
	bool categoriesAvailable() const;
	pDesktopFolder* startMenu() { return &mStartMenu; }

protected:
	pDesktopFolder mStartMenu;

	int count( pDesktopFolder* );
	void populateApplications();

};

#endif // PDESKTOPAPPLICATIONS_H
