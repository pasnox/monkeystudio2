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
	\file pTreeComboBox.h
	\date 2008-01-14T00:27:56
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief An extended QComboBox
*/
#ifndef PTREECOMBOBOX_H
#define PTREECOMBOBOX_H

#include "../objects/MonkeyExport.h"

#include <QWidget>
#include <QModelIndex>
#include <QPointer>

class QFrame;
class QTreeView;
class QAbstractItemModel;

/*!
	\brief An extended QComboBox
	\details that handle a QTreeView
*/
class Q_MONKEY_EXPORT pTreeComboBox : public QWidget
{
	Q_OBJECT

public:
	pTreeComboBox( QWidget* parent = 0 );
	~pTreeComboBox();

	bool eventFilter( QObject*, QEvent* );

	virtual QSize sizeHint() const;
	int count() const;

	virtual QSize iconSize() const;
	virtual void setIconSize( const QSize& );

	virtual void hidePopup();
	virtual void showPopup();

	QTreeView* view() const;
	void setView( QTreeView* );

	QAbstractItemModel* model() const;
	void setModel( QAbstractItemModel* );

	QModelIndex rootIndex() const;
	void setRootIndex( const QModelIndex& );

	QModelIndex currentIndex() const;
	void setCurrentIndex( const QModelIndex& );
	
	void expandAll();

protected slots:
	void internal_activated( const QModelIndex& );
	void internal_clicked( const QModelIndex& );
	void internal_currentChanged( const QModelIndex&, const QModelIndex& );

protected:
	QSize mIconSize;
	QSize mSizeHint;
	QFrame* mFrame;
	QPointer<QTreeView> mView;
	QPointer<QAbstractItemModel> mModel;
	QModelIndex mIndex;
	bool mForce;

	void paintEvent( QPaintEvent* );
	void hideEvent( QHideEvent* );
	void enterEvent( QEvent* );
	void leaveEvent( QEvent* );
	void mousePressEvent( QMouseEvent* );

	void calculPopupGeometry();

signals:
	void activated( const QModelIndex& );
	void clicked( const QModelIndex& );
	void currentChanged( const QModelIndex& );
	void highlighted( const QModelIndex& );

};

#endif // PTREECOMBOBOX_H
