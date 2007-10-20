#include "UIAbout.h"

#include <QFile>
#include <QDesktopServices>
#include <QTabBar>

const QString mInformations =
"<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" style=\"font-size:small;\">"
"	<tr>"
"		<td align=\"center\"><br /><img src=\":/application/icons/application/monkey2.png\" width=\"32\" height=\"32\"></td>"
"		<td>%2 v%3<br />%4<br />The Monkey Studio Team<br /><a href=\"http://%6\">Home page</a></td>"
"	</tr>"
"</table>";

const QString mDatasMask =
"<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" style=\"font-size:small;\">"
"	<tr>"
"		<td>"
"			<b>%1</b><br />"
"			%2 - %3<br />"
"			%4 (<a href=\"%5\">%6</a>)<br />"
"		</td>"
"	</tr>"
"</table>";

const QList<pDatas> mTeamates = QList<pDatas>()
	<< pDatas( "Filipe Azevedo", "Nox P@sNox", QObject::tr( "France" ), "pasnox@gmail.com", QObject::tr( "Creator & Principal Developer" ) )
	<< pDatas( "Manuel Schmidt", "oversize", QObject::tr( "Germany" ), "manuel@schmidtman.de", QObject::tr( "Web Developer & Web Designer" ) )
	<< pDatas( "Yannick", "xiantia", QObject::tr( "France" ), "xiantia@gmail.com", QObject::tr( "GDB Driver Integration" ) )
	<< pDatas( "Julien Decologne", "Judd", QObject::tr( "France" ), "judd@hotmail.com", QObject::tr( "Splashscreen & Icons Designer" ) )
	<< pDatas( "Mashin Evgeniy", "evgenM", QObject::tr( "Russia" ), "mashin.zhenya@gmail.com", QObject::tr( "Many shared code between our IDEs" ) )
	<< pDatas( "Kopats Andrei", "hlamer", QObject::tr( "Belarus" ), "hlamer@tut.by", QObject::tr( "Principal Developer, Class Browser" ) )
	<< pDatas( "Roper Alexander", "Minirop", QObject::tr( "France" ), "minirop@peyj.com", QObject::tr( "Qt Pro Parser, Some Features/Help" ) );

const QList<pDatas> mDonors = QList<pDatas>()
	<< pDatas( "Filipe Azevedo", "Nox P@sNox", QObject::tr( "France" ), "pasnox@gmail.com", QObject::tr( "No donations for now, you can use this <a href=\"http://sourceforge.net/project/project_donations.php?group_id=163493\">link</a> to make donation. Donations will help paying host/domain, and relatives things about the projects ( may need a mac book for better mac version :p )." ) );

const QList<pDatas> mLinks = QList<pDatas>()
	<< pDatas( "", "", QObject::tr( "Location" ), "http://www.trolltech.com", QObject::tr( "Trolltech's Qt homepage." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://webissues.mimec.org", QObject::tr( "Team collaboration across the Internet." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://webissues.monkeystudio.org", QObject::tr( "Our webissues server ( login: anonymous, password: anonymous )." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://trac.monkeystudio.org", QObject::tr( "You can send bug/patch/ideas/what ever you want to our tracker." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://forums.monkeystudio.org", QObject::tr( "A forum is at your disposition." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://www.monkeystudio.org/feeds/latest", QObject::tr( "Get the latest news using the rss feed." ) )
	<< pDatas( "", "", QObject::tr( "Location" ), "http://trac.monkeystudio.org/wiki/MeetTheMonkeys", QObject::tr( "Meet the Monkeys ( All team including testers )." ) );

UIAbout::UIAbout( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );

	// window title
	setWindowTitle( windowTitle().arg( PROGRAM_NAME ) );

	// mouse cursor
	twAbout->findChild<QTabBar*>()->setCursor( Qt::PointingHandCursor );

	// change label color
	QPalette lp( lInformations->palette() );
	lp.setColor( lInformations->backgroundRole(), Qt::white );
	lInformations->setPalette( lp );

	// show informations table
	lInformations->setTextInteractionFlags( Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard );
	lInformations->setText( mInformations.arg( PROGRAM_NAME ).arg( PROGRAM_VERSION ).arg( PROGRAM_COPYRIGHTS ).arg( PROGRAM_DOMAIN ) );

	// team
	foreach ( pDatas i, mTeamates )
		tbTeam->append( mDatasMask.arg( i.Comment ).arg( i.Name ).arg( i.Login ).arg( i.Pays ).arg( QString( "mailto:" ).append( i.Email ) ).arg( i.Email ) );
	tbTeam->moveCursor( QTextCursor::Start );

	// license
	QFile file( ":/licenses/texts/license.gpl" );
	file.open( QFile::ReadOnly );
	tbLicense->setPlainText( file.readAll() );
	file.close();
	tbLicense->moveCursor( QTextCursor::Start );

	// donors
	foreach ( pDatas i, mDonors )
		tbDonations->append( mDatasMask.arg( i.Comment ).arg( i.Name ).arg( i.Login ).arg( i.Pays ).arg( QString( "mailto:" ).append( i.Email ) ).arg( i.Email ) );
	tbDonations->moveCursor( QTextCursor::Start );

	// links
	foreach ( pDatas i, mLinks )
		tbLinks->append( mDatasMask.arg( i.Comment ).arg( i.Name ).arg( i.Login ).arg( i.Pays ).arg( i.Email ).arg( i.Email ) );
	tbLinks->moveCursor( QTextCursor::Start );

	// connections
	foreach ( QTextBrowser* b, twAbout->findChildren<QTextBrowser*>() )
		connect( b, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
}

void UIAbout::anchorClicked( const QUrl& u )
{
	QTextBrowser* b = qobject_cast<QTextBrowser*>( sender() );
	if ( !b )
		return;
	b->setHtml( b->toHtml() );
	QDesktopServices::openUrl( u );
}
