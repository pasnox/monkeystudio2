/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UISettingsQMake.h
** Date      : 2007-11-04T22:53:39
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef UISETTINGSQMAKE_H
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
