#include "UISettingsQMakeXUPProjectEditor.h"

#include <monkey.h>

#include <QPushButton>
#include <QDir>
#include <QWhatsThis>

UISettingsQMakeXUPProjectEditor::UISettingsQMakeXUPProjectEditor( QWidget* parent )
	: QWidget( parent )
{
	// set up dialog
	setupUi( this );
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QIcon( ":/help/icons/help/keyword.png" ) );
	dbbButtons->button( QDialogButtonBox::Save )->setIcon( QIcon( ":/file/icons/file/save.png" ) );
	// load settings
	loadSettings();
	// connections
	connect( lwQtVersions, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwQtModules, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwQtConfigurations, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbAdd*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbAdd_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbRemove*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbRemove_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbClear*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbClear_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbUp*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbUp_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbDown*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbDown_clicked() ) );
}

void UISettingsQMakeXUPProjectEditor::tbAdd_clicked()
{
	QListWidget* lw = 0;
	if ( sender() == tbAddQtVersion )
		lw = lwQtVersions;
	else if ( sender() == tbAddFilter )
		lw = lwFilters;
	else if ( sender() == tbAddScope )
		lw = lwScopes;
	else if ( sender() == tbAddPathFile )
		lw = lwPathFiles;
	else if ( sender() == tbAddQtModule )
		lw = lwQtModules;
	else if ( sender() == tbAddQtConfiguration )
		lw = lwQtConfigurations;
	if ( lw )
	{
		lw->addItem( tr( "New value" ) );
		lw->setCurrentItem( lw->item( lw->count() -1 ) );
		lw->scrollToItem( lw->item( lw->count() -1 ) );
		lw->currentItem()->setFlags( lw->currentItem()->flags() | Qt::ItemIsEditable );
	}
}

void UISettingsQMakeXUPProjectEditor::tbRemove_clicked()
{
	if ( sender() == tbRemoveQtVersion )
		delete lwQtVersions->selectedItems().value( 0 );
	else if ( sender() == tbRemoveFilter )
		delete lwFilters->selectedItems().value( 0 );
	else if ( sender() == tbRemoveScope )
		delete lwScopes->selectedItems().value( 0 );
	else if ( sender() == tbRemovePathFile )
		delete lwPathFiles->selectedItems().value( 0 );
	else if ( sender() == tbRemoveQtModule )
		delete lwQtModules->selectedItems().value( 0 );
	else if ( sender() == tbRemoveQtConfiguration )
		delete lwQtConfigurations->selectedItems().value( 0 );
}

void UISettingsQMakeXUPProjectEditor::tbClear_clicked()
{
	if ( sender() == tbClearQtVersions )
		lwQtVersions->clear();
	else if ( sender() == tbClearFilters )
		lwFilters->clear();
	else if ( sender() == tbClearScopes )
		lwScopes->clear();
	else if ( sender() == tbClearPathFiles )
		lwPathFiles->clear();
	else if ( sender() == tbClearQtModules )
		lwQtModules->clear();
	else if ( sender() == tbClearQtConfiguration )
		lwQtConfigurations->clear();
}

void UISettingsQMakeXUPProjectEditor::tbUp_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QListWidget* lw = 0;
	if ( tb == tbUpQtVersion )
		lw = lwQtVersions;
	else if ( tb == tbUpFilter )
		lw = lwFilters;
	else if ( tb == tbUpScope )
		lw = lwScopes;
	else if ( tb == tbUpPathFile )
		lw = lwPathFiles;
	else if ( tb == tbUpQtModule )
		lw = lwQtModules;
	else if ( tb == tbUpQtConfiguration )
		lw = lwQtConfigurations;
	if ( !lw )
		return;
	if ( QListWidgetItem* it = lw->selectedItems().value( 0 ) )
	{
		int i = lw->row( it );
		if ( i != 0 )
			lw->insertItem( i -1, lw->takeItem( i ) );
		lw->setCurrentItem( it );
	}
}

void UISettingsQMakeXUPProjectEditor::tbDown_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QListWidget* lw = 0;
	if ( tb == tbDownQtVersion )
		lw = lwQtVersions;
	else if ( tb == tbDownFilter )
		lw = lwFilters;
	else if ( tb == tbDownScope )
		lw = lwScopes;
	else if ( tb == tbDownPathFile )
		lw = lwPathFiles;
	else if ( tb == tbDownQtModule )
		lw = lwQtModules;
	else if ( tb == tbDownQtConfiguration )
		lw = lwQtConfigurations;
	if ( !lw )
		return;
	if ( QListWidgetItem* it = lw->selectedItems().value( 0 ) )
	{
		int i = lw->row( it );
		if ( i != lw->count() -1 )
			lw->insertItem( i +1, lw->takeItem( i ) );
		lw->setCurrentItem( it );
	}
}

