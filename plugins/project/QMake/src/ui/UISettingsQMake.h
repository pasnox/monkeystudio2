/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : UISettingsQMake.h
** Date      : 2008-01-14T00:54:10
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
#ifndef UISETTINGSQMAKE_H
#define UISETTINGSQMAKE_H

#include "ui_UISettingsQMake.h"

#include <QList>

struct QtItem
{
	enum Roles { ValueRole = Qt::UserRole, VariableRole, HelpRole };
	QtItem( const QString& t, const QString& v, const QString& s, const QString& h = QString::null )
		: Text( t ), Value( v ), Variable( s ), Help( h )
	{}
	QString Text;
	QString Value;
	QString Variable;
	QString Help;
};
typedef QList<QtItem> QtItemList;

struct QtVersion
{
	enum Roles { PathRole = Qt::UserRole, QMakeRole, lupdateRole, lreleaseRole, DocsPathRole, DefaultRole };
	QtVersion() {}
	QtVersion( const QString& t, const QString& p, const QString& q, const QString& u, const QString& r, const QString& d, bool df )
		: Text( t ), Path( p ), QMake( q ), lupdate( u ), lrelease( r ), DocsPath( d ), Default( df )
	{}
	QString Text;
	QString Path;
	QString QMake;
	QString lupdate;
	QString lrelease;
	QString DocsPath;
	bool Default;
};
typedef QList<QtVersion> QtVersionList;

class UISettingsQMake : public QWidget, public Ui::UISettingsQMake
{
	Q_OBJECT
	
public:
	enum Type { BinaryType, PathType };
	enum Role { TypeRole = Qt::UserRole, DataRole };
	UISettingsQMake( QWidget* = 0 );
	
	static QStringList defaultFilters();
	static QStringList readFilters();
	static QStringList defaultFiltersToolTips();
	static QStringList readFiltersToolTips();
	static QStringList defaultScopes();
	static QStringList readScopes();
	static QStringList defaultPathFiles();
	static QStringList readPathFiles();
	static QtItemList defaultQtModules();
	static QtItemList readQtModules();
	static QtItemList defaultSettings();
	static QtItemList readSettings();
	static QtVersionList defaultVersions();
	static QtVersionList readVersions();
	static QtVersion defaultVersion( const QString& = QString::null );
	
private slots:
	void loadSettings();
	void lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* );
	void tbQtVersion_clicked();
	void tbAdd_clicked();
	void tbRemove_clicked();
	void tbClear_clicked();
	void tbUp_clicked();
	void tbDown_clicked();
	void on_tbDefaultQtVersion_clicked();
	void on_tbToolTipFilter_clicked();
	void on_dbbButtons_helpRequested();
	void on_dbbButtons_clicked( QAbstractButton* );
	
};

#endif // UISETTINGSQMAKE_H
