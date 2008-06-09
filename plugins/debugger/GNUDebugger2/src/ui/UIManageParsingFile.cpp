#include "UIManageParsingFile.h"
//
#include <QCloseEvent>
#include <QFile>
#include <QDataStream>
#include <QCheckBox>

//
QPointer<UIManageParsingFile> UIManageParsingFile::_self = 0L;
//
UIManageParsingFile* UIManageParsingFile::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIManageParsingFile( parent );
	return _self;
}
//
UIManageParsingFile::UIManageParsingFile( QWidget* parent )
	: QDialog( parent )
{
	setupUi( this );

	l = GdbPatternFile::instance()->get();
	
	foreach(GdbPattern p, *l)
	{
//		QCheckBox *cb= new QCheckBox();
		
		QTreeWidgetItem *i =  new QTreeWidgetItem(treeWidget);//QStringList() << QString::number(GdbPatternFile::instance()->getId( p ))
		//<< GdbPatternFile::instance()->getPattern( p ) << GdbPatternFile::instance()->getComment( p ));

		i->setText(2, QString::number(GdbPatternFile::instance()->getId( p )));
		i->setText(3, GdbPatternFile::instance()->getPattern( p ));
		i->setText(4, GdbPatternFile::instance()->getComment( p ));

//		cb->setTristate(true);
		p.enable ? i->setCheckState(1,Qt::Checked) : i->setCheckState(1,Qt::Unchecked);
//		i->setTristate(1, true);
		
		i->setFlags(i->flags() | Qt::ItemIsEditable);
		
		if( GdbPatternFile::instance()->getId( p ) >= 20000 ) i->setIcon(0, QIcon(":/icons/warningred.png"));
		else i->setIcon(0, QIcon(":/icons/warning.png"));

//		treeWidget->setItemWidget(i, 1, cb);

		//		treeWidget->addTopLevelItem(i);
	}
	connect(bSave, SIGNAL(clicked()), this,  SLOT(onSave()));


}
//
void UIManageParsingFile::closeEvent( QCloseEvent* e )
{
	e->accept();
}

/*void UIManageParsingFile::onSave()
{
	QFile f(GdbPatternFile::instance()->getFileName());

	f.open(QIODevice::WriteOnly);
	QDataStream s(&f);

	for(int i=0; i< treeWidget->topLevelItemCount (); i++)
	{
		QTreeWidgetItem *it = treeWidget->topLevelItem(i);
		s << QString("[" + it->data(0, Qt::DisplayRole).toString() + "]" + it->data(1, Qt::DisplayRole).toString() + "[" + it->data(2, Qt::DisplayRole).toString() + "]"  );
	}
	f.close();
}
*/

void UIManageParsingFile::onSave()
{
	for(int i=0; i< treeWidget->topLevelItemCount (); i++)
	{
		QTreeWidgetItem *it = treeWidget->topLevelItem(i);
		GdbPattern p = {it->data(2, Qt::DisplayRole).toString(), 
			QRegExp(it->data(1, Qt::DisplayRole).toString()),
			it->data(0, Qt::DisplayRole).toInt(), 
			it->checkState(0) == Qt::Checked ? true : false};
		l->replace(i, p);
	}
}
