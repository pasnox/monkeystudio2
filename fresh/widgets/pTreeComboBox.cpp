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

#include <QTreeView>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QStyleOptionComboBox>
#include <QStylePainter>
#include <QEvent>

pTreeComboBox::pTreeComboBox( QWidget* w )
	: QWidget( w ), mView( 0 ), mModel( 0 )
{
	mIconSize = QSize( 16, 16 );
	setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed ) );
	setView( new QTreeView );
	mView->header()->hide();
	mView->resize( width(), 200 );
}

pTreeComboBox::~pTreeComboBox()
{ delete mView; }

bool pTreeComboBox::eventFilter( QObject* o, QEvent* e )
{
	QEvent::Type t = e->type();
	if ( t == QEvent::Enter )
	{
		qWarning( "focus in" );
		e->accept();
		return true;
	}
	else if ( t == QEvent::Leave )
	{
		qWarning( "focus out" );
		e->accept();
		return true;
	}
	return QWidget::eventFilter( o, e );
}

QSize pTreeComboBox::sizeHint() const
{ return QSize( 79, 22 ); }

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
	o.currentIcon = currentIndex().data( Qt::DecorationRole ).value<QIcon>();
	o.iconSize = iconSize();
	o.currentText = currentIndex().data( Qt::DisplayRole ).toString();
	o.editable = false;
	o.frame = true;
	o.popupRect = QRect();
	
	if ( !mView || ( mView && !mView->isVisible() && rect().contains( mapFromGlobal( QCursor::pos() ) ) ) )
		o.state |= QStyle::State_MouseOver;
	
	if ( mView && mView->isVisible() )
		o.state |= QStyle::State_On;
	
	QStylePainter p( this );
	p.drawComplexControl( QStyle::CC_ComboBox, o );
	p.drawControl( QStyle::CE_ComboBoxLabel, o );
}

void pTreeComboBox::hideEvent( QHideEvent* )
{
	if ( mView && mView->isVisible() )
		hidePopup();
}

void pTreeComboBox::enterEvent( QEvent* )
{ update(); }

void pTreeComboBox::leaveEvent( QEvent* )
{ update(); }

void pTreeComboBox::mousePressEvent( QMouseEvent* )
{
	if ( !mView )
		return;
	
	if ( mView->isVisible() )
		hidePopup();
	else
		showPopup();
}

void pTreeComboBox::hidePopup()
{
	if ( mView )
	{
		mView->hide();
		update();
	}
}

void pTreeComboBox::showPopup()
{
	if ( mView )
	{
		mView->expandAll();
		mView->move( mapToGlobal( rect().bottomLeft() ) );
		mView->resize( width(), mView->height() );
		mView->show();
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
		mView->window()->setWindowFlags( Qt::Popup );
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
	if ( mView && mView->model() != m )
	{
		mModel = m;
		mView->setModel( mModel );
		connect( mView->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( internal_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	}
}

QModelIndex pTreeComboBox::currentIndex() const
{ return mView ? mView->currentIndex() : QModelIndex(); }

void pTreeComboBox::setCurrentIndex( const QModelIndex& i )
{
	if ( mView )
		mView->setCurrentIndex( i );
}

void pTreeComboBox::internal_activated( const QModelIndex& i )
{
	emit activated( i );
	hidePopup();
}

void pTreeComboBox::internal_clicked( const QModelIndex& i )
{
	emit clicked( i );
	hidePopup();
}

void pTreeComboBox::internal_currentChanged( const QModelIndex& c, const QModelIndex& )
{ emit selected( c ); }
