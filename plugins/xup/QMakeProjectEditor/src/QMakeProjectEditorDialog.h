#ifndef QMAKEPROJECTEDITORDIALOG_H
#define QMAKEPROJECTEDITORDIALOG_H

#include <xupmanager.h>

#include "ui_UIQtConfiguration.h"
#include "QtVersionManager.h"

class XUPItem;

class QMakeProjectEditorDialog : public UIXUPProjectEditor
{
	Q_OBJECT
	
public:
	QMakeProjectEditorDialog( XUPItem* project, QWidget* parent );

protected:
	QWidget* mQtConfigurationPage;
	Ui::UIQtConfiguration mUiQtConfiguration;
	QtVersionManager mQtManager;
	QtVersion mQtVersion;

protected slots:
	void loadsQtVersions();
	void cbTemplate_currentIndexChanged( const QString& text );
	void cbOperator_currentIndexChanged( const QString& text );
	void lwQtVersions_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	void lQtVersionInformations_linkActivated( const QString& link );
	virtual void accept();
};

#endif // QMAKEPROJECTEDITORDIALOG_H
