#include "UIGdbBreakpoint.h"
//
#include <QCloseEvent>
#include <QComboBox>
#include <QLineEdit>
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
			QTreeWidgetItem *i =  new QTreeWidgetItem(treeWidget);
			i->setText(2, bbp.condition);
			i->setText(3, QString::number(bbp.index));
			i->setText(4, QString::number(bbp.line));
			i->setText(5, bp->fileName);

			QComboBox * cb = new QComboBox(this);
			cb->addItem("True", true);
			cb->addItem("False", false);

			QLineEdit * le = new QLineEdit(bbp.condition);
			le->setAlignment(Qt::AlignHCenter);

			connect( cb, SIGNAL(currentIndexChanged ( int )), this , SLOT(onEnableChanged ( int )));
			connect( le, SIGNAL(editingFinished ()), this , SLOT(onConditionChanged ()));

			bbp.enable ? cb->setCurrentIndex(0) : cb->setCurrentIndex(1);
				
			bbp.hit ? i->setIcon(0,QIcon(":/icons/buttonok.png")) : i->setIcon(0,QIcon());
			// because under Qt4.4.0 the last row is not re-sizing (comboBox)
			i->setSizeHint( 1,cb->sizeHint() );			
			treeWidget->setItemWidget(i, 1, cb);
			treeWidget->setItemWidget(i, 2, le);
		}
	}
}

void UIGdbBreakpoint::onEnableChanged(int i)
{

	for(int j=0; j< treeWidget->topLevelItemCount(); j++)
	{	
		QTreeWidgetItem *it = (QTreeWidgetItem*) treeWidget->topLevelItem(j);
		QComboBox *cb = (QComboBox*) sender();
		if(treeWidget->itemWidget(it,1) == cb)
			emit enabledBreakpoint(it->text(5), it->text(3).toInt(), !cb->currentIndex());
	}
}

void UIGdbBreakpoint::onConditionChanged()
{

	for(int j=0; j< treeWidget->topLevelItemCount(); j++)
	{	
		QTreeWidgetItem *it = (QTreeWidgetItem*) treeWidget->topLevelItem(j);
		QLineEdit *le = (QLineEdit*) sender();
		if(treeWidget->itemWidget(it,2) == le)
			emit conditionnedBreakpoint(it->text(5), it->text(3).toInt(), le->text());
	}
}
