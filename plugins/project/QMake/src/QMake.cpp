#include "QMake.h"
#include "QMakeItem.h"
#include "UIProjectsManager.h"
#include "ProjectsProxy.h"
#include "UISettingsQMake.h"

QMake::QMake()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "QMake" );
	mPluginInfos.Description = tr( "Plugin for managing QMake project" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>; Roper Alexander aka minirop <minirop@peyj.com>";
	mPluginInfos.Type = BasePlugin::iProject;
	mPluginInfos.Name = "QMake";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

bool QMake::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// set usable suffixes
		mSuffixes[tr( "Qt Projects" )] = QStringList() << "*.pro" << "*.pri";
		// set filtered items
		UIProjectsManager::instance()->proxy()->addFilterValues( UISettingsQMake::readFilters() );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// clear suffixes
		mSuffixes.clear();
		// unset filtered items
		UIProjectsManager::instance()->proxy()->removeFilterValues( UISettingsQMake::readFilters() );
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QWidget* QMake::settingsWidget()
{ return UISettingsQMake::instance(); }

ProjectItem* QMake::getProjectItem( const QString& s )
{
	// don t open project if plugin is not enabled
	if ( !isEnabled() || !QDir::match( mSuffixes[tr( "Qt Projects" )], s ) )
		return 0;
	
	// create project item
	ProjectItem* it = new QMakeItem( ProjectItem::ProjectType );
	
	// set project filename
	it->setValue( s );
	
	// set item plugin
	it->setPlugin( this );
	
	// return item
	return it;
}

Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
