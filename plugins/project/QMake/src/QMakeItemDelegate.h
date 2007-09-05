#ifndef QMAKEITEMDELEGATE_H
#define QMAKEITEMDELEGATE_H

#include <QItemDelegate>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QToolButton>
#include <QEvent>
#include <QCursor>
#include <QFocusEvent>
#include <QApplication>

class QMakeItemDelegate : public QItemDelegate
{
	Q_OBJECT
	
public:
	QMakeItemDelegate( QWidget* );
	virtual QWidget* createEditor( QWidget* , const QStyleOptionViewItem&, const QModelIndex& ) const;
	virtual void setModelData( QWidget*, QAbstractItemModel* m, const QModelIndex& ) const;
	
};

class QValueFileFolderEdit : public QWidget
{
	Q_OBJECT
	
public:
	QValueFileFolderEdit( QWidget* p, const QModelIndex& i )
		: QWidget( p )
	{
		QHBoxLayout* hb = new QHBoxLayout( this );
		hb->setMargin( 0 );
		hb->setSpacing( 1 );
		hb->addWidget( ( cbValue = new QComboBox( this ) ), 100 );
		hb->addWidget( ( tbFile = new QToolButton( this ) ) );
		hb->addWidget( ( tbFolder = new QToolButton( this ) ) );
		tbFile->setIcon( QPixmap( ":/Icons/Icons/fileopen.png" ) );
		tbFolder->setIcon( QPixmap( ":/Icons/Icons/filesys-folder.png" ) );
		cbValue->setEditable( true );
		setText( i.data().toString() );
		
		setFocusProxy( cbValue );
		setFocusPolicy( Qt::ClickFocus );
		cbValue->installEventFilter( this );
	}
	
	bool eventFilter( QObject* o, QEvent* e )
	{
		if ( e->type() == QEvent::FocusOut )
			if ( !isAncestorOf( QApplication::widgetAt( QCursor::pos() ) ) )
				QApplication::sendEvent( this, e );
		return QWidget::eventFilter( o, e );
	}
	
	void setText( const QString& s )
	{
		if ( cbValue->findText( s ) == -1 )
			cbValue->addItem( s );
		cbValue->setEditText( s );
	}
	
	QString text() const
	{
		return cbValue->currentText();
	}
	
private:
	QComboBox* cbValue;
	QToolButton* tbFile;
	QToolButton* tbFolder;
	
};

#endif // QMAKEITEMDELEGATE_H
