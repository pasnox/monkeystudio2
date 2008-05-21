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

class QListView;
class QFileSystemModel;

class Q_MONKEY_EXPORT AddFilesDialog : public QFileDialog
{
	Q_OBJECT

public:
	AddFilesDialog( ScopedProjectItemModel* scopedmodel, XUPItem* selecteditem, QWidget* = 0 );

	inline XUPItem* currentItem() const { return qobject_cast<ProjectItemModel*>( mScoped->sourceModel() )->itemFromIndex( mScoped->mapToSource( tcbProjects->currentIndex() ) ); }
	inline QString currentOperator() const { return cbOperators->currentText(); }
	inline bool isRecursive() const { return cbRecursive->isChecked(); }
	QStringList selectedFilesFolders() const;

protected:
	ScopedProjectItemModel* mScoped;
	pTreeComboBox* tcbProjects;
	QComboBox* cbOperators;
	QCheckBox* cbRecursive;
	QPushButton* pbAdd;
	QListView* lvFiles;
	QTreeView* tvFiles;
	QLineEdit* leFiles;
	QFileSystemModel* mModel;

protected slots:
	void doubleClicked( const QModelIndex& index );
	void selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
	void addClicked();
};

#endif // ADDFILESDIALOG_H
