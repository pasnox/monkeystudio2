#ifndef UIMANAGEPARSINGFILE_H
#define UIMANAGEPARSINGFILE_H
//
#include "ui_UIManageParsingFile.h"
//
#include <QPointer>
//
#include "../kernel/gdbPatternFile.1.3.h"

#include "UIManageDelegate.h"

//! Manage parsing file
/**
\author xiantia.

This Ui can modifit the parsing file. It use QTreeWidget and Delegate for this.
You can find pattern too and change values.
*/

class UIManageParsingFile : public QDialog, public Ui::UIManageParsingFile
{
	Q_OBJECT
	//
public:
	static UIManageParsingFile* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIManageParsingFile( QWidget* = 0 );
	static QPointer<UIManageParsingFile> _self;
	//
	QList<GdbPattern> * l;

	QStandardItemModel *model;
	UIManageDelegate *delegate;

public slots:
	//! Save
	void onSave();
	//! Find
	void onTextChanged(QString s);
};
//
#endif // UIMANAGEPARSINGFILE_H
