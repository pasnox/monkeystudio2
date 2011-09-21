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