void UISettingsQMakeXUPProjectEditor::on_tbDefaultQtVersion_clicked()
{
	if ( QListWidgetItem* it = lwQtVersions->selectedItems().value( 0 ) )
	{
		for ( int i = 0; i < lwQtVersions->count(); i++ )
		{
			QListWidgetItem* cit = lwQtVersions->item( i );
			QtVersion v = cit->data( Qt::UserRole ).value<QtVersion>();
			v.Default = it == cit;
			cit->setData( Qt::UserRole, QVariant::fromValue( v ) );
			cit->setBackground( QBrush( v.Default ? Qt::green : Qt::transparent ) );
		}
	}
}

void UISettingsQMakeXUPProjectEditor::on_leQtVersionPath_textChanged( const QString& )
{ lw_currentItemChanged( lwQtVersions->currentItem(), lwQtVersions->currentItem() ); }

void UISettingsQMakeXUPProjectEditor::on_tbQtVersionPath_clicked()
{
	const QString s = pMonkeyStudio::getExistingDirectory( tr( "Locate your qt installation directory" ), leQtVersionPath->text(), window() );
	if ( !s.isNull() )
		leQtVersionPath->setText( s );
}

void UISettingsQMakeXUPProjectEditor::on_tbQtVersionQMakeSpec_clicked()
{
	const QString s = pMonkeyStudio::getExistingDirectory( tr( "Locate your qt installation directory" ), leQtVersionPath->text(), window() );
	if ( !s.isNull() )
	{
		if ( cbQtVersionQMakeSpec->findText( s ) == -1 )
			cbQtVersionQMakeSpec->addItem( s );
		cbQtVersionQMakeSpec->setCurrentIndex( cbQtVersionQMakeSpec->findText( s ) );
	}
}

void UISettingsQMakeXUPProjectEditor::lw_currentItemChanged( QListWidgetItem* c, QListWidgetItem* p )
{
	if ( p )
	{
		if ( p->listWidget() == lwQtVersions )
		{
			QtVersion v = p->data( Qt::UserRole ).value<QtVersion>();
			v.Version = leQtVersionVersion->text();
			v.Path = leQtVersionPath->text();
			v.QMakeSpec = cbQtVersionQMakeSpec->currentText();
			v.QMakeParameters = leQtVersionQMakeParameters->text();
			p->setData( Qt::UserRole, QVariant::fromValue( v ) );
			p->setText( v.Version );
		}
		else if ( p->listWidget() == lwQtModules )
		{
			QtItem it = p->data( Qt::UserRole ).value<QtItem>();
			it.Text = leCaptionQtModule->text();
			it.Value = leValueQtModule->text();
			it.Variable = leVariableQtModule->text().toUpper();
			it.Help = teHelpQtModule->toPlainText();
			p->setData( Qt::UserRole, QVariant::fromValue( it ) );
			p->setText( it.Text );
		}
		else if ( p->listWidget() == lwQtConfigurations )
		{
			QtItem it = p->data( Qt::UserRole ).value<QtItem>();
			it.Text = leCaptionQtConfiguration->text();
			it.Value = leValueQtConfiguration->text();
			it.Variable = leVariableQtConfiguration->text().toUpper();
			it.Help = teHelpQtConfiguration->toPlainText();
			p->setData( Qt::UserRole, QVariant::fromValue( it ) );
			p->setText( it.Text );
		}
	}
	if ( c )
	{
		if ( c->listWidget() == lwQtVersions )
		{
			QtVersion v = c->data( Qt::UserRole ).value<QtVersion>();
			leQtVersionVersion->setText( v.Version );
			leQtVersionPath->setText( v.Path );
			// need to get all mkspecs for this qt versions
			cbQtVersionQMakeSpec->clear();
			QDir mkspecs( QString( v.Path ).append( "/mkspecs" ) );
			if ( mkspecs.exists() )
			{
				foreach ( const QFileInfo& fi, mkspecs.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name ) )
				{
					if ( fi.fileName() != "common" && fi.fileName() != "features" )
						cbQtVersionQMakeSpec->addItem( fi.fileName() );
				}
			}
			if ( cbQtVersionQMakeSpec->findText( v.QMakeSpec ) == -1 )
				cbQtVersionQMakeSpec->addItem( v.QMakeSpec );
			cbQtVersionQMakeSpec->setCurrentIndex( cbQtVersionQMakeSpec->findText( v.QMakeSpec ) );
			leQtVersionQMakeParameters->setText( v.QMakeParameters );
			wQtVersion->setEnabled( true );
		}
		else if ( c->listWidget() == lwQtModules )
		{
			QtItem it = c->data( Qt::UserRole ).value<QtItem>();
			leCaptionQtModule->setText( it.Text );
			leValueQtModule->setText( it.Value );
			leVariableQtModule->setText( it.Variable );
			teHelpQtModule->setPlainText( it.Help );
			wQtModule->setEnabled( true );
		}
		else if ( c->listWidget() == lwQtConfigurations )
		{
			QtItem it = c->data( Qt::UserRole ).value<QtItem>();
			leCaptionQtConfiguration->setText( it.Text );
			leValueQtConfiguration->setText( it.Value );
			leVariableQtConfiguration->setText( it.Variable );
			teHelpQtConfiguration->setPlainText( it.Help );
			wQtConfiguration->setEnabled( true );
		}
	}
	else if ( sender() == lwQtVersions )
	{
		wQtVersion->setEnabled( false );
		leQtVersionVersion->clear();
		leQtVersionPath->clear();
		cbQtVersionQMakeSpec->clear();
	}
	else if ( sender() == lwQtModules )
	{
		wQtModule->setEnabled( false );
		leCaptionQtModule->clear();
		leValueQtModule->clear();
		leVariableQtModule->clear();
		teHelpQtModule->clear();
	}
	else if ( sender() == lwQtConfigurations )
	{
		wQtConfiguration->setEnabled( false );
		leCaptionQtConfiguration->clear();
		leValueQtConfiguration->clear();
		leVariableQtConfiguration->clear();
		teHelpQtConfiguration->clear();
	}
}

