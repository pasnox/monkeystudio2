/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/

/*!
	\file UIGdbBreakpoint.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Ui for show breakpoints in QTabWiget. It used by GdbBreakpoint class. 
*/

#include "UIGdbBreakpoint.h"
//
#include <QCloseEvent>
#include <QComboBox>
#include <QLineEdit>
 //
#include <QMessageBox>

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

    	// set model
	model = new QStandardItemModel(0,6);
	treeView->setModel(model);
    delegate = new UIBreakpointDelegate(this);
	treeView->setItemDelegate(delegate);
    treeView->setAlternatingRowColors(true);

	model->setHeaderData(0, Qt::Horizontal, tr("Hit"));
	model->setHeaderData(1, Qt::Horizontal, tr("Enable"));
	model->setHeaderData(2, Qt::Horizontal, tr("Condition"));
	model->setHeaderData(3, Qt::Horizontal, tr("Index"));
	model->setHeaderData(4, Qt::Horizontal, tr("Line"));
	model->setHeaderData(5, Qt::Horizontal, tr("File"));

	connect(model, SIGNAL(dataChanged ( const QModelIndex & , const QModelIndex &  )), this, SLOT(onItemChanged ( const QModelIndex & , const QModelIndex &  )));
	treeView->setColumnWidth ( 0, QPixmap(":/icons/buttonok.png").size().width()) ;
	treeView->setRootIsDecorated(false);
}

//

void UIGdbBreakpoint::closeEvent( QCloseEvent* e )
{
	e->accept();
}

//

void UIGdbBreakpoint::upDateData(const QList<Breakpoint *> & bl)
{
	model->removeRows(0, model->rowCount());

	// foreach Breakpoint
	foreach(Breakpoint *bp, bl)
	{
		// for one file
		foreach(BaseBreakpoint bbp, bp->bp)
		{
			int i = model->rowCount();
			model->insertRow(i);

			bbp.enable ? model->setData(model->index(i, 1), "True" ) :model->setData(model->index(i, 1), "False" );
			model->setData(model->index(i, 2),  bbp.condition );
			model->setData(model->index(i, 3), QString::number(bbp.index) );
			model->setData(model->index(i, 4), QString::number(bbp.line) );
			model->setData(model->index(i, 5), bp->fileName);

			if( bbp.hit ) 
				model->setData(model->index(i, 0), QIcon(":/icons/buttonok.png"), Qt::DecorationRole );
			else 
				model->setData(model->index(i, 0), QIcon(), Qt::DecorationRole );
		}
	}
}


void UIGdbBreakpoint::onItemChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{

	for(int j=0; j< model->rowCount(); j++)
	{	
		if(model->index(j,1) == topLeft && model->index(j,5).data().toString() !="" )
			emit enabledBreakpoint(model->index(j,5).data().toString(), model->index(j,3).data().toInt(),!model->index(j,1).data(Qt::UserRole).toInt() );
		if(model->index(j,2) == topLeft && model->index(j,5).data().toString() !="" )
			emit conditionnedBreakpoint(model->index(j,5).data().toString(), model->index(j,3).data().toInt(), model->index(j,2).data().toString());
	}
}


void UIGdbBreakpoint::onEnableChanged(int i)
{
/*
	QComboBox *cb = (QComboBox*) sender();
	for(int j=0; j< model->rowCount(); j++)
	{	
		QModelIndex  m = model->index(j,1);
		if( (QComboBox*)(m.data()) == cb)
			emit enabledBreakpoint(model->index(j,5).data().toString(), model->index(j,3).data().toInt(), !cb->currentIndex());
	}
*/}

void UIGdbBreakpoint::onConditionChanged()
{

/*	for(int j=0; j< treeWidget->topLevelItemCount(); j++)
	{	
		QTreeWidgetItem *it = (QTreeWidgetItem*) treeWidget->topLevelItem(j);
		QLineEdit *le = (QLineEdit*) sender();
		if(treeWidget->itemWidget(it,2) == le)
			emit conditionnedBreakpoint(it->text(5), it->text(3).toInt(), le->text());
	}
*/}

