#include "UITemplatesWizard.h"
#include "pTemplatePreviewer.h"
#include "pEditor.h"
#include "pFileManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"

#include "ProjectsModel.h"
#include "ProjectsProxy.h"
#include "UIProjectsManager.h"

#include <QFileInfo>
#include <QDir>
#include <QDateTime>

using namespace pMonkeyStudio;

UITemplatesWizard::UITemplatesWizard( QWidget* w )
	: QDialog( w )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	// fill available languages
	cbLanguages->addItems( availableLanguages() );
	// fill type comboobox
	pTemplate::fillComboBox( cbTypes );
	// assign projects combobox
	mProjects = UIProjectsManager::instance()->model();
	cbProjects->setModel( mProjects->scopesProxy() );
	ProjectItem* p = UIProjectsManager::instance()->currentProject();
	cbProjects->setCurrentIndex( mProjects->scopesProxy()->mapFromSource( p ? p->index() : QModelIndex() ) );
	// show correct page
	on_swPages_currentChanged( 0 );
	// restore infos
	pSettings* s = pSettings::instance();
	cbLanguages->setCurrentIndex( cbLanguages->findText( s->value( "Recents/FileWizard/Language", "C++" ).toString() ) );
	leDestination->setText( s->value( "Recents/FileWizard/Destination" ).toString() );
	leAuthor->setText( s->value( "Recents/FileWizard/Author" ).toString() );
	cbLicenses->setEditText( s->value( "Recents/FileWizard/License", "GPL" ).toString() );
	cbOpen->setChecked( s->value( "Recents/FileWizard/Open", true ).toBool() );
	//
	qWarning( "UITemplatesWizard::UITemplatesWizard( QWidget* w ) make getting operators options" );
	cbOperators->addItems( QStringList() << "=" << "+=" << "-=" << "*=" << "~=" );
}

void UITemplatesWizard::setType( pTemplate::TemplateType t )
{ cbTypes->setCurrentIndex( cbTypes->findData( t ) ); }

void UITemplatesWizard::on_cbLanguages_currentIndexChanged( const QString& s )
{
	// clear lwTemplates
	lwTemplates->clear();
	// create blank file
	QListWidgetItem* it = new QListWidgetItem( lwTemplates );
	it->setIcon( QIcon( ":/templates/icons/templates/empty.png" ) );
	it->setToolTip( tr( "Blank File" ) );
	it->setText( tr( "Blank" ) );
	it->setData( Qt::UserRole +1, QString() );
	// create tempaltes
	foreach ( pTemplate t, pTemplatesManager::availableTemplates() )
	{
		if ( t.Language == s && pTemplate::stringForType( t.Type ) == cbTypes->currentText() )
		{
			it = new QListWidgetItem( lwTemplates );
			it->setIcon( QIcon( t.Icon ) );
			it->setToolTip( t.Description );
			it->setText( t.Name );
			it->setData( Qt::UserRole +1, t.FileNames );
		}
	}
}

void UITemplatesWizard::on_cbTypes_currentIndexChanged( const QString& )
{ on_cbLanguages_currentIndexChanged( cbLanguages->currentText() ); }

void UITemplatesWizard::on_swPages_currentChanged( int i )
{
	switch ( i )
	{
		case 0:
			lInformations->setText( tr( "Templates" ) );
			break;
		case 1:
			lInformations->setText( tr( "Templates Preview" ) );
			generatePreview();
			break;
	}
	// set correct text/state to buttons
	pbPrevious->setEnabled( i != 0 );
	pbNext->setText( tr( "Next" ) );
	if ( swPages->currentIndex() == swPages->count() -1 )
		pbNext->setText( tr( "Finish" ) );
}

void UITemplatesWizard::on_tbDestination_clicked()
{
	QString s = getExistingDirectory( tr( "Select the file(s) destination" ), leDestination->text(), window() );
	if ( !s.isNull() )
		leDestination->setText( s );
}

void UITemplatesWizard::on_pbPrevious_clicked()
{ swPages->setCurrentIndex( swPages->currentIndex() -1 ); }

void UITemplatesWizard::on_pbNext_clicked()
{
	if ( !checkTemplates() )
		return;
	
	int i = swPages->currentIndex();
	
	if ( i < swPages->count() -1 )
		swPages->setCurrentIndex( i +1 );
	
	if ( i == swPages->count() -1 )
		accept();
}

