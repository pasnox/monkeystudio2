#include "UIGdbWatch.h"
//
#include <QCloseEvent>
//
#include <QMessageBox>

QPointer<UIGdbWatch> UIGdbWatch::_self = 0L;
//
UIGdbWatch* UIGdbWatch::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIGdbWatch( parent );
	return _self;
}
//
UIGdbWatch::UIGdbWatch( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	setAcceptDrops(true);
	connect(treeWidget, SIGNAL(pressed ( const QModelIndex &  ) ), this , SLOT(userDeleteVar ( const QModelIndex &   )));

}
//
void UIGdbWatch::closeEvent( QCloseEvent* e )
{
	e->accept();
}

//

void UIGdbWatch::dragEnterEvent(QDragEnterEvent *event)
{
     if (event->mimeData()->hasFormat("text/plain"))
         event->acceptProposedAction();
}

//

void UIGdbWatch::dropEvent(QDropEvent *event)
{
	QMessageBox::warning(NULL,"Event", event->mimeData()->text());
	QTreeWidgetItem *i = new QTreeWidgetItem(treeWidget);
	i->setText(0,event->mimeData()->text());

	emit userAddVar(event->mimeData()->text());
}

//

void UIGdbWatch::userDeleteVar ( const QModelIndex &  m)
{
	if( Qt::RightButton == QApplication::mouseButtons())
	{
		QTreeWidgetItem *i = treeWidget->currentItem ();
		if(i->parent() == NULL)
			delete i;
	}
}

//

QList< QTreeWidgetItem*> UIGdbWatch::getAllvar()
{
	QList< QTreeWidgetItem*> it ;
	for(int i=0 ; i< treeWidget->topLevelItemCount(); i++)
		it << treeWidget->topLevelItem(i);
	return it;
}
