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
typedef QList<QtItem*> QtItemList;

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
	static QString defaultQMake();
	static QString defaultlupdate();
	static QString defaultlrelease();
	
private slots:
	void loadSettings();
	void lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* );
	void tbAdd_clicked();
	void tbRemove_clicked();
	void tbClear_clicked();
	void tbUp_clicked();
	void tbDown_clicked();
	void on_tbToolTipFilter_clicked();
	void on_bbDialog_helpRequested();
	void on_bbDialog_clicked( QAbstractButton* );
	
};

#endif // UISETTINGSQMAKE_H
