/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UITemplatesWizard.cpp
** Date      : 2008-01-14T00:37:11
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
#include "UITemplatesWizard.h"
#include "../../workspace/pFileManager.h"
#include "../../pMonkeyStudio.h"
#include "../../coremanager/MonkeyCore.h"
#include "../../settingsmanager/Settings.h"

#warning uncomment include
/*
#include "../../xupmanager/ProjectItemModel.h"
#include "../../xupmanager/ScopedProjectItemModel.h"
*/
#include "../../xupmanager/core/XUPItem.h"
#include "../../xupmanager/core/XUPProjectItem.h"
#include "../../xupmanager/gui/XUPProjectManager.h"
#include "../../variablesmanager/VariablesManager.h"

#include <QDir>

using namespace pMonkeyStudio;

UITemplatesWizard::UITemplatesWizard( QWidget* w )
	: QDialog( w )
{
	// init dialog
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	saWidgets->setWidgetResizable( true );
	
	// create scrollarea widget
	QWidget* cw = new QWidget;
	gridLayout = new QGridLayout( cw );
	gridLayout->setMargin( 5 );
	gridLayout->setSpacing( 3 );
	saWidgets->setWidget( cw );
	
	// get templates
	mTemplates = pTemplatesManager::instance()->getTemplates();
	
	// set default language/type
	cbLanguages->addItem( tr( "All" ), "All" );
	cbTypes->addItem( tr( "All" ), "All" );
	
	// languages/types
	foreach( pTemplate tp, mTemplates )
	{
		if ( cbLanguages->findData( tp.Language, Qt::UserRole, Qt::MatchFixedString ) == -1 )
			cbLanguages->addItem( tr( qPrintable( tp.Language ) ), tp.Language );
		if ( cbTypes->findData( tp.Type, Qt::UserRole, Qt::MatchFixedString ) == -1 )
			cbTypes->addItem( tr( qPrintable( tp.Type ) ), tp.Type );
	}

	// assign projects combobox
#warning uncomment UITemplatesWizard constructor
	/*
	mProjects = MonkeyCore::projectsManager()->model();
	cbProjects->setModel( mProjects->scopedModel() );
	XUPItem* p = MonkeyCore::projectsManager()->currentProject();
	cbProjects->setCurrentIndex( mProjects->scopedModel()->mapFromSource( p ? p->index() : QModelIndex() ) );
	*/

	// restore infos
	pSettings* s = MonkeyCore::settings();
	cbLanguages->setCurrentIndex( cbLanguages->findText( s->value( "Recents/FileWizard/Language", "C++" ).toString() ) );
	leDestination->setText( s->value( "Recents/FileWizard/Destination" ).toString() );
	cbOpen->setChecked( s->value( "Recents/FileWizard/Open", true ).toBool() );
	
	// connections
	connect( cbLanguages, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onFiltersChanged() ) );
	connect( cbTypes, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onFiltersChanged() ) );
	
	// init template
	onFiltersChanged();
}

void UITemplatesWizard::setType( const QString& s )
{ cbTypes->setCurrentIndex( cbTypes->findData( s ) ); }

void UITemplatesWizard::onFiltersChanged()
{
	// get combobox text
	QString t = cbTypes->itemData( cbTypes->currentIndex() ).toString();
	QString l = cbLanguages->itemData( cbLanguages->currentIndex() ).toString();
	QString i;

	// clear lwTemplates
	lwTemplates->clear();

	// create templates items
	foreach ( pTemplate tp, mTemplates )
	{
		if (  ( l == "All" || tp.Language == l || tp.Language == "All" ) &&
			( t == "All" || tp.Type == t || tp.Type == "All" ) )
		{
			QListWidgetItem* it = new QListWidgetItem( lwTemplates );
			i = ":/templates/icons/templates/empty.png";
			if ( !tp.Icon.isEmpty() )
			{
				if ( QFile::exists( tp.DirPath +tp.Icon ) )
					i = tp.DirPath +tp.Icon;
				else if ( QFile::exists( ":/templates/icons/templates/" +tp.Icon ) )
					i = ":/templates/icons/templates/" +tp.Icon;
			}
			it->setIcon( QIcon( i ) );
			it->setToolTip( tp.Description );
			it->setText( tp.Name );
			it->setData( Qt::UserRole +1, mTemplates.indexOf( tp ) );
		}
	}

	// disable groupbox
	lInformations->setText( "No Template Selected" );
	gbInformations->setEnabled( false );
}

