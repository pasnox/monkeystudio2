#include "UIGdbBreakpoint.h"
//
#include <QCloseEvent>
#include <QStandardItemModel>
 //
#include "treeDelegate.h"

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

	
	treeModel = new QStandardItemModel(0, 6);

	treeModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Hit"));
	treeModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Enable"));
	treeModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Condition"));
	treeModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Index"));
	treeModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Line"));
	treeModel->setHeaderData(5, Qt::Horizontal, QObject::tr("File"));

	treeView->setRootIsDecorated(false);
    treeView->setAlternatingRowColors(true);
	treeView->setModel(treeModel);
	treeView->setColumnWidth ( 0, QPixmap(":/icons/buttonok.png").size().width()) ;

	treeView->setItemDelegate( new TreeDelegate(this));
}

//

void UIGdbBreakpoint::closeEvent( QCloseEvent* e )
{
	e->accept();
}

//

void UIGdbBreakpoint::upDateData(const QList<Breakpoint *> & bl)
{
	// get the current row count and delete this
	int rc = treeModel->rowCount();
	treeModel->removeRows(0, rc);
	
	// foreach Breakpoint
	foreach(Breakpoint *bp, bl)
	{
		// for one file
		foreach(BaseBreakpoint bbp, bp->bp)
		{
			rc = treeModel->rowCount();
			treeModel->insertRow(rc);
		
			treeModel->setData(treeModel->index(rc,0), bbp.hit ? QIcon(":/icons/buttonok.png"): QIcon(), Qt::DecorationRole);
			treeModel->setData(treeModel->index(rc,1), bbp.enable ? "True": "False");
			treeModel->setData(treeModel->index(rc,2), bbp.condition);
			treeModel->setData(treeModel->index(rc,3), bbp.index);
			treeModel->setData(treeModel->index(rc,4), bbp.line);
			treeModel->setData(treeModel->index(rc,5), bp->fileName);
		}
	}
}

/*
<PasNox> deja utiliser le model directmeent na aucun sens !
<PasNox> pk tu fais ca ?
<PasNox> utilsie des QTreeItem pour ajouter des items
<PasNox> puis
<PasNox> setItemDelegate( item, delagate )
<PasNox> ensuite au lieu d'utiliser plein de data
<PasNox> creer une structure
<PasNox> que tu register avec qRegisterMetaType, comme ca tu pourra la stocker dans des variant
<PasNox> struct BreakPoint
<PasNox> {
<PasNox> bool enable;
<PasNox> string condition;
<PasNox> ...
<PasNox> };
<PasNox> qRegisterMetaType<BreakPoint>( "BreakPoint" )
<PasNox> item->setData( UserRole, QVariant::fromValue( breakpoiiinstructure ) );
<xiantia> oui ok, deja je veut just que lorsque je vait sur "enable" il m'ouvre un QComboBox sur la column 1
<PasNox> BreakPoint bp = item->data( userrole ).value<BreakPoint>();
*/

