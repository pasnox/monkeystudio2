#ifndef UISETTINGSQMAKEXUPPROJECTEDITOR_H
#define UISETTINGSQMAKEXUPPROJECTEDITOR_H

#include "ui_UISettingsQMakeXUPProjectEditor.h"
#include "../QtVersionManager.h"

class UISettingsQMakeXUPProjectEditor : public QWidget, public Ui::UISettingsQMakeXUPProjectEditor
{
	Q_OBJECT
	
public:
	UISettingsQMakeXUPProjectEditor( QWidget* parent = 0 );

protected:
	QtVersionManager mQtManager;

protected slots:
	void tbAdd_clicked();
	void tbRemove_clicked();
	void tbClear_clicked();
	void tbUp_clicked();
	void tbDown_clicked();
	void on_tbDefaultQtVersion_clicked();
	void on_leQtVersionPath_textChanged( const QString& text );
	void on_tbQtVersionPath_clicked();
	void on_tbQtVersionQMakeSpec_clicked();
	void lw_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	void loadSettings();
	void on_dbbButtons_helpRequested();
	void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // UISETTINGSQMAKEXUPPROJECTEDITOR_H
