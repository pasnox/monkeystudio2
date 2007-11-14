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

	static void execute( pAbstractChild*, QCloseEvent* ); // derecated

private:
	UISaveFiles( QWidget*, bool );
	void addFile( const QString&, bool, pAbstractChild* );

	UISaveFiles( pAbstractChild*, QCloseEvent* ); // deprecated
	void addFile( const QString&, bool ); // deprecated

	QCloseEvent* mEvent;
	QListWidget* lwFiles;
	QDialogButtonBox* dbbButtons;

private slots:
	void clicked( QAbstractButton* );

};

#endif // UISAVEFILES_H
