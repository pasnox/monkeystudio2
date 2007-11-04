/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pDockFileBrowser.h
** Date      : 2007-11-04T22:50:37
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
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
class QLineEdit;
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
	QLineEdit* mLineEdit;
	QListView* mListView;
	QDirModel* mDirModel;
	void showEvent( QShowEvent* );
	void hideEvent( QHideEvent* );

private:
	pDockFileBrowser( QWidget* = 0 );

private slots:
	void tbUp_clicked();
	void tbRefresh_clicked();
	void lv_doubleClicked( const QModelIndex& );
	void cb_currentIndexChanged( const QString& );

signals:
	void saveSettings();
	void restoreSettings();

};

#endif // PDOCKFILEBROWSER_H
