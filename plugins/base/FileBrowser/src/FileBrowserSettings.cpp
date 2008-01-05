/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : FileBrouserSettings.cpp
** Date      : 2007-11-04T22:50:45
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "FileBrowserSettings.h"
#include "pStringListEditor.h"
#include "pDockFileBrowser.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

FileBrowserSettings::FileBrowserSettings()
{
	// window attribute
	setAttribute( Qt::WA_DeleteOnClose );
	
	// list editor
	mEditor = new pStringListEditor( this, tr( "Except Suffixes" ) );
	mEditor->setValues( pDockFileBrowser::instance()->wildcards() );
	
	// apply button
	QPushButton* applyBtn = new QPushButton( tr( "Apply" ), this );
	
	// button layout
	QHBoxLayout* applyBox = new QHBoxLayout;
	applyBox->addWidget( applyBtn, 0, Qt::AlignRight );
	
	// global layout
	QVBoxLayout* vbox = new QVBoxLayout( this );
	vbox->setMargin( 5 );
	vbox->setSpacing( 3 );
	vbox->addWidget( mEditor );
	vbox->addLayout( applyBox );
	
	// connections
	connect ( applyBtn, SIGNAL ( clicked() ), this, SLOT ( setSettings() ) );
}

void FileBrowserSettings::setSettings()
{ pDockFileBrowser::instance()->setWildcards( mEditor->values() ); }
