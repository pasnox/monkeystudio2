#ifndef ADDFILESDIALOG_H
#define ADDFILESDIALOG_H

#include <fresh.h>

#include <QFileDialog>
#include <QGroupBox>
#include <QComboBox>

#include "ScopedProjectItemModel.h"
#include "ProjectItemModel.h"
#include "XUPItem.h"

class Q_MONKEY_EXPORT AddFilesDialog : public QFileDialog
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
