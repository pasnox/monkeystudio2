#ifndef QCTAGSSENSESEARCHPOPUP_H
#define QCTAGSSENSESEARCHPOPUP_H

#include <QTreeView>

class qCtagsSenseEntry;

class qCtagsSenseSearchPopup : public QTreeView
{
    Q_OBJECT
    
public:
    qCtagsSenseSearchPopup( QWidget* widget );
    virtual ~qCtagsSenseSearchPopup();
    
    void setWidget( QWidget* widget );
    inline QWidget* widget() const { return mWidget; }
    
    void showPopup( const QRect& rect = QRect() );

protected:
    bool eatFocusOut;
    QWidget* mWidget;
    
    virtual bool eventFilter( QObject* object, QEvent* event );
    bool searchPopupEventFilter( QEvent* event );

protected slots:
    void _q_activated( const QModelIndex& index );

signals:
    void fileNameActivated( const QString& fileName );
    void entryActivated( qCtagsSenseEntry* entry );
};

#endif // QCTAGSSENSESEARCHPOPUP_H
