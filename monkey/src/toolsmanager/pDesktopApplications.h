/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox
** Project   : pDesktopApplications
** FileName  : pDesktopApplications.h
** Date      : ven. août 17 21:19:02 2007
** License   : GPL
** Comment   : Your comment here
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
