#ifndef ADDFILESDIALOG_H
#define ADDFILESDIALOG_H

#include <QFileDialog>
#include <QGroupBox>
#include <QComboBox>

#include <fresh.h>

#include "../ScopedProjectItemModel.h"
#include "../ProjectItemModel.h"
#include "../XUPItem.h"

class AddFilesDialog : public QFileDialog
{
	Q_OBJECT

public:
	AddFilesDialog( ScopedProjectItemModel* scopedmodel, XUPItem* selecteditem, QWidget* = 0 );

	inline XUPItem* currentItem() const { return qobject_cast<ProjectItemModel*>( mScoped->sourceModel() )->itemFromIndex( mScoped->mapToSource( tcbProjects->currentIndex() ) ); }
	inline QString currentOperator() const { return cbOperators->currentText(); }

protected:
	ScopedProjectItemModel* mScoped;

	pTreeComboBox* tcbProjects;
	QComboBox* cbOperators;
};

#endif // ADDFILESDIALOG_H