bool UITemplatesWizard::checkTemplates()
{
	if ( !lwTemplates->selectedItems().count() )
	{
		information( tr( "Templates..." ), tr( "Choose a template to continue." ), this );
		return false;
	}
	
	if ( leBaseName->text().isEmpty() )
	{
		information( tr( "Base Name..." ), tr( "Choose a base name for your file(s)." ), this );
		return false;
	}
	
	if ( leDestination->text().isEmpty() )
	{
		information( tr( "Destination..." ), tr( "Choose a destination for your file(s)." ), this );
		return false;
	}
	
	ProjectItem* it = mProjects->itemFromIndex( mProjects->scopesProxy()->mapToSource( cbProjects->currentIndex() ) );
	
	if ( cbAddToProject->isChecked() )
	{
		if ( !it )
		{
			information( tr( "Add To Project..." ), tr( "You need to select a project to add to." ), this );
			return false;
		}
		else if ( cbTypes->itemData( cbTypes->currentIndex() ).toInt() == pTemplate::ttProjects && !it->isProjectsContainer() )
		{
			information( tr( "Add To Project..." ), tr( "The project you select is not a projects container." ), this );
			return false;
		}
	}
	
	return true;
}

void UITemplatesWizard::generatePreview()
{
	// delete all existing preview
	qDeleteAll( sView->findChildren<pTemplatePreviewer*>() );
	//
	int i = 0;
	// create new preview
	foreach ( QListWidgetItem* it, lwTemplates->selectedItems() )
	{
		// get template files
		foreach ( QString s, it->data( Qt::UserRole +1 ).toStringList() )
		{
			pTemplatePreviewer* p = new pTemplatePreviewer;
			if ( s.isEmpty() )
			{
				p->setFileName( leBaseName->text() );
				if ( it->text() == tr( "Blank" ) )
					p->setOpen( true );
			}
			else
			{
				p->setFileName( leBaseName->text().append( QString( ".%1" ).arg( QFileInfo( s ).suffix() ) ) );
				p->open( s );
				if ( cbTypes->itemData( cbTypes->currentIndex() ).toInt() != pTemplate::ttProjects || ( i != 0 && cbTypes->itemData( cbTypes->currentIndex() ).toInt() == pTemplate::ttProjects ) )
					p->editor()->insertAt( pTemplatesManager::templatesHeader(), 0, 0 );
			}
			p->setDestination( leDestination->text() );
			// process content parsing
			pTemplateContent tc;
			tc.Name = leBaseName->text();
			tc.Author = leAuthor->text();
			tc.License = cbLicenses->currentText();
			tc.Project = mProjects->itemFromIndex( mProjects->scopesProxy()->mapToSource( cbProjects->currentIndex() ) );
			if ( !cbAddToProject->isChecked() )
				tc.Project = 0;
			tc.FileName = p->fileName();
			tc.Comment = tr( "Your comment here" );
			tc.Content = p->editor()->text();
			// set content
			p->editor()->setText( tc.Content.isEmpty() ? "\n" : pTemplatesManager::processContent( tc ) );
			// add widget to splitter
			sView->addWidget( p );
		}
		// increase i
		i++;
	}
}

void UITemplatesWizard::accept()
{
	// get current template type
	pTemplate::TemplateType t = (pTemplate::TemplateType)cbTypes->itemData( cbTypes->currentIndex() ).toInt();
	// get project item
	ProjectItem* it = mProjects->itemFromIndex( mProjects->scopesProxy()->mapToSource( cbProjects->currentIndex() ) );
	//
	int i = 0;
	// create files
	foreach ( pTemplatePreviewer* p, sView->findChildren<pTemplatePreviewer*>() )
	{
		// if open
		if ( p->open() )
		{
			// set filename
			QString s = QDir::cleanPath( p->destination().append( "/" ).append( p->fileName() ) );
			// check if file already existing
			if ( QFile::exists( s ) && !question( tr( "Overwrite File..." ), tr( "The file '%1' already exists, do you want to continue ?" ).arg( p->fileName() ) ) )
				continue;
			// if can save
			if ( p->editor()->saveFile( s ) )
			{
				if ( cbAddToProject->isChecked() )
				{
					if ( t == pTemplate::ttProjects && i == 0 )
					{
						if ( !it->addProject( s ) )
							warning( "Adding Project...", tr( "Can't add project:\n%1" ).arg( s ) );
					}
					else if ( t != pTemplate::ttProjects )
					{
						it->project()->addExistingFile( s, it, cbOperators->currentText() );
						if ( cbOpen->isChecked() )
							pFileManager::instance()->openFile( s );
					}
				}
				else if ( cbOpen->isChecked() )
				{
					if ( t == pTemplate::ttProjects && i == 0 )
						pFileManager::instance()->openProject( s );
					else
						pFileManager::instance()->openFile( s );
				}
			}
			else
			{
				warning( "Create File...", tr( "Can't create file:\n%1\nAborting." ).arg( s ) );
				return;
			}
		}
		// increase i
		i++;
	}
	// remember some infos
	pSettings* s = pSettings::instance();
	s->setValue( "Recents/FileWizard/Language", cbLanguages->currentText() );
	s->setValue( "Recents/FileWizard/Destination", leDestination->text() );
	s->setValue( "Recents/FileWizard/Author", leAuthor->text() );
	s->setValue( "Recents/FileWizard/License", cbLicenses->currentText() );
	s->setValue( "Recents/FileWizard/Open", cbOpen->isChecked() );
	// close dialog
	QDialog::accept();
}
