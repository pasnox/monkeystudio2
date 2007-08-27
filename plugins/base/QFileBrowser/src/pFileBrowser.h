/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : QFileBrowser Plugin
** FileName  : pFileBrowser.h
** Date      : 2007-08-27T23:00:25
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PFILEBROWSER_H
#define PFILEBROWSER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QDockWidget>
#include <QModelIndex>

class QComboBox;
class QListView;
class QDirModel;

class Q_MONKEY_EXPORT pFileBrowser : public QDockWidget, public QSingleton<pFileBrowser>
{
	Q_OBJECT
	friend class QSingleton<pFileBrowser>;

public:
	~pFileBrowser();

protected:
	QComboBox* mComboBox;
	QListView* mListView;
	QDirModel* mDirModel;

private:
	pFileBrowser( QWidget* = 0 );

private slots:
	void lv_doubleClicked( const QModelIndex& );
	void cb_currentIndexChanged( const QString& );

};

#endif // PFILEBROWSER_H