void UITemplatesWizard::on_lwTemplates_itemPressed( QListWidgetItem* it )
{
	// delete labels & combos
	qDeleteAll( mLabels );
	mLabels.clear();
	qDeleteAll( mCombos );
	mCombos.clear();
	
	// get template
	pTemplate t = mTemplates.value( it->data( Qt::UserRole +1 ).toInt() );
	int r = 1;
	
	// set template informations
	lInformations->setText( t.Description );
	
	// create labels/combos
	foreach( QString v, t.Variables.keys() )
	{
		QComboBox* c = new QComboBox();
		c->setEditable( true );
		c->addItems( t.Variables.value( v ) );
		
		QLabel* l = new QLabel( v +" :" );
		l->setToolTip( v );
		l->setBuddy( c );
		
		gridLayout->addWidget( l, r, 0 );
		mLabels << l;
		
		gridLayout->addWidget( c, r++, 1 );
		mCombos << c;
	}
	
	//saWidgets->resize( saWidgets->widget()->sizeHint() );
	
	// enable groupbox
	gbInformations->setEnabled( true );
}

void UITemplatesWizard::on_cbProjects_currentChanged( const QModelIndex& index )
{
#warning uncomment UITemplatesWizard::on_cbProjects_currentChanged
/*
	const QModelIndex idx = mProjects->scopedModel()->mapToSource( index );
	if ( XUPProjectItem* it = dynamic_cast<XUPProjectItem*> (mProjects->itemFromIndex( idx )) )
	{
		cbOperators->clear();
		cbOperators->addItems( it->operators() );
		leDestination->setText( it->project()->projectPath() );
	}
*/
}

void UITemplatesWizard::on_tbDestination_clicked()
{
	QString s = getExistingDirectory( tr( "Select the file(s) destination" ), leDestination->text(), window() );
	if ( !s.isNull() )
		leDestination->setText( s );
}

bool UITemplatesWizard::checkTemplate()
{
	// check item available
	if ( !lwTemplates->selectedItems().count() )
	{
		information( tr( "Template..." ), tr( "You need to select a template." ), window() );
		return false;
	}
	
	// return default value
	return true;
}

void UITemplatesWizard::on_pbCreate_clicked()
{
	// check if we can go later
	if ( !checkTemplate() )
		return;
	
	// get variables
	VariablesManager::Dictionary v;
	v[ "Destination" ] = leDestination->text();
	
	// iterate each labels
	foreach ( QLabel* l, mLabels )
		v[l->toolTip()] = qobject_cast<QComboBox*>( l->buddy() )->currentText();
	
	// get template
	pTemplate t = mTemplates.value( lwTemplates->selectedItems().value( 0 )->data( Qt::UserRole +1 ).toInt() );
	
	// check if need open files
	if ( !cbOpen->isChecked() )
	{
		t.FilesToOpen.clear();
		t.ProjectsToOpen.clear();
	}
	
	// check if need add files
	if ( !gbAddToProject->isChecked() || !cbProjects->currentIndex().isValid() )
		t.FilesToAdd.clear();
	
	// don t open project, because adding it to a parent will automatically open it
	if ( !t.FilesToAdd.isEmpty() )
		t.ProjectsToOpen.clear();
	
	// get proejct to add
#warning uncomment item creation
	XUPItem* si = 0;//t.FilesToAdd.isEmpty() ? 0 : mProjects->itemFromIndex( mProjects->scopedModel()->mapToSource( cbProjects->currentIndex() ) );
	
	// process templates
	if ( !pTemplatesManager::instance()->realiseTemplate( si, cbOperators->currentText(), t, v ) )
		return;
	
	// remember some infos
	pSettings* s = MonkeyCore::settings();
	s->setValue( "Recents/FileWizard/Language", cbLanguages->currentText() );
	s->setValue( "Recents/FileWizard/Destination", leDestination->text() );
	s->setValue( "Recents/FileWizard/Open", cbOpen->isChecked() );
	
	// close dialog
	QDialog::accept();	
}
