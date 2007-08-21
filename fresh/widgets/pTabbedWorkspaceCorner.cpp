/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pTabbedWorkspaceCorner.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class provide a QWidget that can be put as corner in a pTabbedWorkspace
 ********************************************************************************************************/
#include "pTabbedWorkspaceCorner.h"
#include "pTabbedWorkspaceCornerButton.h"
#include "pTabbedWorkspace.h"

#include <QPaintEvent>
#include <QPainter>
#include <QStyle>

#include <QDebug>

pTabbedWorkspaceCorner::pTabbedWorkspaceCorner( pTabbedWorkspace* w )
	: QWidget( w ), mWorkspace( w ), mLayout( new QBoxLayout( QBoxLayout::LeftToRight, this ) )
{
	/*
	QBoxLayout::LeftToRight
	QBoxLayout::RightToLeft
	QBoxLayout::TopToBottom
	QBoxLayout::BottomToTop
	*/

	mLayout->setSpacing( 3 );
	mLayout->setMargin( 0 );
}

pTabbedWorkspaceCorner::~pTabbedWorkspaceCorner()
{
	clearActions();
}

QBoxLayout::Direction pTabbedWorkspaceCorner::direction() const
{
	return mLayout->direction();
}

void pTabbedWorkspaceCorner::clearActions()
{
	qDeleteAll( findChildren<pTabbedWorkspaceCornerButton*>() );
}

void pTabbedWorkspaceCorner::setDirection( QBoxLayout::Direction d )
{
	// change buttons direction
	foreach ( pTabbedWorkspaceCornerButton* tb, findChildren<pTabbedWorkspaceCornerButton*>() )
		tb->setDirection( d );

	// change layout direction
	mLayout->setDirection( d );
}

void pTabbedWorkspaceCorner::addAction( QAction* a )
{
	// create button
	pTabbedWorkspaceCornerButton* tb = new pTabbedWorkspaceCornerButton( this );
	tb->setDefaultAction( a );
	tb->setDirection( direction() );

	// add to layout
	mLayout->addWidget( tb );
}

void pTabbedWorkspaceCorner::setActions( QList<QAction*> l )
{
	// hide to avoid flickering
	hide();

	// delete all buttons
	clearActions();

	// create button associated with actions
	foreach ( QAction* a, l )
		addAction( a );
	
	// show corner
	show();
}
