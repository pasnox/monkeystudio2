#ifndef UIQMAKEPROJECTSETTINGS_H
#define UIQMAKEPROJECTSETTINGS_H

#include "QSingleton.h"
#include "ui_UIQMakeProjectSettings.h"
#include "UISettingsQMake.h"

#include <QApplication>
#include <QHash>

class ProjectsModel;
class QMakeProxy;
class ProjectItem;
class QDirModel;

struct ProjectVariable
{
	ProjectVariable() {}
	ProjectVariable( const QString& v, const QString& o )
	{ setVariable( v ); setOperator( o ); }
	
	bool operator== ( const ProjectVariable& v ) const
	{ return getVariable() == v.getVariable() && getOperator() == v.getOperator(); }
	
	ProjectVariable& operator=( const ProjectVariable& o )
	{ setVariable( o.getVariable() ); setOperator( o.getOperator() ); return *this; }
	
	QString getVariable() const
	{ return mVariable; }
	void setVariable( const QString& s )
	{ mVariable = s; }
	
	QString getOperator() const
	{ return mOperator; }
	void setOperator( const QString& s )
	{ mOperator = s; }
	
	QString mVariable;
	QString mOperator;
};

struct ProjectKey
{
	ProjectKey() {}
	ProjectKey( ProjectItem* s, const ProjectVariable& v )
	{ setScope( s ); setProjectVariable( v ); }
	
	bool operator== ( const ProjectKey& v ) const
	{ return getScope() == v.getScope() && getProjectVariable() == v.getProjectVariable(); }
	
	ProjectKey& operator=( const ProjectKey& o )
	{ setScope( o.getScope() ); setProjectVariable( o.getProjectVariable() ); return *this; }
	
	ProjectItem* getScope() const
	{ return mScope; }
	void setScope( ProjectItem* s )
	{ mScope = s; }
	
	QString getVariable() const
	{ return mVariable.getVariable(); }
	void setVariable( const QString& s )
	{ mVariable.setVariable( s ); }
	
	QString getOperator() const
	{ return mVariable.getOperator(); }
	void setOperator( const QString& s )
	{ mVariable.setOperator( s ); }
	
	ProjectVariable getProjectVariable() const
	{ return mVariable; }
	void setProjectVariable( const ProjectVariable& v )
	{ mVariable = v; }
	
	ProjectItem* mScope;
	ProjectVariable mVariable;
};

typedef QHash<ProjectKey, QStringList> QtSettings;

class UIQMakeProjectSettings : public QDialog, public Ui::UIQMakeProjectSettings, public QSingleton<UIQMakeProjectSettings>
{
	Q_OBJECT
	friend class QSingleton<UIQMakeProjectSettings>;
	
protected:
	bool mInit;
	ProjectItem* mProject;
	ProjectsModel* mModel;
	QtSettings mSettings;
	QtSettings mOriginalSettings;
	QDirModel* mDirs;
	QMakeProxy* mScopesProxy;
	QMakeProxy* mVariablesProxy;
	QMakeProxy* mContentProxy;
	QStringList mBlackList;

	void showEvent( QShowEvent* );
	void closeEvent( QCloseEvent* );
	QStringList currentCONFIG() const;
	void setCurrentCONFIG( const QString& );
	QStringList currentQT() const;
	void setCurrentQT( const QString& );
	void setCurrentTRANSLATIONS( const QStringList& );
	const QString checkTranslationsPath();
	void checkOthersVariables();
	void updateOthersValues();
	ProjectKey currentKey( const QString& ) const;
	void addValue( const ProjectKey& s, const QString& v );
	void addValues( const ProjectKey& s, const QStringList& v );
	void setValue( const ProjectKey& s, const QString& v );
	void setValues( const ProjectKey& s, const QStringList& v );
	void removeValue( const ProjectKey& s, const QString& v );
	void removeValues( const ProjectKey& s, const QStringList& v );
	void clearValues( const ProjectKey& s );
	QStringList values( const ProjectKey& s ) const;
	QString value( const ProjectKey& s ) const;
	QModelIndex currentIndex();
	void setCurrentIndex( const QModelIndex& );

private:
	UIQMakeProjectSettings( ProjectItem*, QWidget* = QApplication::activeWindow() );
	
	
protected slots:
	void querySettings();
	void loadModules();
	void loadConfigs();
	void loadLanguages();
	void loadPlugins();
	void setDir( const QString& );
	void setDir( const QModelIndex& );
	void addValue( const QString& );
	void editValue( const QString& );
	void removeValue( const QString& = QString::null );
	void on_tbUndo_clicked();
	void on_tbAddScope_clicked();
	void on_tbRemoveScope_clicked();
	void cb_highlighted( int );
	void tb_clicked();
	void sb_valueChanged( int );
	void on_cbScopes_highlighted( const QModelIndex& );
	void on_cbScopes_currentChanged( const QModelIndex& );
	void on_cbOperators_currentIndexChanged( const QString& );
	void on_cbTemplate_currentIndexChanged( const QString& );
	void lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* );
	void on_lwFiles_itemDoubleClicked( QListWidgetItem* );
	void on_cbVariables_currentIndexChanged( const QString& );
	void on_pbAddValue_clicked();
	void on_pbEditValue_clicked();
	void on_pbRemoveValue_clicked();
	void on_pbClearValues_clicked();
	void on_lwTranslations_itemChanged( QListWidgetItem* );
	void on_tbOthersVariablesAdd_clicked();
	void on_tbOthersVariablesEdit_clicked();
	void on_tbOthersVariablesRemove_clicked();
	void on_lwOthersVariables_itemSelectionChanged();
	void on_tbOthersValuesAdd_clicked();
	void on_tbOthersValuesEdit_clicked();
	void on_tbOthersValuesRemove_clicked();
	void on_tbOthersValuesClear_clicked();
	void on_tbOthersValuesUp_clicked();
	void on_tbOthersValuesDown_clicked();
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
	
};

#endif // UIQMAKEPROJECTSETTINGS_H
