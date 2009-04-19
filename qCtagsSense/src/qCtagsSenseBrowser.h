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
class qCtagsSenseEntry;

class QCTAGSSENSE_EXPORT qCtagsSenseBrowser : public QFrame, public Ui::qCtagsSenseBrowser
{
	Q_OBJECT

public:
	qCtagsSenseBrowser( QWidget* parent = 0 );
	virtual ~qCtagsSenseBrowser();
	
	qCtagsSense* sense() const;

public slots:
	void setCurrentFileName( const QString& fileName );
	void tagEntry( const QString& fileName );
	void tagEntries( const QMap<QString, QString>& entries );

protected:
	qCtagsSense* mSense;
	QString mLanguage, mFileName;
	qCtagsSenseLanguagesModel* mLanguagesModel;
	qCtagsSenseFilesModel* mFilesModel;
	qCtagsSenseMembersModel* mMembersModel;

protected slots:
	void on_cbLanguages_currentIndexChanged( int id );
	void on_cbFileNames_currentIndexChanged( int id );
	void on_cbMembers_currentIndexChanged( int id );
	void on_tvMembers_activated( const QModelIndex& index );
	void mSense_indexingProgress( int value, int total );
	void mSense_indexingChanged();
	void mLanguagesModel_ready();
	void mFilesModel_ready();
	void mMembersModel_ready();
	void on_tvMembers_customContextMenuRequested( const QPoint& pos );

signals:
	void memberActivated( qCtagsSenseEntry* entry );
};

#endif // QCTAGSSENSEBROWSER_H
