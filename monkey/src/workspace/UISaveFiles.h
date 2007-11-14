#ifndef UISAVEFILES_H
#define UISAVEFILES_H

#include <QDialog>

#include "MonkeyExport.h"

class pAbstractChild;
class QListWidget;
class QAbstractButton;
class QDialogButtonBox;

class Q_MONKEY_EXPORT UISaveFiles : public QDialog
{
	Q_OBJECT

public:
	enum Buttons { bSaveSelected = 0, bDiscardAll, bCancelClose };
	static UISaveFiles::Buttons saveDocuments( QWidget*, QList<pAbstractChild*>, bool );
	static UISaveFiles::Buttons saveDocument( QWidget*, pAbstractChild*, bool );

private:
	UISaveFiles( QWidget*, bool );
	void addFile( const QString&, pAbstractChild* );

	QListWidget* lwFiles;
	QDialogButtonBox* dbbButtons;

private slots:
	void clicked( QAbstractButton* );

};

#endif // UISAVEFILES_H
