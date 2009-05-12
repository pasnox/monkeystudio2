/****************************************************************************
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
****************************************************************************/
#ifndef QCTAGSSENSEBROWSER_H
#define QCTAGSSENSEBROWSER_H

#include "qCtagsSenseExport.h"
#include "ui_qCtagsSenseBrowser.h"

class qCtagsSense;
class qCtagsSenseLanguagesModel;
class qCtagsSenseFilesModel;
class qCtagsSenseMembersModel;
class qCtagsSenseSearchModel;
class qCtagsSenseEntry;
class QAction;
class QMovie;
class QTimer;

class QCTAGSSENSE_EXPORT qCtagsSenseBrowser : public QFrame, public Ui::qCtagsSenseBrowser
{
	Q_OBJECT

public:
	qCtagsSenseBrowser( QWidget* parent = 0 );
	virtual ~qCtagsSenseBrowser();
	
	qCtagsSense* sense() const;
	
	qCtagsSenseLanguagesModel* languagesModel() const;
	qCtagsSenseFilesModel* filesModel() const;
	qCtagsSenseMembersModel* membersModel() const;
	QAction* membersAction() const;
	QAction* viewBrowserAction() const;
	QAction* viewSearchResultsAction() const;
	void popupMenu( QTreeView* view, const QPoint& pos );

public slots:
	void setSystemPaths( const QStringList& paths, const QStringList& oldPaths );
	void setFilteredSuffixes( const QStringList& suffixes );
	void setCurrentFileName( const QString& fileName );
	void tagEntry( const QString& fileName );
	void tagEntries( const QMap<QString, QString>& entries );

protected:
	qCtagsSense* mSense;
	QString mLanguage, mFileName;
	qCtagsSenseLanguagesModel* mLanguagesModel;
	qCtagsSenseFilesModel* mFilesModel;
	qCtagsSenseMembersModel* mMembersModel;
	qCtagsSenseSearchModel* mSearchModel;
	QAction* aMembers;
	QMovie* mLoading;
	QTimer* mSearchTimer;

protected slots:
	void on_aBrowser_toggled( bool checked );
	void on_aSearchResults_toggled( bool checked );
	void on_tvMembers_activated( const QModelIndex& index );
	void on_tvSearchResult_activated( const QModelIndex& index );
	void mSense_indexingProgress( int value, int total );
	void mSense_indexingChanged();
	void mMembersModel_ready();
	void on_leSearch_textChanged( const QString& search );
	void mSearchModel_searching( bool searching );
	void mSearchTimer_timeout();
	void mSearchModel_ready();
	void on_tvMembers_customContextMenuRequested( const QPoint& pos );

signals:
	void entryActivated( qCtagsSenseEntry* entry );
	void fileNameActivated( const QString& fileName );
};

#endif // QCTAGSSENSEBROWSER_H
