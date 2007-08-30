/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : FileBrowser Plugin
** FileName  : pDockFileBrowser.h
** Date      : 2007-08-27T23:00:25
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PDOCKFILEBROWSER_H
#define PDOCKFILEBROWSER_H

#include "QSingleton.h"

#include <QDockWidget>
#include <QModelIndex>

class QComboBox;
class QListView;
class QDirModel;

class pDockFileBrowser : public QDockWidget, public QSingleton<pDockFileBrowser>
{
	Q_OBJECT
	friend class QSingleton<pDockFileBrowser>;

public:
	~pDockFileBrowser();

	QString currentDrive() const;
	void setCurrentDrive( const QString& );

	QString currentPath() const;
	void setCurrentPath( const QString& );

protected:
	bool mShown;
	QComboBox* mComboBox;
	QListView* mListView;
	QDirModel* mDirModel;
	void showEvent( QShowEvent* );
	void hideEvent( QHideEvent* );

private:
	pDockFileBrowser( QWidget* = 0 );

private slots:
	void tb_clicked();
	void lv_doubleClicked( const QModelIndex& );
	void cb_currentIndexChanged( const QString& );

signals:
	void saveSettings();
	void restoreSettings();

};

#endif // PDOCKFILEBROWSER_H
