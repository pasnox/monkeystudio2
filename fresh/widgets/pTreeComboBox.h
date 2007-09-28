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

#include <QComboBox>
#include <QPointer>

class QTreeView;
class QAbstractItemModel;

class pTreeComboBox : public QComboBox
{
	Q_OBJECT

public:
	pTreeComboBox( QWidget* = 0 );
	~pTreeComboBox();

	virtual void hidePopup();
	virtual void showPopup();

	QTreeView* view() const;
	void setView( QTreeView* );

	QAbstractItemModel* model() const;
	void setModel( QAbstractItemModel* );

	QModelIndex currentIndex() const;
	void setCurrentIndex( const QModelIndex& );

protected slots:
	void internal_currentChanged( const QModelIndex&, const QModelIndex& );
	void internal_clicked( const QModelIndex& );

protected:
	QPointer<QTreeView> mView;
	QPointer<QAbstractItemModel> mModel;

signals:
	void selected( const QModelIndex& );
	void clicked( const QModelIndex& );

};

#endif // PTREECOMBOBOX_H
