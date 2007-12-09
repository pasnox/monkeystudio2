/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : pTreeComboBox
** FileName  : pTreeComboBox.cpp
** Date      : 2007-09-28T14:20:27
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pTreeComboBox.h"

#include <QVBoxLayout>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QComboBox>
#include <QHeaderView>
#include <QStyleOptionComboBox>
#include <QStylePainter>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QSizeGrip>

pTreeComboBox::pTreeComboBox( QWidget* w )
	: QWidget( w ), mFrame( new QFrame ), mView( 0 ), mModel( 0 ), mForce( false )
{
	mFrame->setWindowFlags( Qt::Popup );
	QVBoxLayout* vl = new QVBoxLayout( mFrame );
	vl->setSpacing( 0 );
	vl->setMargin( 0 );
	
	mIconSize = QSize( 16, 16 );
	mSizeHint = QComboBox().sizeHint();
	setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed ) );
	setView( new QTreeView );
	mView->header()->hide();
	mView->resize( mView->viewport()->size() );
}

pTreeComboBox::~pTreeComboBox()
{ delete mView; }

bool pTreeComboBox::eventFilter( QObject* o, QEvent* e )
{
	QEvent::Type t = e->type();
	if ( t == QEvent::Hide )
	{
		if ( currentIndex() != mIndex )
		{
			mForce = false;
			mView->clearSelection();
			mView->setCurrentIndex( mIndex );
			mForce = true;
		}
		update();
	}
	else if ( t == QEvent::MouseMove )
	{
		if ( QMouseEvent* me = dynamic_cast<QMouseEvent*>( e ) )
		{
			QModelIndex i = mView->indexAt( mView->mapFromGlobal( me->globalPos() ) );
			if ( mView->currentIndex() != i )
			{
				emit highlighted( i );
				mForce = false;
				mView->clearSelection();
				mView->setCurrentIndex( i );
				mForce = true;
			}
		}
	}
	return QWidget::eventFilter( o, e );
}

QSize pTreeComboBox::sizeHint() const
{ return mSizeHint; }

int recursiveCount( const QModelIndex& it )
{
	int j = 0;
	if ( !it.parent().isValid() )
		j++;
	for ( int i = 0; i < it.model()->rowCount( it ); i++ )
	{
		j++;
		if ( it.model()->rowCount( it.child( i, 0 ) ) )
			j += recursiveCount( it.child( i, 0 ) );
	}
	return j;
}

int pTreeComboBox::count() const
{ return recursiveCount( mModel->index( 0, 0 ) ); }

QSize pTreeComboBox::iconSize() const
{ return mIconSize; }

void pTreeComboBox::setIconSize( const QSize& s )
{
	if ( mIconSize != s )
	{
		mIconSize = s;
		update();
	}
}

void pTreeComboBox::paintEvent( QPaintEvent* )
{
	QStyleOptionComboBox o;

	// QStyleOption
	o.initFrom( this );
	
	// QStyleOptionComplex
	o.activeSubControls = 0;
	o.subControls = QStyle::SC_ComboBoxEditField | QStyle::SC_ComboBoxArrow | QStyle::SC_ComboBoxFrame;

	// QStyleOptionComboBox
	QModelIndex i = currentIndex();
	o.currentIcon = i.data( Qt::DecorationRole ).value<QIcon>();
	o.iconSize = mView && mView->iconSize() != QSize( -1, -1 ) ? mView->iconSize() : iconSize();
	o.currentText = i.data( Qt::DisplayRole ).toString();
	o.editable = false;
	o.frame = true;
	o.popupRect = QRect();
	
	if ( !mFrame->isVisible() && rect().contains( mapFromGlobal( QCursor::pos() ) ) )
		o.state |= QStyle::State_MouseOver;
	
	if ( mFrame->isVisible() )
		o.state |= QStyle::State_On;
	
	QStylePainter p( this );
	p.drawComplexControl( QStyle::CC_ComboBox, o );
	p.drawControl( QStyle::CE_ComboBoxLabel, o );
}

void pTreeComboBox::hideEvent( QHideEvent* )
{ hidePopup(); }

void pTreeComboBox::enterEvent( QEvent* )
{ update(); }

void pTreeComboBox::leaveEvent( QEvent* )
{ update(); }

void pTreeComboBox::mousePressEvent( QMouseEvent* )
{
	if ( !mView )
		return;
	mFrame->isVisible() ? hidePopup() : showPopup();
}

void pTreeComboBox::hidePopup()
{
	if ( mFrame->isVisible() )
		mFrame->hide();
}

