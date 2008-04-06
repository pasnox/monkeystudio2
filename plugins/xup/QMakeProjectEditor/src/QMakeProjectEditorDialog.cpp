#include "QMakeProjectEditorDialog.h"

#include <coremanager.h>
#include <pluginsmanager.h>


QMakeProjectEditorDialog::QMakeProjectEditorDialog( XUPItem* project, QWidget* parent )
	: UIXUPProjectEditor( project, parent )
{
	// init Qt configuration widget
	mQtConfigurationPage = new QWidget( this );
	mUiQtConfiguration.setupUi( mQtConfigurationPage );
	// insert qt conf page into project settings editor
	insertPage( 0, mQtConfigurationPage );
	cbPages->setCurrentIndex( 0 );
	// load qt version
	loadsQtVersions();
	// connections
	connect( mUiQtConfiguration.lwQtVersions, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lwQtVersions_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( mUiQtConfiguration.lQtVersionInformations, SIGNAL( linkActivated( const QString& ) ), this, SLOT( lQtVersionInformations_linkActivated( const QString& ) ) );
}

void QMakeProjectEditorDialog::loadsQtVersions()
{
	mUiQtConfiguration.lwQtVersions->clear();
	foreach ( const QtVersion& v, mQtManager.versions() )
	{
		QListWidgetItem* it = new QListWidgetItem( v.Version, mUiQtConfiguration.lwQtVersions );
		it->setData( Qt::UserRole, QVariant::fromValue( v ) );
		if ( v.Default )
			it->setBackground( QBrush( Qt::green ) );
	}
	
#warning create a projectsettingsValue() / setProjectSettingsValue member so we abstract the call for stocking/reading specific settings of project in project
}

void QMakeProjectEditorDialog::lwQtVersions_currentItemChanged( QListWidgetItem* current, QListWidgetItem* )
{
	if ( current )
	{
		QtVersion v = current->data( Qt::UserRole ).value<QtVersion>();
		mUiQtConfiguration.leQtVersion->setText( v.Version );
		mUiQtConfiguration.leQtPath->setText( v.Path );
		mUiQtConfiguration.leQtQMakeSpec->setText( v.QMakeSpec );
		mUiQtConfiguration.leQtQMakeParameters->setText( v.QMakeParameters );
	}
}

void QMakeProjectEditorDialog::lQtVersionInformations_linkActivated( const QString& link )
{
	if ( link == "plugin_config" )
	{
		XUPPlugin* xp = MonkeyCore::pluginsManager()->plugin<XUPPlugin*>( PluginsManager::stAll, mProject->projectSettingsValue( "EDITOR" ) );
		if ( xp )
		{
			QWidget* cd = xp->settingsWidget();
			cd->setAttribute( Qt::WA_DeleteOnClose );
			cd->setWindowFlags( Qt::Dialog );
			cd->setWindowModality( Qt::ApplicationModal );
			connect( cd->findChild<QDialogButtonBox*>()->button( QDialogButtonBox::Save ), SIGNAL( clicked() ), cd, SLOT( close() ) );
			connect( cd, SIGNAL( destroyed( QObject* ) ), this, SLOT( loadsQtVersions() ) );
			cd->show();
		}
	}
}
