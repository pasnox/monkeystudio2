#include "pActionsManager.h"
#include "pActionsShortcutsManager.h"

#include <QApplication>
#include <QDir>
#include <QSettings>

#include <QDebug>

const QString pActionsManager::mSettingsScope = "Shortcuts Manager";
QMapStringString pActionsManager::mPathPartTranslations;

pActionsManager::pActionsManager( const QString& name, QObject* parent )
	: QObject( parent )
{
	Q_ASSERT( !name.isEmpty() );
	
	mName = name;
	mSettings = 0;
}

pActionsManager::~pActionsManager()
{
	qDeleteAll( mActions );
}

void pActionsManager::setSettings( QSettings* settings )
{
	mSettings = settings;
}

QSettings* pActionsManager::settings() const
{
	return mSettings;
}

QString pActionsManager::name() const
{
	return mName;
}

void pActionsManager::reloadSettings() const
{
	if ( mSettings )
	{
		foreach ( QAction* action, actions() )
		{
			const QString key = QString( "%1/%2/%3/%4" ).arg( mSettingsScope ).arg( mName ).arg( actionPath( action ) ).arg( action->objectName() );
			QString shortcutText = defaultShortcut( action );
			shortcutText = mSettings->value( key, shortcutText ).toString();
			const QKeySequence shortcut = QKeySequence( shortcutText );
			
			action->setShortcut( shortcut );
		}
	}
}

void pActionsManager::actionDestroyed( QObject* object )
{
	foreach ( QAction* action, mActions )
	{
		if ( action == object )
		{
			mActions.removeAll( action );
		}
	}
}

void pActionsManager::editActionsShortcuts()
{
	pActionsShortcutsManager dlg( this, QApplication::activeWindow() );
	
	dlg.exec();
}

QActionList pActionsManager::actions( const QString& path ) const
{
	if ( path.isEmpty() )
	{
		return mActions;
	}
	
	QString fPath = fixedPath( path );
	QActionList actions;
	
	foreach ( QAction* action, mActions )
	{
		if ( actionPath( action ) == fPath )
		{
			actions << action;
		}
	}
	
	return actions;
}

QAction* pActionsManager::action( const QString& path, const QString& name ) const
{
	foreach ( QAction* action, actions( path ) )
	{
		if ( action->objectName() == name )
		{
			return action;
		}
	}
	
	return 0;
}

QAction* pActionsManager::newAction( const QString& path, const QKeySequence& dShortcut, const QString& name )
{
	Q_ASSERT( !name.isEmpty() );
	
	QAction* action = pActionsManager::action( path, name );
	
	if ( action )
	{
		if ( defaultShortcut( action ) != dShortcut )
		{
			setDefaultShortcut( action, dShortcut );
		}
		
		return action;
	}
	
	action = new QAction( this );
	
	setActionsManager( action, this );
	setActionPath( action, path );
	setDefaultShortcut( action, dShortcut );
	action->setObjectName( name );
	
	return action;
}

QString pActionsManager::lastError() const
{
	return mLastError;
}

QString pActionsManager::fixedPath( const QString& path )
{
	QString fPath = QDir::cleanPath( path ).replace( "\\", "/" );
	
	if ( fPath.endsWith( "/" ) )
	{
		fPath.chop( 1 );
	}
	
	return fPath;
}

bool pActionsManager::setShortcut( QAction* action, const QKeySequence& shortcut )
{
	Q_ASSERT( action );
	
	pActionsManager* manager = actionsManager( action );
	
	Q_ASSERT( manager );
	
	// try to found already used shortcut
	foreach ( QAction* a, manager->mActions )
	{
		if ( a != action && a->shortcut() == shortcut && !shortcut.isEmpty() )
		{
			manager->mLastError = tr( "Key Sequence '%1' already assigned to '%2/%3'" ).arg( shortcut.toString() ).arg( actionPath( a ) ).arg( a->text() );
			return false;
		}
	}
	
	// remove old/set shortcut entry
	QSettings* settings = manager->settings();
	if ( settings )
	{
		const QString key = QString( "%1/%2/%3/%4" ).arg( mSettingsScope ).arg( manager->mName ).arg( actionPath( action ) ).arg( action->objectName() );
		
		if ( action->shortcut() == shortcut )
		{
			settings->remove( key );
		}
		else
		{
			settings->setValue( key, shortcut.toString() );
		}
	}
	
	// set new shortcut to action
	action->setShortcut( shortcut );
	
	// return success
	return true;
}

pActionsManager* pActionsManager::actionsManager( QAction* action )
{
	Q_ASSERT( action );
	return action->property( QString::number( ActionsManager ).toLocal8Bit().constData() ).value<pActionsManager*>();
}

void pActionsManager::setActionsManager( QAction* action, pActionsManager* manager )
{
	Q_ASSERT( action );
	
	// reparent
	action->setParent( qApp );
	
	// remove it from old manager if needed
	pActionsManager* curManager = actionsManager( action );
	
	// unmanage action
	if ( curManager )
	{
		curManager->mActions.removeAll( action );
		action->setProperty( QString::number( ActionsManager ).toLocal8Bit().constData(), 0 );
		
		disconnect( action, SIGNAL( destroyed( QObject* ) ), curManager, SLOT( actionDestroyed( QObject* ) ) );
	}
	
	// manage action
	if ( manager )
	{
		action->setParent( manager );
		manager->mActions.append( action );
		action->setProperty( QString::number( ActionsManager ).toLocal8Bit().constData(), QVariant::fromValue( manager ) );
		
		connect( action, SIGNAL( destroyed( QObject* ) ), manager, SLOT( actionDestroyed( QObject* ) ) );
	}
}

QString pActionsManager::actionPath( QAction* action )
{
	Q_ASSERT( action );
	return action->property( QString::number( ActionPath ).toLocal8Bit().constData() ).toString();
}

void pActionsManager::setActionPath( QAction* action, const QString& path )
{
	Q_ASSERT( action );
	action->setProperty( QString::number( ActionPath ).toLocal8Bit().constData(), fixedPath( path ) );
}

QKeySequence pActionsManager::defaultShortcut( QAction* action )
{
	Q_ASSERT( action );
	return action->property( QString::number( DefaultShortcut ).toLocal8Bit().constData() ).value<QKeySequence>();
}

void pActionsManager::setDefaultShortcut( QAction* action, const QKeySequence& shortcut )
{
	Q_ASSERT( action );
	action->setProperty( QString::number( DefaultShortcut ).toLocal8Bit().constData(), QVariant::fromValue( shortcut ) );
	
	if ( action->shortcut().isEmpty() )
	{
		action->setShortcut( shortcut );
	}
}

QString pActionsManager::pathPartTranslation( const QString& part )
{
	return mPathPartTranslations.value( part, part );
}

QString pActionsManager::pathTranslation( const QString& path, const QString& separator )
{
	QString fPath = fixedPath( path );
	QStringList parts = fPath.split( "/", QString::SkipEmptyParts );
	
	for ( int i = 0; i < parts.count(); i++ )
	{
		parts[ i ] = pathPartTranslation( parts.at( i ) );
	}
	
	return parts.join( separator );
}

QString pActionsManager::pathTranslation( QAction* action, const QString& separator )
{
	Q_ASSERT( action );
	
	return pathTranslation( actionPath( action ), separator );
}

void pActionsManager::setPathPartTranslation( const QString& part, const QString& translation )
{
	mPathPartTranslations[ part ] = translation;
}
