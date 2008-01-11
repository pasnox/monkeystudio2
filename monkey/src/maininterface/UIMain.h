/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef UIMAIN_H
#define UIMAIN_H

#include "MonkeyExport.h"
#include "pMainWindow.h"

class QActionGroup;

class Q_MONKEY_EXPORT UIMain : public pMainWindow
{
	Q_OBJECT
	friend class MonkeyCore;

public:
	QMenu* createPopupMenu();

protected:
	QActionGroup* agStyles;

	UIMain( QWidget* = 0 );
	void initGui();
	void closeEvent( QCloseEvent* );
	void initMenuBar();
	void initToolBar();
	void initConnections();

protected slots:
	void menu_aboutToShow();
};

#endif // UIMAIN_H
