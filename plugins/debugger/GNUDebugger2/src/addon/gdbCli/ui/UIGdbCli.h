#ifndef UIGDCLI_H
#define UIGDCLI_H
//
#include "ui_UIGdbCli.h"
//
#include <QPointer>
//

//! Ui for show answer from Gdb in QTextEdit. It used by GdbCli class. 
/**
\author xiantia
\version 1.3.2
*/
class UIGdbCli : public QWidget, public Ui::UIGdbCli
{
	Q_OBJECT
	//
public:
	static UIGdbCli* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIGdbCli( QWidget* = 0 );
	static QPointer<UIGdbCli> _self;
	//
};
//
#endif // UIGDBBACKTRACE_H
