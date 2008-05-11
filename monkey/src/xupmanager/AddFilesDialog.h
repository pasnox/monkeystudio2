#ifndef ADDFILESDIALOG_H
#define ADDFILESDIALOG_H

#include <fresh.h>

#include <QFileDialog>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>

#include "ScopedProjectItemModel.h"
#include "ProjectItemModel.h"
#include "XUPItem.h"

class Q_MONKEY_EXPORT AddFilesDialog : public QFileDialog
{
	Q_OBJECT

public:
	enum Type { Files = 0, Folder };
	AddFilesDialog( AddFilesDialog::Type type, ScopedProjectItemModel* scopedmodel, XUPItem* selecteditem, QWidget* = 0 );

	inline XUPItem* currentItem() const { return qobject_cast<ProjectItemModel*>( mScoped->sourceModel() )->itemFromIndex( mScoped->mapToSource( tcbProjects->currentIndex() ) ); }
	inline QString currentOperator() const { return cbOperators->currentText(); }
	inline bool isRecursive() const { return cbRecursive->isChecked(); }

protected:
	ScopedProjectItemModel* mScoped;
	AddFilesDialog::Type mType;
	pTreeComboBox* tcbProjects;
	QComboBox* cbOperators;
	QCheckBox* cbRecursive;
};

#endif // ADDFILESDIALOG_H
