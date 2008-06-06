#include "UIManageParsingFile.h"
//
#include <QCloseEvent>
#include <QFile>
#include <QdataStream>

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
		QTreeWidgetItem *i =  new QTreeWidgetItem(QStringList() << QString::number(GdbPatternFile::instance()->getId( p ))
		<< GdbPatternFile::instance()->getPattern( p ) << GdbPatternFile::instance()->getComment( p ));

		i->setFlags(i->flags() | Qt::ItemIsEditable);
		i->setIcon(0, QIcon(":/icons/buttonok.png"));
		treeWidget->addTopLevelItem(i);
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
		GdbPattern p = {it->data(2, Qt::DisplayRole).toString(),  QRegExp(it->data(1, Qt::DisplayRole).toString()), it->data(0, Qt::DisplayRole).toInt() };
		l->replace(i, p);
	}
}
