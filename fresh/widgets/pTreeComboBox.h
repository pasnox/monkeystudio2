/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : pTreeComboBox
** FileName  : pTreeComboBox.h
** Date      : 2007-09-28T14:20:27
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PTREECOMBOBOX_H
#define PTREECOMBOBOX_H

#include "MonkeyExport.h"

#include <QWidget>
#include <QModelIndex>
#include <QPointer>

class QFrame;
class QTreeView;
class QAbstractItemModel;

class Q_MONKEY_EXPORT pTreeComboBox : public QWidget
{
	Q_OBJECT

public:
	pTreeComboBox( QWidget* = 0 );
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
