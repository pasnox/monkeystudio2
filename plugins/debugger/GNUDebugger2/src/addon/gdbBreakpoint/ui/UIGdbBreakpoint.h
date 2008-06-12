#ifndef UIGDBBREAKPOINT_H
#define UIGDBBREAKPOINT_H
//
#include "ui_UIGdbBreakpoint.h"
//
#include <QPointer>
//
#include <QStandardItemModel>
#include "../gdbBreakpointStruct.h"

class UIGdbBreakpoint : public QWidget, public Ui::UIGdbBreakpoint
{
	Q_OBJECT
	//

public slots:

	void onEnableChanged(int );
	void onConditionChanged( );

public:
	static UIGdbBreakpoint* self( QWidget* = 0 );
	//
	void upDateData(const QList<Breakpoint *> & );

protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIGdbBreakpoint( QWidget* = 0 );
	static QPointer<UIGdbBreakpoint> _self;
	void addRow(QAbstractItemModel *model, QString hit, bool enable, QString condition, int line, QString file);
	
	QStandardItemModel *treeModel;
	//
signals:

	void enabledBreakpoint(const QString  &, const int &, const bool &);
	void conditionnedBreakpoint(const QString  &, const int &, const QString &);

};
//
#endif // UIGDBBREAKPOINT_H
