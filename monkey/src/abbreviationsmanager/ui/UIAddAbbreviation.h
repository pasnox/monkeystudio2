#ifndef UIADDABBREVIATION_H
#define UIADDABBREVIATION_H

#include "MonkeyExport.h"
#include "ui_UIAddAbbreviation.h"

class QTreeWidget;

class Q_MONKEY_EXPORT UIAddAbbreviation : public QDialog, public Ui::UIAddAbbreviation
{
	Q_OBJECT

public:
	static void edit( QTreeWidget* );

private:
	UIAddAbbreviation( QWidget*, QTreeWidget* );
	~UIAddAbbreviation();

	QTreeWidget* mTree;

private slots:
	void accept();

};

#endif // UIADDABBREVIATION_H
