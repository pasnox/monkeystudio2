/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#ifndef PCHECKCOMBOBOX_H
#define PCHECKCOMBOBOX_H

#include <QComboBox>
#include <QStyledItemDelegate>

class QEvent;
class QListView;

class pGenericTableModel;

class pCheckComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    pCheckComboBoxDelegate( QObject* parent, QComboBox* cmb );

    static bool isSeparator( const QModelIndex& index );
    static void setSeparator( QAbstractItemModel* model, const QModelIndex& index, bool set );

protected:
    virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
    QComboBox *mCombo;
};

class pCheckComboBox : public QComboBox
{
    Q_OBJECT
    
public:
    pCheckComboBox( QWidget* parent = 0 );
    virtual ~pCheckComboBox();
    
    virtual void showPopup();
    
    void insertSeparator( int index );
    void addSeparator();
    
    bool isSeparator( int index );
    void setSeparator( int index, bool set = true );
    
    QList<int> checkedRows() const;
    QStringList checkedStringList() const;
    QModelIndexList checkedIndexes() const;
    void clearCheckStates();

public slots:
    virtual void retranslateUi();
    
protected:
    pGenericTableModel* mModel;
    QListView* mView;
    pCheckComboBoxDelegate* mDelegate;
    
    virtual bool event( QEvent* event );
    virtual void changeEvent( QEvent* event );
    virtual void paintEvent( QPaintEvent* event );
    
    QModelIndex modelIndex( int index ) const;
    QString text() const;
};

#endif // PCHECKCOMBOBOX_H