void UISettingsQMakeXUPProjectEditor::loadSettings()
{
	// general
	foreach ( const QtVersion& v, mQtManager.versions() )
	{
		QListWidgetItem* it = new QListWidgetItem( v.Version, lwQtVersions );
		it->setData( Qt::UserRole, QVariant::fromValue( v ) );
		if ( v.Default )
			it->setBackground( QBrush( Qt::green ) );
	}
	/*
	// filters & scopes
	lwFilters->addItems( readFilters() );
	lwScopes->addItems( readScopes() );
	lwPathFiles->addItems( readPathFiles() );
	// set filters tooltips
	QStringList ft = readFiltersToolTips();
	for ( int i = 0; i < ft.count(); i++ )
		if ( QListWidgetItem* it = lwFilters->item( i ) )
			it->setToolTip( ft.at( i ) );
	// set items editable
	QList<QListWidgetItem*> items = QList<QListWidgetItem*> () 
	<< lwFilters->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive )
	<< lwScopes->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive )
	<< lwPathFiles->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive );
	foreach ( QListWidgetItem* it, items )
		it->setFlags( it->flags() | Qt::ItemIsEditable );
	items.clear();
	*/
	// qt modules
	foreach ( QtItem i, mQtManager.modules() )
	{
		QListWidgetItem* it = new QListWidgetItem( i.Text, lwQtModules );
		it->setData( Qt::UserRole, QVariant::fromValue( i ) );
	}
	// configuration
	foreach ( QtItem i, mQtManager.configurations() )
	{
		QListWidgetItem* it = new QListWidgetItem( i.Text, lwQtConfigurations );
		it->setData( Qt::UserRole, QVariant::fromValue( i ) );
	}
}

void UISettingsQMakeXUPProjectEditor::on_dbbButtons_helpRequested()
{
	QString s;
	switch ( twQMake->currentIndex() )
	{
		case 0:
			s = tr( "Here you can configure the path/filename for the differents Qt tools, generate Qt api for auto completion." );
			break;
		case 1:
			s = tr( "<b>Filters</b>: Are the variables names that are shown when the project view is filtered.<br/>"
				"<b>Scopes</b>: scopes are used in projects settings so you can configure variable for differents scopes easily.<br/>"
				"<b>Path/Files</b>: This list is used to let plugin know about witch variable names are based on path or files for there contents." 	);
			break;
		case 2:
			s = tr( "<b>Qt Modules</b>: This list contains differents modules available with Qt, you can add your own module if you want, so you can easily check them in project settings." );
			break;
		case 3:
			s = tr( "<b>Settings</b>: This list contains differents settings available with Qt, you can add your own if you want, so you can easily check them in project settings." );
			break;
	}
	if ( !s.isEmpty() )
		QWhatsThis::showText( mapToGlobal( rect().center() ), s );
}

void UISettingsQMakeXUPProjectEditor::on_dbbButtons_clicked( QAbstractButton* b )
{
	// only accept save button
	if ( dbbButtons->standardButton( b )  != QDialogButtonBox::Save )
		return;
	// save qt versions
	QtVersionList versions;
	for ( int i = 0; i < lwQtVersions->count(); i++ )
		versions << lwQtVersions->item( i )->data( Qt::UserRole ).value<QtVersion>();
	mQtManager.setVersions( versions );
	// save modules
	QtItemList modules;
	for ( int i = 0; i < lwQtModules->count(); i++ )
		modules << lwQtModules->item( i )->data( Qt::UserRole ).value<QtItem>();
	mQtManager.setModules( modules );
	// save configurations
	QtItemList configurations;
	for ( int i = 0; i < lwQtConfigurations->count(); i++ )
		configurations << lwQtConfigurations->item( i )->data( Qt::UserRole ).value<QtItem>();
	mQtManager.setConfigurations( configurations );
}
