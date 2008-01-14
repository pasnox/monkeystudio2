/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : pDockFileBrowser.h
** Date      : 2008-01-14T00:39:57
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/

#ifndef PDOCKFILEBROWSER_H
#define PDOCKFILEBROWSER_H

#include "QSingleton.h"
#include "pDockWidget.h"

#include <QModelIndex>
#include <QSortFilterProxyModel>

class pTreeComboBox;
class QLineEdit;
class QListView;
class QDirModel;
class QTreeView;

class pDockFileBrowser : public pDockWidget, public QSingleton<pDockFileBrowser>
{
	Q_OBJECT
	friend class QSingleton<pDockFileBrowser>;

	class FilteredModel: public QSortFilterProxyModel
	{
		friend class pDockFileBrowser;
			
	protected:
		QStringList mWildcards;
	
		bool filterAcceptsRow( int row, const QModelIndex& parent ) const;
	
	public:
		FilteredModel( QObject* parent = 0 ) : QSortFilterProxyModel( parent ) {}
	};
	
public:
	QString currentPath() const;
	QStringList wildcards() const;

protected:
	bool mShown;
	pTreeComboBox* mCombo;
	QLineEdit* mLineEdit;
	QTreeView* mTree;
	QDirModel* mDirsModel;
	FilteredModel* mFilteredModel;

	void showEvent( QShowEvent* );
	void hideEvent( QHideEvent* );

private:
	pDockFileBrowser( QWidget* = 0 );

public slots:
	void setCurrentPath( const QString& );
	void setWildcards( const QStringList& );

private slots:
	void tbUp_clicked();
	void tbRefresh_clicked();
	void tbRoot_clicked();
	void cb_currentChanged( const QModelIndex& );
	void tv_doubleClicked( const QModelIndex& );

signals:
	void saveSettings();
	void restoreSettings();
};

#endif // PDOCKFILEBROWSER_H
