#include "UITemplatesWizard.h"
#include "pFileManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"

#include "ProjectsModel.h"
#include "ProjectsProxy.h"
#include "UIProjectsManager.h"
#include "VariablesManager.h"

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
	
	QStringList l( tr( "All" ) );
	QStringList t( tr( "All" ) );
	foreach( pTemplate tp, mTemplates )
	{
		if ( !l.contains( tp.Language, Qt::CaseInsensitive ) )
			l << tp.Language;
		if ( !t.contains( tp.Type, Qt::CaseInsensitive ) )
			t << tp.Type;
	}
	
	// add items
	cbLanguages->addItems( l );
	cbTypes->addItems( t );
	cbOperators->addItems( availableOperators() );
	
    // assign projects combobox
    mProjects = UIProjectsManager::instance()->model();
    cbProjects->setModel( mProjects->scopesProxy() );
    ProjectItem* p = UIProjectsManager::instance()->currentProject();
    cbProjects->setCurrentIndex( mProjects->scopesProxy()->mapFromSource( p ? p->index() : QModelIndex() ) );
	
    // restore infos
    pSettings s;
    cbLanguages->setCurrentIndex( cbLanguages->findText( s.value( "Recents/FileWizard/Language", "C++" ).toString() ) );
    leDestination->setText( s.value( "Recents/FileWizard/Destination" ).toString() );
    cbOpen->setChecked( s.value( "Recents/FileWizard/Open", true ).toBool() );
	
	// connections
	connect( cbLanguages, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onFiltersChanged() ) );
	connect( cbTypes, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onFiltersChanged() ) );
	
	// init template
	onFiltersChanged();
}

void UITemplatesWizard::setType( const QString& s )
{ cbTypes->setCurrentIndex( cbTypes->findText( s ) ); }

void UITemplatesWizard::onFiltersChanged()
{
	// get combobox text
	QString t = cbTypes->currentText();
	QString l = cbLanguages->currentText();
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
	
	// don t open project, because adding it to a parent will auto matically open it
	if ( !t.FilesToAdd.isEmpty() )
		t.ProjectsToOpen.clear();
	
	// get proejct to add
	ProjectItem* si = t.FilesToAdd.isEmpty() ? 0 : mProjects->itemFromIndex( mProjects->scopesProxy()->mapToSource( cbProjects->currentIndex() ) );
	
	// process templates
	if ( !pTemplatesManager::instance()->realiseTemplate( si, cbOperators->currentText(), t, v ) )
		return;
	
    // remember some infos
    pSettings s;
    s.setValue( "Recents/FileWizard/Language", cbLanguages->currentText() );
    s.setValue( "Recents/FileWizard/Destination", leDestination->text() );
    s.setValue( "Recents/FileWizard/Open", cbOpen->isChecked() );
	
    // close dialog
    QDialog::accept();	
}
