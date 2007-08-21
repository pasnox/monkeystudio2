#ifndef UIEDITTEMPLATE_H
#define UIEDITTEMPLATE_H

#include "MonkeyExport.h"
#include "ui_UIEditTemplate.h"

class QTreeWidget;
class QTreeWidgetItem;

class Q_MONKEY_EXPORT UIEditTemplate : public QDialog, public Ui::UIEditTemplate
{
	Q_OBJECT

public:
	static void edit( QTreeWidget*, QTreeWidgetItem* );

private:
	UIEditTemplate( QWidget*, QTreeWidget*, QTreeWidgetItem* );
	~UIEditTemplate();

	bool canAddFile( const QString& );

	QTreeWidget* mTree;
	QTreeWidgetItem* mItem;

private slots:
	void on_tbIcon_clicked();
	void on_pbAdd_clicked();
	void on_pbEdit_clicked();
	void on_pbRemove_clicked();
	void accept();

};

#endif // UIEDITTEMPLATE_H
