/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pDockToolBar.cpp
** Date      : 2008-01-14T00:27:40
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#include "pDockToolBar.h"
#include "pDockToolBarManager.h"
#include "pTabbedWorkspaceCornerButton.h"

#include <QButtonGroup>
#include <QFrame>
#include <QBoxLayout>
#include <QEvent>
#include <QDockWidget>
#include <QAction>
#include <QMainWindow>
#include <QKeyEvent>

pDockToolBar::pDockToolBar( pDockToolBarManager* t, Qt::Orientation o, QMainWindow* w )
	: QToolBar( w ), mManager( t ), mUniqueId( 0 )
{
	// need docktoolbar manager
	Q_ASSERT( t != 0 );
	Q_UNUSED( t );

	// toolbar is not movable
	setMovable( false );
	
	// toggle exclusive action
	aToggleExclusive = new QAction( this );
	aToggleExclusive->setCheckable( true );
	aToggleExclusive->setChecked( true );

	// change font
	QFont f( font() );
	f.setPointSize( 8 );
	setFont( f );

	// set icon size
	setIconSize( QSize( 16, 16 ) );

	// create button frame
	mFrame = new QFrame;

	// create buttons layout
	mLayout = new QBoxLayout( QBoxLayout::LeftToRight, mFrame );
	mLayout->setMargin( 0 );
	mLayout->setSpacing( 0 );

	// add frame to toolbar
	aDockFrame = addWidget( mFrame );

	// connect orientation change
	connect( this, SIGNAL( orientationChanged( Qt::Orientation ) ), this, SLOT( internal_orientationChanged( Qt::Orientation ) ) );

	// set toolbar/layout orientation
	setOrientation( o );
}

bool pDockToolBar::eventFilter( QObject* o, QEvent* e )
{
	// get event type
	QEvent::Type t = e->type();
	// try casting object to dockwidget
	QDockWidget* d = qobject_cast<QDockWidget*>( o );

	// if it s a dock widget
	if ( d )
	{
		if ( t == QEvent::Show || t == QEvent::Hide )
		{
			// if exclusive, hide all except this one
			if ( t == QEvent::Show && aToggleExclusive->isChecked() )
			{
				foreach ( QDockWidget* dw, docks() )
				{
					if ( dw != d && dw->isVisible() )
						dw->hide();
				}
			}

			// get dock button
			QAbstractButton* b = button( d );

			// check the dock button
			b->setChecked( t == QEvent::Show );

			// check button text
			internal_checkButtonText( b );

			// check toolbar visibility
			internal_checkVisibility();
		}
		else if ( t == QEvent::KeyPress )
		{
			if ( static_cast<QKeyEvent*>( e )->key() == Qt::Key_Escape )
				d->hide();
		}
	}

	// deturn default event filter
	return QToolBar::eventFilter( o, e );
}

void pDockToolBar::addAction( QAction* a, bool b )
{
	// create a separator if no action
	if ( !a )
	{
		a = new QAction( this );
		a->setSeparator( true );
	}

	if ( b )
		QToolBar::insertAction( aDockFrame, a );
	else
		QToolBar::addAction( a );

	internal_checkVisibility();
}

void pDockToolBar::addActions( QList<QAction*> a, bool b )
{
	if ( b )
		QToolBar::insertActions( aDockFrame, a );
	else
		QToolBar::addActions( a );

	internal_checkVisibility();
}

