#ifndef QMAKEPROJECTEDITORDIALOG_H
#define QMAKEPROJECTEDITORDIALOG_H

#include <xupmanager.h>

#include "ui_UIQtConfiguration.h"
#include "QtVersionManager.h"

class XUPItem;

class QMakeProjectEditorDialog : public UIXUPProjectEditor, public Ui::UIQtConfiguration
{
	Q_OBJECT
	
public:
	QMakeProjectEditorDialog( XUPItem* project, QWidget* parent );

protected:
	QWidget* mQtConfigurationPage;
	QtVersionManager mQtManager;
	QtVersion mQtVersion;

protected slots:
	void createLanguages();
	void createQtModulesConfigurations();
	void loadsQtVersions();
	void loadsQtConfigurations();
	void currentScopeChanged( XUPItem* curScope );
	void currentOperatorChanged( const QString& curOperator );
	void lwQtVersions_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	void lQtVersionInformations_linkActivated( const QString& link );
	void cbTemplate_currentIndexChanged( const QString& text );
	void gbBuild_changed();
	void gbApplication_changed();
	void gbOutput_changed();
	void tbOutputPath_clicked();
	void gbVersion_clicked( bool checked );
	void gbVersion_changed();
	void cbBuildAutoIncrement_clicked( bool checked );
	void qtModulesConfigurations_itemSelectionChanged();
	void qtModulesConfigurations_changed();
	void twTranslations_itemChanged( QTreeWidgetItem* item, int column );
	virtual void accept();
};

#endif // QMAKEPROJECTEDITORDIALOG_H
