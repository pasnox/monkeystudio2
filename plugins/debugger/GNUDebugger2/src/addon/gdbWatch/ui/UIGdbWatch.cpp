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
	\file UIGdbWatch.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Ui for show variables values in QTabWiget. It used by GdbWatch class. 
*/

#include "UIGdbWatch.h"
//
#include <QCloseEvent>
//
#include <QMessageBox>

QPointer<UIGdbWatch> UIGdbWatch::_self = 0L;
//
/*!
	\details Static function for create new object
	\param parent of this object
*/
UIGdbWatch* UIGdbWatch::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIGdbWatch( parent );
	return _self;
}
//

/*!
	\details Create new object
	\param parent of this object
*/
UIGdbWatch::UIGdbWatch( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	setAcceptDrops(true);
	connect(treeWidget, SIGNAL(pressed ( const QModelIndex &  ) ), this , SLOT(userDeleteVar ( const QModelIndex &   )));

}
//

/*!
	\details Close event
	\param e is the event
*/
void UIGdbWatch::closeEvent( QCloseEvent* e )
{
	e->accept();
}

//
/*!
	\details Enter drag event
	\param event is QDragEnterEvent event
*/
void UIGdbWatch::dragEnterEvent(QDragEnterEvent *event)
{
     if (event->mimeData()->hasFormat("text/plain"))
         event->acceptProposedAction();
}

//
/*!
	\details Enter drop event
	\param event is QDropEvent event
*/
void UIGdbWatch::dropEvent(QDropEvent *event)
{
//	QMessageBox::warning(NULL,"Event", event->mimeData()->text());
	QTreeWidgetItem *i = new QTreeWidgetItem(treeWidget);
	i->setText(0,event->mimeData()->text());
	emit userAddVar(event->mimeData()->text());
}

//

/*!
	\details User has deleted var in Ui
	\param m is QModelIndex
*/
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
/*!
	\details Get list of all var in Ui
	\retval List of var
*/
QStringList UIGdbWatch::getAllvar()
{
	QStringList it ;
	for(int i=0 ; i< treeWidget->topLevelItemCount(); i++)
		it << treeWidget->topLevelItem(i)->text(0);
	return it;
}
