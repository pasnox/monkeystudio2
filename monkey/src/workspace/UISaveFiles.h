/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UISaveFiles.h
** Date      : 2007-11-04T22:46:54
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
