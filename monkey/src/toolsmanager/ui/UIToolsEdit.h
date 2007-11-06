#ifndef UITOOLSEDIT_H
#define UITOOLSEDIT_H

#include "MonkeyExport.h"
#include "ui_UIToolsEdit.h"
#include "QSingleton.h"

#include <QApplication>

class Q_MONKEY_EXPORT UIToolsEdit : public QDialog, public Ui::UIToolsEdit, public QSingleton<UIToolsEdit>
{
	Q_OBJECT
	friend class QSingleton<UIToolsEdit>;

public:
	enum ItemData { idCaption = Qt::DisplayRole, idFileIcon = Qt::UserRole +1, idFilePath, idWorkingPath };

private:
	UIToolsEdit( QWidget* = QApplication::activeWindow() );

protected:
	void closeEvent( QCloseEvent* );
	bool eventFilter( QObject*, QEvent* );

public slots:
	void on_lwTools_itemSelectionChanged();
	void on_pbNew_clicked();
	void on_pbDelete_clicked();
	void on_pbUp_clicked();
	void on_pbDown_clicked();
	void helpRequested();
	void on_leCaption_editingFinished();
	void on_tbFileIcon_clicked();
	void on_leFilePath_editingFinished();
	void on_tbFilePath_clicked();
	void on_tbUpdateWorkingPath_clicked();
	void on_leWorkingPath_editingFinished();
	void on_tbWorkingPath_clicked();
	void accept();

};

#endif // UITOOLSEDIT_H
