#ifndef UIGDBWATCH_H
#define UIGDBWATCH_H
//
#include "ui_UIGdbWatch.h"
//
#include <QPointer>
//
class UIGdbWatch : public QWidget, public Ui::UIGdbWatch
{
	Q_OBJECT
	//
public:
	static UIGdbWatch* self( QWidget* = 0 );
	//
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	QStringList getAllvar();

public slots:

	void userDeleteVar ( const QModelIndex &  ) ;

protected:

	void closeEvent( QCloseEvent* );
	//
private:
	UIGdbWatch( QWidget* = 0 );
	static QPointer<UIGdbWatch> _self;
	//
signals:

	void userAddVar(const QString &);

};
//
#endif // UIGDBBACKTRACE_H
