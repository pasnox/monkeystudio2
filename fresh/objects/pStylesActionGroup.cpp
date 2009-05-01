#include "pStylesActionGroup.h"

#include <QStyleFactory>
#include <QSysInfo>

pStylesActionGroup::pStylesActionGroup( const QString& textFormat, QObject* parent )
	: QActionGroup( parent )
{
	mCheckable = true;
	mTextFormat = textFormat;
	
	refreshActions();
	
	connect( this, SIGNAL( triggered( QAction* ) ), this, SLOT( actionTriggered( QAction* ) ) );
}

QString pStylesActionGroup::systemStyle()
{
	QString style;
#if defined( Q_OS_WIN )
	switch ( QSysInfo::WindowsVersion )
	{
		case QSysInfo::WV_XP:
		case QSysInfo::WV_2003:
			style = "windowsxp";
			break;
		case QSysInfo::WV_VISTA:
			style = "windowsvista";
			break;
		case QSysInfo::WV_32s:
		case QSysInfo::WV_95:
		case QSysInfo::WV_98:
		case QSysInfo::WV_Me:
		case QSysInfo::WV_NT:
		case QSysInfo::WV_2000:
		default:
			style = "windows";
			break;
	}
#elif defined( Q_OS_MAC )
	style = "macintosh";
#else
	const QString desktop = qgetenv( "DESKTOP_SESSION" ).toLower();
	const QString version = qgetenv( QString( "%1_SESSION_VERSION" ).arg( desktop.toUpper() ).toLocal8Bit() );
	
	if ( desktop == "kde" && version == "4" )
	{
		style = "oxygen";
	}
	else if ( desktop == "gnome" )
	{
#if QT_VERSION < 0x040500
		style = "cleanlooks";
#else
		style = "gtk+";
#endif
	}
	else
	{
		style = "plastique";
	}
#endif
	
	return style;
}

QAction* pStylesActionGroup::systemAction() const
{
	return mActions.value( systemStyle() );
}

void pStylesActionGroup::refreshActions()
{
	QString curStyle = currentStyle();
	
	qDeleteAll( mActions.values() );
	mActions.clear();
	
	// Add style actions
	const QStringList styles = QStyleFactory::keys();
	const QStringList::const_iterator cend = styles.constEnd();
	
	// Make sure ObjectName  is unique in case toolbar solution is used.
	QString objNamePrefix = QLatin1String( "__qt_designer_style_" );
	
	// Create styles. Set style name string as action data.
	for ( QStringList::const_iterator it = styles.constBegin(); it !=  cend ;++it )
	{
		QAction* a = new QAction( mTextFormat.arg( *it ), this );
		QString objName = objNamePrefix;
		objName += *it;
		//objName += objNamePostfix;
		
		a->setObjectName( objName );
		a->setData( *it );
		a->setCheckable( true );
		a->setChecked( *it == curStyle );
		
		mActions[ (*it).toLower() ] = a;
		
		addAction( a );
	}
}

void pStylesActionGroup::setCheckable( bool checkable )
{
	mCheckable = checkable;
	
	foreach ( QAction* action, actions() )
	{
		action->setCheckable( mCheckable );
	}
}

bool pStylesActionGroup::isCheckable() const
{
	return mCheckable;
}

void pStylesActionGroup::actionTriggered( QAction* action )
{
	emit styleSelected( action->data().toString() );
}

void pStylesActionGroup::setCurrentStyle( const QString& style )
{
	QAction* action = mActions.value( style );
	
	if ( action )
	{
		action->setChecked( true );
	}
}

QString pStylesActionGroup::currentStyle() const
{
	return checkedAction() ? checkedAction()->data().toString() : systemStyle();
}
