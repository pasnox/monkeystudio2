/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIAddExistingFiles.cpp
** Date      : 2007-11-04T22:45:48
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
