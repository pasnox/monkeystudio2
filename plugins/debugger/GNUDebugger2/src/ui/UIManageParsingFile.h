#ifndef UIMANAGEPARSINGFILE_H
#define UIMANAGEPARSINGFILE_H
//
#include "ui_UIManageParsingFile.h"
//
#include <QPointer>
//
#include "../kernel/gdbPatternFile.1.3.h"

#include "UIManageDelegate.h"


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
		void onSave();
	void onTextChanged(QString s);
};
//
#endif // UIMANAGEPARSINGFILE_H
