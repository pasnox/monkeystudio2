/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pToolsManager.h
** Date      : 2007-11-04T22:46:53
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef PTOOLSMANAGER_H
#define PTOOLSMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QApplication>
#include <QFileIconProvider>

class pSettings;
class pMenuBar;
class QAction;
	
struct Q_MONKEY_EXPORT pTool
{
	pTool( const QString& c, const QString& i, const QString& f, const QString& p, bool b = false )
	:	Caption( c ), FileIcon( i ), FilePath( f ), WorkingPath( p ), DesktopEntry( b ) {}

	QString Caption;
	QString FileIcon;
	QString FilePath;
	QString WorkingPath;
	bool DesktopEntry;
};

class Q_MONKEY_EXPORT pToolsManager : public QObject, public QSingleton<pToolsManager>
{
	Q_OBJECT
	friend class QSingleton<pToolsManager>;

public:
	enum ToolType { ttAll = -1, ttUserEntry, ttDesktopEntry };

	static const QFileIconProvider* iconProvider();
	static const QList<pTool> tools( ToolType = pToolsManager::ttAll );
	static void initializeTools();

protected:
	static QFileIconProvider* mIconProvider;

private:
	pToolsManager( QObject* = QApplication::instance() );
	~pToolsManager();

public slots:
	void editTools_triggered();
	void toolsMenu_triggered( QAction* );

};

#endif // PTOOLSMANAGER_H
