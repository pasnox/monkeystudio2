#include "pStylesActionGroup.h"

#include <QStyleFactory>
#include <QStyle>
#include <QApplication>

pStylesActionGroup::pStylesActionGroup( const QString& textFormat, QObject* parent )
	: QActionGroup( parent )
{
	mCheckable = true;
	mTextFormat = textFormat;
	
	refreshActions();
	
	connect( this, SIGNAL( triggered( QAction* ) ), this, SLOT( actionTriggered( QAction* ) ) );
}

QStringList pStylesActionGroup::availableStyles()
{
	return QStyleFactory::keys();
}

QString pStylesActionGroup::systemStyle()
{
	const QStringList styles = availableStyles();
	
#if defined( Q_OS_WIN )
	const QStringList possibleStyles = QStringList()
		<< "windowsvista"
		<< "windowsxp"
		<< "windows";
	
	for ( int i = possibleStyles.count() -1; i > -1; i-- ) {
		if ( styles.contains( possibleStyles.at( i ), Qt::CaseInsensitive ) ) {
			return possibleStyles.at( i );
		}
	}
#elif defined( Q_OS_MAC )
	return "macintosh (aqua)";
#else
	const QString desktop = qgetenv( "DESKTOP_SESSION" ).toLower();
	const QString version = qgetenv( QString( "%1_SESSION_VERSION" ).arg( desktop.toUpper() ).toLocal8Bit() );
	QString style;
	
	if ( desktop == "kde" && version == "4" ) {
		style = "oxygen";
	}
	else if ( desktop == "gnome" ) {
		style = styles.contains( "gtk+", Qt::CaseInsensitive ) ? "gtk+" :"cleanlooks";
	}
	
	if ( styles.contains( style, Qt::CaseInsensitive ) ) {
		return style;
	}
#endif
	
	return styles.value( 0 );
}

QString pStylesActionGroup::applicationStyle()
{
	return QApplication::style()->objectName();
}

QAction* pStylesActionGroup::applicationAction() const
{
	return mActions.value( applicationStyle() );
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
		a->setData( ( *it ).toLower() );
		a->setCheckable( true );
		a->setChecked( *it == curStyle );
		
		mActions[ ( *it ).toLower() ] = a;
		
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
	QAction* action = mActions.value( style.toLower() );
	
	if ( action )
	{
		action->setChecked( true );
	}
}

QString pStylesActionGroup::currentStyle() const
{
	return checkedAction() ? checkedAction()->data().toString() : systemStyle();
}
