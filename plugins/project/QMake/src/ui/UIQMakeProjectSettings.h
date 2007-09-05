#ifndef UIQMAKEPROJECTSETTINGS_H
#define UIQMAKEPROJECTSETTINGS_H
//
#include "ui_UIQMakeProjectSettings.h"
#include "UISettingsQMake.h"
//
#include <QApplication>
#include <QHash>
//
class QMakeProjectProxy;
class QMakeProjectModel;
class QMakeProjectItem;
class QDirModel;
//
typedef QHash<QString, QStringList> QtSettings; // scope|operator|variable, values
//
class UIQMakeProjectSettings : public QDialog, public Ui::UIQMakeProjectSettings
{
	Q_OBJECT
	//
public:
	static void execute( QMakeProjectItem*, QWidget* = QApplication::activeWindow() );
	~UIQMakeProjectSettings();
	QModelIndex currentIndex();
	void setCurrentIndex( const QModelIndex& );
	QString projectName() const;
	QString projectPath() const;
	QString getFilePath( const QString& );
	QString getRelativeFilePath( const QString& );
	QString getStringValues( const QString&, const QString& = "=", const QString& = QString::null ) const;
	QStringList getListValues( const QString&, const QString& = "=", const QString& = QString::null ) const;
	void setListValues( const QStringList&, const QString&, const QString& = "=", const QString& = QString::null );
	void setStringValues( const QString&, const QString&, const QString& = "=", const QString& = QString::null );
	//
protected:
	UIQMakeProjectSettings( QMakeProjectItem*, QWidget* = 0 );
	void closeEvent( QCloseEvent* );
	//
	bool mReady;
	QMakeProjectProxy* mScopesProxy;
	QMakeProjectProxy* mContentProxy;
	//
	QMakeProjectModel* mModel;
	QMakeProjectItem* mProject;
	QModelIndex mProjectIndex;
	QtItemList mModules;
	QtItemList mConfigs;
	QtSettings mSettings;
	QDirModel* mDirs;
	//
protected slots:
	void loadEncodings();
	void loadModules();
	void loadConfigs();
	void loadSettings();
	void loadLanguages();
	void setDir( const QString& );
	void setDir( const QModelIndex& );
	void addValue( const QString& );
	void editValue( const QString& );
	void removeValue( const QString& = QString::null );
	void tb_clicked();
	void sb_valueChanged( int );
	void on_cbTemplate_currentIndexChanged( const QString& );
	void lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* );
	void cb_highlighted( int );
	void on_cbScopes_currentIndexChanged( const QString& );
	void on_cbOperators_currentIndexChanged( const QString& );
	void on_lwFiles_itemDoubleClicked( QListWidgetItem* );
	void on_cbVariables_currentIndexChanged( const QString& );
	void on_pbAddValue_clicked();
	void on_pbEditValue_clicked();
	void on_pbRemoveValue_clicked();
	void on_pbClearValues_clicked();
	void on_lwTranslations_itemChanged( QListWidgetItem* );
	void on_tvScopes_clicked( const QModelIndex& );
	void on_tvScopes_doubleClicked( const QModelIndex& );
	void on_lvContents_doubleClicked( const QModelIndex& );
	void on_tbAdd_clicked();
	void on_tbEdit_clicked();
	void on_tbRemove_clicked();
	void on_tbClear_clicked();
	void on_tbUp_clicked();
	void on_tbDown_clicked();
	void on_dbbButtons_helpRequested();
	void accept();
	void reject();
	//
};
//
#endif // UIQMAKEPROJECTSETTINGS_H
