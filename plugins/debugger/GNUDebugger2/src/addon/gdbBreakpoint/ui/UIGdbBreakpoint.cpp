#include "UIGdbBreakpoint.h"
//
#include <QCloseEvent>
#include <QComboBox>
 //

QPointer<UIGdbBreakpoint> UIGdbBreakpoint::_self = 0L;
//

UIGdbBreakpoint* UIGdbBreakpoint::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIGdbBreakpoint( parent );
	return _self;
}

//

UIGdbBreakpoint::UIGdbBreakpoint( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	treeWidget->setRootIsDecorated(false);
    treeWidget->setAlternatingRowColors(true);
	treeWidget->setColumnWidth ( 0, QPixmap(":/icons/buttonok.png").size().width()) ;
}

//

void UIGdbBreakpoint::closeEvent( QCloseEvent* e )
{
	e->accept();
}

//

void UIGdbBreakpoint::upDateData(const QList<Breakpoint *> & bl)
{
	treeWidget->clear();

	// foreach Breakpoint
	foreach(Breakpoint *bp, bl)
	{
		// for one file
		foreach(BaseBreakpoint bbp, bp->bp)
		{
			QComboBox * cb = new QComboBox();
			cb->addItem("True", true);
			cb->addItem("False", false);

			connect( cb, SIGNAL(currentIndexChanged ( int )), this , SLOT(onCurrentIndexChanged ( int )));

			bbp.enable ? cb->setCurrentIndex(0) : cb->setCurrentIndex(1);

			QTreeWidgetItem *i =  new QTreeWidgetItem(treeWidget);
			i->setText(2, bbp.condition);
			i->setText(3, QString::number(bbp.index));
			i->setText(4, QString::number(bbp.line));
			i->setText(5, bp->fileName);
			
//			QTreeWidgetItem *i =  new QTreeWidgetItem(QStringList() << QString()/* id */ << QString() /* ComboBox */
//				<< bbp.condition << QString::number(bbp.index) << QString::number(bbp.line) << bp->fileName);
				
		//	i->setFlags(i->flags() | Qt::ItemIsEditable);
			bbp.hit ? i->setIcon(0,QIcon(":/icons/buttonok.png")) : i->setIcon(0,QIcon());
//			treeWidget->addTopLevelItem(i);
			treeWidget->setItemWidget(i, 1, cb);
		}
	}
}

void UIGdbBreakpoint::onCurrentIndexChanged(int i)
{

	for(int j=0; j< treeWidget->topLevelItemCount(); j++)
	{	
		QTreeWidgetItem *it = (QTreeWidgetItem*) treeWidget->topLevelItem(j);
		QComboBox *cb = (QComboBox*) sender();
		if(treeWidget->itemWidget(it,1) == cb)
		{
			emit enabledBreakpoint(it->text(5), it->text(3).toInt(), !cb->currentIndex());
		}
	}
}

