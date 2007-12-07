/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UIProjectHeaders
** FileName  : UIProjectHeaders.cpp
** Date      : 2007-11-04T02:21:34
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UIProjectHeaders.h"
#include "pMonkeyStudio.h"
#include "VariablesManager.h"
#include "ProjectHeaders.h"

#include <QDir>
#include <QFileInfoList>
#include <QTextCodec>
#include <QHash>
#include <QStringList>
#include <QPushButton>
#include <QDateTime>

using namespace pMonkeyStudio;

UIProjectHeaders::UIProjectHeaders( QWidget* p, ProjectHeaders* ph )
	: QDialog( p ), mPlugin( ph )
{
	setupUi( this );
	cbLanguages->addItems( availableLanguages() );
	dbbButtons->button( QDialogButtonBox::Ok )->setIcon( QPixmap( ":/icons/icons/ok.png" ) );
	dbbButtons->button( QDialogButtonBox::Cancel )->setIcon( QPixmap( ":/icons/icons/cancel.png" ) );
}

void UIProjectHeaders::on_cbLanguages_highlighted( int )
{
	cbLanguages->setItemData( cbLanguages->currentIndex(), leRegExp->text(), Qt::UserRole +1 );
	cbLanguages->setItemData( cbLanguages->currentIndex(), teLicensing->toPlainText(), Qt::UserRole +2 );
}

void UIProjectHeaders::on_cbLanguages_currentIndexChanged( int i )
{
	leRegExp->setText( cbLanguages->itemData( i, Qt::UserRole +1 ).toString() );
	teLicensing->setPlainText( cbLanguages->itemData( i, Qt::UserRole +2 ).toString() );
}

void UIProjectHeaders::on_tbDirectory_clicked()
{
	const QString s = getExistingDirectory( tr( "Choose the directory to scan" ), QString(), window() );
	if ( !s.isEmpty() )
		leDirectory->setText( s );
}

void UIProjectHeaders::accept()
{
	// get filters for this language
	QStringList filters;
	QHash<QString, QStringList> l = availableLanguagesSuffixes();
	foreach ( QString k, l.keys() )
		foreach ( QString e, l.value( k ) )
			if ( !filters.contains( e ) )
				filters << e;
	
	// set dictionnary
	VariablesManager::Dictionary v;
	v[ "authors" ] = leAuthors->text();
	v[ "projectname" ] = leProjectName->text();
	v[ "license" ] = cbLicenses->currentText();
	v[ "homepage" ] = leHomePage->text();
	v[ "comment" ] = teComment->toPlainText().trimmed();
	
	// get variables manager
	VariablesManager* vm = VariablesManager::instance();
	
	// get recursive files
	foreach ( QFileInfo fi, getFiles( QDir( leDirectory->text() ), filters, true ) )
	{
		QFile f( fi.absoluteFilePath() );
		f.open( QIODevice::ReadWrite | QIODevice::Text );
		QString b = QTextCodec::codecForLocale()->toUnicode( f.readAll() );
		QString l = languageForFileName( fi.fileName() );
		
		// regexp
		QRegExp rx( mPlugin->templatesHeaderRegExp( l ) );
		rx.setMinimal( true );
		
		int i;
		if ( ( i = rx.indexIn( b ) ) != -1 )
		{
			if ( !question( tr( "Replace Licensing..." ), tr( "There already is a licensing, replace it ?\n\n%1" ).arg( rx.cap( 1 ) ), window() ) )
				continue;
			b.remove( 0, rx.cap( 1 ).length() );
		}
		
		// set variables
		v[ "filename" ] = fi.fileName();
		v[ "date" ] = fi.created().toString( Qt::ISODate );
		
		// replaces variables
		b.prepend( vm->replaceAllVariables( mPlugin->templatesHeader( l ), v ) );
		
		// write buffer
		f.resize( 0 );
		f.write( qPrintable( b ) );
		f.close();
	}
}