int pDockToolBar::addDock( QDockWidget* d, const QString& s, const QIcon& i )
{	
	// cancel if no dock or dock already managed
	if ( !d || id( d ) != -1 )
		return -1;

	// check if this dock is already in another bar, and remove it
	pDockToolBar* tb = mManager->bar( mManager->dockWidgetAreaToToolBarArea( mManager->mainWindow()->dockWidgetArea( d ) ) );
	if ( tb && tb->id( d ) != -1 )
		tb->removeDock( d );

	// set dock title
	if ( !s.isEmpty() )
		d->setWindowTitle( s );

	// set object name if needed
	if ( d->objectName().isEmpty() )
		d->setObjectName( QString( "QDockWidget_%1" ).arg( s ).replace( " ", "_" ).trimmed() );

	// set dock icon
	if ( !i.isNull() )
		d->setWindowIcon( i );

	// create button
	pTabbedWorkspaceCornerButton* pb = new pTabbedWorkspaceCornerButton( this, mManager->toolBarAreaToBoxLayoutDirection( mManager->mainWindow()->toolBarArea( this ) ) );
	pb->setCheckable( true );
	pb->setFont( font() );
	pb->setText( d->windowTitle() );
	pb->setToolTip( pb->text() );
	pb->setProperty( "Caption", pb->text() );
	pb->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	pb->setIconSize( iconSize() );
	pb->setIcon( i );

	// add button to layout
	mLayout->addWidget( pb );

	// add dock to correct area
	Qt::DockWidgetArea da = mManager->mainWindow()->dockWidgetArea( d );
	Qt::DockWidgetArea ta = mManager->toolBarAreaToDockWidgetArea( mManager->mainWindow()->toolBarArea( this ) );
	if ( da != ta )
		mManager->mainWindow()->addDockWidget( ta, d, orientation() );

	// if exclusive, hide all dock except this one
	if ( aToggleExclusive->isChecked() && count() )
	{
		foreach ( QDockWidget* dw, docks() )
		{
			if ( dw->isVisible() )
				dw->hide();
		}
	}

	// check button according to dock visibility
	pb->setChecked( d->isVisible() );

	// check button text
	internal_checkButtonText( pb );

	// add dock/button to list
	mButtons[mUniqueId] = pb;
	mDocks[mUniqueId] = d;

	// filter the dock
	d->installEventFilter( this );

	// connect
	connect( d->toggleViewAction(), SIGNAL( changed() ), this, SLOT( internal_dockChanged() ) );
	connect( d, SIGNAL( destroyed( QObject* ) ), this, SLOT( internal_dockDestroyed( QObject* ) ) );
	connect( pb, SIGNAL( clicked( bool ) ), this, SLOT( internal_buttonClicked( bool ) ) );

	// check if we need to hide/show the toolbar
	internal_checkVisibility();

	// return unique auto increment id of the dock
	return mUniqueId++;
}

void pDockToolBar::removeDock( int i )
{
	removeDock( dock( i ) );
}

void pDockToolBar::removeDock( QDockWidget* d )
{
	// get dock id
	int i = id( d );

	// cancel if dock is not acutally own by this toolbar
	if ( i == -1 )
		return;
	
	// remove filter event
	d->removeEventFilter( this );

	// disconnect
	disconnect( d->toggleViewAction(), SIGNAL( changed() ), this, SLOT( internal_dockChanged() ) );
	disconnect( d, SIGNAL( destroyed( QObject* ) ), this, SLOT( internal_dockDestroyed( QObject* ) ) );

	// delete button
	QAbstractButton* b = button( d );
	mButtons.remove( i );
	b->deleteLater();

	// remove dock from list
	mDocks.remove( i );

	// check if we need to hide/show the toolbar
	internal_checkVisibility();
}

bool pDockToolBar::isDockVisible( int i ) const
{
	return isDockVisible( dock( i ) );
}

bool pDockToolBar::isDockVisible( QDockWidget* d ) const
{
	// if dock is in internal list
	if ( id( d ) != -1 )
		return button( d )->isChecked();

	// else return widget visibility
	return d ? d->isVisible() : false;
}

void pDockToolBar::setDockVisible( QDockWidget* d, bool b )
{
	d->setVisible( b );
}

bool pDockToolBar::exclusive() const
{
	return aToggleExclusive->isChecked();
}

void pDockToolBar::setExclusive( bool b )
{
	if ( aToggleExclusive->isChecked() == b )
		return;
	aToggleExclusive->setChecked( b );

	// if exclusive, hide all
	if ( aToggleExclusive->isChecked() && count() )
	{
		foreach ( QDockWidget* dw, docks() )
		{
			if ( dw->isVisible() )
				dw->hide();
		}
	}
}

int pDockToolBar::id( QDockWidget* d ) const
{
	return mDocks.values().contains( d ) ? mDocks.key( d ) : -1;
}

int pDockToolBar::id( QAbstractButton* b ) const
{
	return mButtons.values().contains( b ) ? mButtons.key( b ) : -1;
}

