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


	// set model
	model = new QStandardItemModel(0,4);
	treeView->setModel(model);
    delegate = new UIManageDelegate(this);
	treeView->setItemDelegate(delegate);
    treeView->setAlternatingRowColors(true);

	model->setHeaderData(0, Qt::Horizontal, tr("Id"));
	model->setHeaderData(1, Qt::Horizontal, tr("Expert"));
	model->setHeaderData(2, Qt::Horizontal, tr("Pattern"));
	model->setHeaderData(3, Qt::Horizontal, tr("Comment"));

	l = GdbPatternFile::instance()->get();
	
	foreach(GdbPattern p, *l)
	{
		int i = model->rowCount();
		model->insertRow(i);

		model->setData(model->index(i, 0), GdbPatternFile::instance()->getId( p ) );
	
		if( GdbPatternFile::instance()->getId( p ) >= 20000 ) 
			model->setData(model->index(i, 0), QIcon(":/icons/warningred.png"), Qt::DecorationRole );
		else 
			model->setData(model->index(i, 0), QIcon(":/icons/warning.png"), Qt::DecorationRole );

		switch (p.enable)
		{
		case 0 :  model->setData(model->index(i, 1), tr("No use"));
			break;
		case 1 :  model->setData(model->index(i, 1), tr("Allway use"));
			break;
		case 2 :  model->setData(model->index(i, 1), tr("Show message"));
			break;
		}

		model->setData(model->index(i, 1), p.enable , Qt::UserRole);

		model->setData(model->index(i, 2), GdbPatternFile::instance()->getPattern( p ) );
		model->setData(model->index(i, 3), GdbPatternFile::instance()->getComment( p ) );
	}
	connect(bSave, SIGNAL(clicked()), this,  SLOT(onSave()));
}

//

void UIManageParsingFile::closeEvent( QCloseEvent* e )
{
	e->accept();
}


void UIManageParsingFile::onSave()
{
	for(int i=0; i< model->rowCount(); i++)
	{
		GdbPattern p = {model->data(model->index(i, 3)).toString(), // comment 
			QRegExp(model->data(model->index(i, 2)).toString()), // pattern
			model->data(model->index(i, 0)).toInt(),  // id
			model->data(model->index(i, 1), Qt::UserRole).toInt()};
		l->replace(i, p);
	}
}
