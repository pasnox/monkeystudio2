#ifndef UIGDBBREAKPOINT_H
#define UIGDBBREAKPOINT_H
//
#include "ui_UIGdbBreakpoint.h"
//
#include <QPointer>
//
#include "../gdbBreakpointStruct.h"

#include "UIBreakpointDelegate.h"

//! Ui for show breakpoints in QTabWiget. It used by GdbBreakpoint class. 
/**
\author xiantia
\version 1.3.2
*/

class UIGdbBreakpoint : public QWidget, public Ui::UIGdbBreakpoint
{
	Q_OBJECT
	//

public slots:

	void onEnableChanged(int );
	void onConditionChanged( );
	void onItemChanged(const QModelIndex & , const QModelIndex & );

public:
	static UIGdbBreakpoint* self( QWidget* = 0 );
	//
	void upDateData(const QList<Breakpoint *> & );
	UIGdbBreakpoint( QWidget* = 0 );

protected:
	void closeEvent( QCloseEvent* );
	//
private:
	static QPointer<UIGdbBreakpoint> _self;
//	void addRow(QAbstractItemModel *model, QString hit, bool enable, QString condition, int line, QString file);
	
	QStandardItemModel *model;
	UIBreakpointDelegate *delegate;

signals:

	void enabledBreakpoint(const QString  &, const int &, const bool &);
	void conditionnedBreakpoint(const QString  &, const int &, const QString &);

};
//
#endif // UIGDBBREAKPOINT_H