QDockWidget* pDockToolBar::dock( int i ) const
{
	return mDocks.value( i );
}

QDockWidget* pDockToolBar::dock( QAbstractButton* b ) const
{
	return dock( id( b ) );
}

QAbstractButton* pDockToolBar::button( int i ) const
{
	return mButtons.value( i );
}

QAbstractButton* pDockToolBar::button( QDockWidget* d ) const
{
	return button( id( d ) );
}

QList<QDockWidget*> pDockToolBar::docks() const
{
	return mDocks.values();
}

QList<QAbstractButton*> pDockToolBar::buttons() const
{
	return mButtons.values();
}

int pDockToolBar::count() const
{
	return docks().count();
}

QAction* pDockToolBar::toggleExclusiveAction() const 
{
	// set action text
	aToggleExclusive->setText( tr( "%1 - Exclusive" ).arg( windowTitle() ) );
	return aToggleExclusive;
}

void pDockToolBar::internal_checkVisibility()
{
	// count toolbar actions, if 1 it s dockframe
	int i = actions().count();

	// need show ?!
	if ( !isVisible() && ( i > 1 || ( i == 1 && count() ) ) )
		show();
	// need hide ?!
	else if ( isVisible() && ( i == 1 && !count() ) )
		hide();
}

void pDockToolBar::internal_checkButtonText( QAbstractButton* b )
{
	// cancel if no button
	if ( !b )
		return;
/*
	// show text when checked, else not
	if ( b->isChecked() && b->text().isEmpty() )
		b->setText( b->property( "Caption" ).toString() );
	else if ( !b->isChecked() && !b->text().isEmpty() )
		b->setText( QString() );
*/
}

void pDockToolBar::internal_orientationChanged( Qt::Orientation o )
{
	// change layout direction
	switch ( o )
	{
		case Qt::Horizontal:
			mLayout->setDirection( QBoxLayout::LeftToRight );
			break;
		case Qt::Vertical:
			mLayout->setDirection( QBoxLayout::TopToBottom );
			break;
	}

	// layout docks
	foreach ( QDockWidget* d, mDocks )
		mManager->mainWindow()->addDockWidget( mManager->mainWindow()->dockWidgetArea( d ), d, o );
}

void pDockToolBar::internal_dockChanged()
{
	// get dock action
	QAction* a = qobject_cast<QAction*>( sender() );

	// get dock
	QDockWidget* d = qobject_cast<QDockWidget*>( a->parent() );

	// no position changed, return
	if ( !d || d->isFloating() || pDockToolBarManager::dockWidgetAreaToToolBarArea( mManager->mainWindow()->dockWidgetArea( d ) ) == mManager->mainWindow()->toolBarArea( this ) )
		return;
	else
		emit dockWidgetAreaChanged( d, this );
}

void pDockToolBar::internal_dockDestroyed( QObject* o )
{
	QDockWidget* d = reinterpret_cast<QDockWidget*>( o ); // qobject_cast<QDockWidget*>( o ); qobject_cast don't work with QDockWidget in destroyed emits :|
	
	// get dock id
	int i = id( d );

	// cancel if dock is not acutally own by this toolbar
	if ( i == -1 )
		return;
	
	// remove filter event
	d->removeEventFilter( this );

	// delete button
	QAbstractButton* b = button( d );
	mButtons.remove( i );
	b->deleteLater();

	// remove dock from list
	mDocks.remove( i );

	// check if we need to hide/show the toolbar
	internal_checkVisibility();
}

void pDockToolBar::internal_buttonClicked( bool b )
{
	// get button
	QAbstractButton* ab = qobject_cast<QAbstractButton*>( sender() );

	// get dock
	QDockWidget* d = dock( ab );

	// return if no dock
	if ( !d )
		return;

	if ( aToggleExclusive->isChecked() )
	{
		foreach ( QDockWidget* dw, docks() )
		{
			if ( dw != d && dw->isVisible() )
				dw->hide();
		}
	}

	// update button text
	internal_checkButtonText( ab );

	// show/hide dock according to b
	if ( d->isVisible() != b )
		d->setVisible( b );

	// emit button clicked
	emit buttonClicked( id( dock( ab ) ) );
}
