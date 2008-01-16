/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIAddExistingFiles.cpp
** Date      : 2008-01-14T00:37:01
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
#include "UIAddExistingFiles.h"

#include <QGridLayout>
#include <QFrame>
#include <QHBoxLayout>

UIAddExistingFiles::UIAddExistingFiles( ProjectsProxy* p, ProjectItem* it, const QStringList& l, QWidget* w )
	: QFileDialog( w, tr( "Choose file(s)/project(s) to add to your project" ), it->canonicalPath() ), mProxy( p ), mItem( it )
{
	Q_ASSERT( mProxy );
	Q_ASSERT( mItem );
	// set file/accept mode
	setAcceptMode( QFileDialog::AcceptOpen );
	setFileMode( QFileDialog::ExistingFiles );
	// create frame for comboboxes
	QFrame* f = new QFrame;
	// add layout to frame
	QHBoxLayout* hl = new QHBoxLayout( f );
	hl->setMargin( 0 );
	hl->setSpacing( 3 );
	// add projects/scopes groupbox
	QGroupBox* gb1 = new QGroupBox;
	gb1->setTitle( tr( "Scopes" ) );
	// set groupbox layout
	QHBoxLayout* hl1 = new QHBoxLayout( gb1 );
	hl1->setMargin( 5 );
	hl1->setSpacing( 0 );
	// create projects/scopes treecombobox
	tcbProjects = new pTreeComboBox;
	tcbProjects->setModel( mProxy );
	tcbProjects->setCurrentIndex( mProxy->mapFromSource( mItem->index() ) );
	hl1->addWidget( tcbProjects );
	// add operators groupbox
	QGroupBox* gb2 = new QGroupBox;
	gb2->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	gb2->setTitle( tr( "Operators" ) );
	// set groupbox layout
	QHBoxLayout* hl2 = new QHBoxLayout( gb2 );
	hl2->setMargin( 5 );
	hl2->setSpacing( 0 );
	// create operators combobox
	cbOperators = new QComboBox;
	cbOperators->addItems( l );
	hl2->addWidget( cbOperators );
	// add groupbox to layout
	hl->addWidget( gb1 );
	hl->addWidget( gb2 );
	// add frame to layout
	qobject_cast<QGridLayout*>( layout() )->addWidget( f, 4, 0, 1, 3 );
}