void pTreeComboBox::calculPopupGeometry() // code copied from QComboBox  original class from Trolltech, arrange to feet my needs
{
	//int itemHeight = mView->sizeHintForIndex( mModel->index( 0, 0 ) ).height();
	QRect listRect( rect() );
	//listRect.setHeight(itemHeight * count() +( 2 *mFrame->layout()->spacing() ) +( 2 *mFrame->frameWidth() ) );
	QRect screen = QApplication::desktop()->screenGeometry(this);
	QPoint below = mapToGlobal(listRect.bottomLeft());
	int belowHeight = screen.bottom() - below.y();
	QPoint above = mapToGlobal(listRect.topLeft());
	int aboveHeight = above.y() - screen.y();
	
	// make sure the widget fits on screen
		if (listRect.width() > screen.width() )
		listRect.setWidth(screen.width());
	if (mapToGlobal(listRect.bottomRight()).x() > screen.right()) {
		below.setX(screen.x() + screen.width() - listRect.width());
		above.setX(screen.x() + screen.width() - listRect.width());
	}
	if (mapToGlobal(listRect.topLeft()).x() < screen.x() ) {
		below.setX(screen.x());
		above.setX(screen.x());
	}
		
	if (listRect.height() <= belowHeight) {
		listRect.moveTopLeft(below);
	} else if (listRect.height() <= aboveHeight) {
		listRect.moveBottomLeft(above);
	} else if (belowHeight >= aboveHeight) {
		listRect.setHeight(belowHeight);
		listRect.moveTopLeft(below);
	} else {
		listRect.setHeight(aboveHeight);
		listRect.moveBottomLeft(above);
	}

	mFrame->setGeometry( listRect );
}

void pTreeComboBox::showPopup()
{
	if ( !mFrame->isVisible() && mView )
	{
		mIndex = currentIndex();
		calculPopupGeometry();
		mFrame->show();
		update();
	}
}

QTreeView* pTreeComboBox::view() const
{ return mView; }

void pTreeComboBox::setView( QTreeView* t )
{
	if ( mView == t )
		return;
	delete mView;
	mView = t;
	if ( mView )
	{
		if ( mFrame->layout()->count() )
			qobject_cast<QVBoxLayout*>( mFrame->layout() )->insertWidget( 0, mView );
		else
		{
			mFrame->layout()->addWidget( mView );
			mFrame->layout()->addWidget( new QSizeGrip( mFrame ) );
		}
		mView->setEditTriggers( QAbstractItemView::NoEditTriggers );
		mView->setMouseTracking( true );
		mView->viewport()->installEventFilter( this );
		setModel( mModel );
		connect( mView, SIGNAL( activated( const QModelIndex& ) ), this, SLOT( internal_activated( const QModelIndex& ) ) );
		connect( mView, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( internal_clicked( const QModelIndex& ) ) );
	}
}

QAbstractItemModel* pTreeComboBox::model() const
{ return mModel; }

void pTreeComboBox::setModel( QAbstractItemModel* m )
{
	if ( mModel != m )
		mModel = m;
	if ( mView && mView->model() != mModel )
	{
		mView->setModel( mModel );
		connect( mView->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( internal_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	}
}

QModelIndex pTreeComboBox::rootIndex() const
{ return mView ? mView->rootIndex() : QModelIndex(); }

void pTreeComboBox::setRootIndex( const QModelIndex& i )
{ if ( mView ) mView->setRootIndex( i ); }

QModelIndex pTreeComboBox::currentIndex() const
{
	if ( mView )
		return mFrame->isVisible() ? mIndex : mView->currentIndex();
	return QModelIndex();
}

void pTreeComboBox::setCurrentIndex( const QModelIndex& i )
{
	if ( mView && ( currentIndex() != i || !i.isValid() ) )
	{
		mIndex = i;
		mForce = true;
		mView->clearSelection();
		mView->setCurrentIndex( i );
		mForce = false;
		update();
	}
}

void pTreeComboBox::expandAll()
{ if ( mView ) mView->expandAll(); }

void pTreeComboBox::internal_activated( const QModelIndex& i )
{
	if ( mIndex != i )
	{
		mIndex = i;
		emit currentChanged( i );
	}
	emit activated( i );
	hidePopup();
}

void pTreeComboBox::internal_clicked( const QModelIndex& i )
{
	if ( mIndex != i )
	{
		mIndex = i;
		emit currentChanged( i );
	}
	emit clicked( i );
	hidePopup();
}

void pTreeComboBox::internal_currentChanged( const QModelIndex& c, const QModelIndex& )
{
	if ( mForce )
		emit currentChanged( c );
}
