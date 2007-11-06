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
	static void execute( pAbstractChild*, QCloseEvent* );

private:
	UISaveFiles( pAbstractChild*, QCloseEvent* );
	void addFile( const QString&, bool );

	pAbstractChild* mChild;
	QCloseEvent* mEvent;
	QListWidget* lwFiles;
	QDialogButtonBox* dbbButtons;

private slots:
	void clicked( QAbstractButton* );

};

#endif // UISAVEFILES_H
