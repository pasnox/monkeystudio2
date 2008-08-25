#ifndef ADDFILESDIALOG_H
#define ADDFILESDIALOG_H

#include <fresh.h>

#include <QFileDialog>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>

#include "ScopedProjectItemModel.h"
#include "ProjectItemModel.h"

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
	inline bool importFiles() const { return gbCopy->isChecked(); }
	inline QString importPath() const { return cbCopy->currentText(); }
	QStringList selectedFilesFolders() const;

protected:
	ScopedProjectItemModel* mScoped;
	pTreeComboBox* tcbProjects;
	QComboBox* cbOperators;
	QCheckBox* cbRecursive;
	QGroupBox* gbCopy;
	QComboBox* cbCopy;
	QPushButton* pbAdd;
	QListView* lvFiles;
	QTreeView* tvFiles;
	QLineEdit* leFiles;
	QFileSystemModel* mModel;

protected slots:
	void doubleClicked( const QModelIndex& index );
	void selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
	void addClicked();
	void projects_currentChanged( const QModelIndex& index );
};

#endif // ADDFILESDIALOG_H
