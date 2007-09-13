#include "UIQMakeProjectSettings.h"
#include "QMakeProxy.h"
#include "ProjectsModel.h"
#include "ProjectItem.h"
#include "UIItemSettings.h"
#include "pMonkeyStudio.h"

#include <QCompleter>
#include <QDirModel>
#include <QHeaderView>
#include <QLocale>
#include <QFileDialog>
#include <QInputDialog>
#include <QFileIconProvider>
#include <QTextCodec>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QWhatsThis>

QString mTranslationMask = "$$TRANSLATIONS_PATH/%2_%3.ts";

UIQMakeProjectSettings::UIQMakeProjectSettings( ProjectItem* m, QWidget* p )
	: QDialog( p ), mProject( m ), mModel( m->model() ), mDirs( new QDirModel( this ) )
{
	Q_ASSERT( mProject != 0 );
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	// set window title
	setWindowTitle( QString( "Project Settings - %1" ).arg( mProject->name() ) );
	// load scopes
	cbScopes->addItems( UISettingsQMake::readScopes() );
	// load operators
	cbOperators->addItems( UISettingsQMake::readOperators() );
	// load text codec
	cbEncodings->addItems( pMonkeyStudio::availableTextCodecs() );
	// load modules & config
	loadModules();
	loadConfigs();
	// set button pixmap
	dbbButtons->button( QDialogButtonBox::Ok )->setIcon( QPixmap( ":/Icons/Icons/buttonok.png" ) );
	dbbButtons->button( QDialogButtonBox::Cancel )->setIcon( QPixmap( ":/Icons/Icons/buttoncancel.png" ) );
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QPixmap( ":/Icons/Icons/helpkeyword.png" ) );
	//
	mDirs->setReadOnly( true );
	mDirs->setFilter( QDir::AllDirs );
	mDirs->setSorting( QDir::Name );
	leDir->setCompleter( new QCompleter( mDirs, leDir ) );
	lvDirs->setModel( mDirs );
#ifndef Q_WS_WIN
	// all os except windows got only one root drive, so don't need to show root
	lvDirs->setRootIndex( mDirs->index( "/" ) );
#endif
	//
	setDir( mDirs->index( mProject->canonicalPath() ) );
	// scopes
	mScopesProxy = new QMakeProxy( mModel, false, mProject );
	mScopesProxy->setFilterRoles( QList<int>() << ProjectsModel::ValueType );
	mScopesProxy->setFiltering( true );
	tvScopes->setModel( mScopesProxy );
	tvScopes->header()->hide();
	tvScopes->setRootIndex( mScopesProxy->mapFromSource( mProject->index() ) );
	// scope content
	mContentProxy = new QMakeProxy( mModel, false, mProject );
	mContentProxy->setFilterRoles( QList<int>() << ProjectsModel::ValueType );
	mContentProxy->setNegateFilter( false );
	mContentProxy->setFiltering( true );
	lvContents->setModel( mContentProxy );
	// set currentindex
	setCurrentIndex( mProject->child( 0, 0 )->index() );
	//loadLanguages();
	// connections
	connect( cbScopes, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( cbOperators, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( cbVariables, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	/*
	connect( tbIcon, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( tbHelpFile, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	foreach ( QSpinBox* sb, gbVersion->findChildren<QSpinBox*>() )
		if ( sb != sbBuild )
			connect( sb, SIGNAL( valueChanged( int ) ), this, SLOT( sb_valueChanged( int ) ) );
	connect( lwQtModules, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwCompilerFlags, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( cbScopes, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( cbOperators, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( tbOutputPath, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( cbVariables, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( leDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( setDir( const QString& ) ) );
	connect( lvDirs, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( setDir( const QModelIndex& ) ) );
	connect( tbTranslationsPath, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	mReady = true;
	*/
	// settings
	on_cbOperators_currentIndexChanged( cbOperators->currentText() );
}

void UIQMakeProjectSettings::showEvent( QShowEvent* )
{ QTimer::singleShot( 0, this, SLOT( querySettings() ) ); }

void UIQMakeProjectSettings::closeEvent( QCloseEvent* e )
{
	if ( result() == QDialog::Rejected && mSettings != mOriginalSettings && QMessageBox::question( this, tr( "Cancel..." ), tr( "The project has been modified, are you sure ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
		e->ignore();
}

QStringList UIQMakeProjectSettings::currentCONFIG() const
{
	QStringList l;
	if ( rbDebugRelease->isChecked() ) l << rbDebugRelease->statusTip();
	if ( rbDebug->isChecked() ) l << rbDebug->statusTip();
	if ( rbRelease->isChecked() ) l << rbRelease->statusTip();
	if ( rbWarnOff->isChecked() ) l << rbWarnOff->statusTip();
	if ( rbWarnOn->isChecked() ) l << rbWarnOn->statusTip();
	if ( cbBuildAll->isChecked() ) l << cbBuildAll->statusTip();
	if ( cbOrdered->isChecked() ) l << cbOrdered->statusTip();
	if ( gbQtModules->isChecked() ) l << gbQtModules->statusTip();
	foreach ( QListWidgetItem* it, lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		if ( it->checkState() == Qt::Checked )
			if ( it->data( QtItem::VariableRole ).toString().toUpper() == lwCompilerFlags->statusTip() )
				l << it->data( QtItem::ValueRole ).toString();
	l << leConfig->text();
	return QStringList( l.join( " " ) );
}

void UIQMakeProjectSettings::setCurrentCONFIG( const QString& s )
{
	// uncheck all button, and remove autoexclusivity
	foreach ( QRadioButton* rb, gbBuild->findChildren<QRadioButton*>() )
	{
		rb->setAutoExclusive( false );
		rb->setChecked( false );
	}
	// applying config
	QString c = s;
	rbDebugRelease->setChecked( c.contains( rbDebugRelease->statusTip(), Qt::CaseInsensitive ) );
	c.remove( rbDebugRelease->statusTip(), Qt::CaseInsensitive );
	rbDebug->setChecked( c.contains( rbDebug->statusTip(), Qt::CaseInsensitive ) );
	c.remove( rbDebug->statusTip(), Qt::CaseInsensitive );
	rbRelease->setChecked( c.contains( rbRelease->statusTip(), Qt::CaseInsensitive ) );
	c.remove( rbRelease->statusTip(), Qt::CaseInsensitive );
	rbWarnOff->setChecked( c.contains( rbWarnOff->statusTip(), Qt::CaseInsensitive ) );
	c.remove( rbWarnOff->statusTip(), Qt::CaseInsensitive );
	rbWarnOn->setChecked( c.contains( rbWarnOn->statusTip(), Qt::CaseInsensitive ) );
	c.remove( rbWarnOn->statusTip(), Qt::CaseInsensitive );
	cbBuildAll->setChecked( c.contains( cbBuildAll->statusTip(), Qt::CaseInsensitive ) );
	c.remove( cbBuildAll->statusTip(), Qt::CaseInsensitive );
	cbOrdered->setChecked( c.contains( cbOrdered->statusTip(), Qt::CaseInsensitive ) );
	c.remove( cbOrdered->statusTip(), Qt::CaseInsensitive );
	gbQtModules->setChecked( c.contains( gbQtModules->statusTip(), Qt::CaseInsensitive ) );
	c.remove( gbQtModules->statusTip(), Qt::CaseInsensitive );
	foreach ( QListWidgetItem* it, lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
	{
		if ( it->data( QtItem::VariableRole ).toString().toUpper() == lwCompilerFlags->statusTip() )
			it->setCheckState( c.contains( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive ) ? Qt::Checked : Qt::Unchecked );
		c.remove( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive );
	}
	leConfig->setText( c.simplified() );
	// re active autoexclusivity
	foreach ( QRadioButton* rb, gbBuild->findChildren<QRadioButton*>() )
		rb->setAutoExclusive( true );
}

QStringList UIQMakeProjectSettings::currentQT() const
{
	QStringList l;
	foreach ( QListWidgetItem* it, lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		if ( it->checkState() == Qt::Checked )
			if ( it->data( QtItem::VariableRole ).toString().toUpper() == lwQtModules->statusTip() )
				l << it->data( QtItem::ValueRole ).toString();
	return l;
}

void UIQMakeProjectSettings::setCurrentQT( const QString& s )
{
	foreach ( QListWidgetItem* it, lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		if ( it->data( QtItem::VariableRole ).toString().toUpper() == lwQtModules->statusTip() )
			it->setCheckState( s.contains( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive ) ? Qt::Checked : Qt::Unchecked );
}

QModelIndex UIQMakeProjectSettings::currentIndex()
{
	QModelIndex i;
	if ( lvContents->selectionModel()->selectedIndexes().count() )
		i = mContentProxy->mapToSource( lvContents->selectionModel()->selectedIndexes().at( 0 ) );
	if ( !i.isValid() && tvScopes->selectionModel()->selectedIndexes().count() )
		i = mScopesProxy->mapToSource( tvScopes->selectionModel()->selectedIndexes().at( 0 ) );
	return i;
}

void UIQMakeProjectSettings::setCurrentIndex( const QModelIndex& i )
{
	// clear selection
	tvScopes->clearSelection();
	lvContents->clearSelection();
	// set/select current index
	if ( !i.isValid() )
		return;
	else if ( i.data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::ValueType )
	{
		tvScopes->setCurrentIndex( mScopesProxy->mapFromSource( i.parent() ) );
		lvContents->setRootIndex( mContentProxy->mapFromSource( i.parent() ) );
		lvContents->setCurrentIndex( mContentProxy->mapFromSource( i ) );
	}
	else
	{
		tvScopes->setCurrentIndex( mScopesProxy->mapFromSource( i ) );
		lvContents->setRootIndex( mContentProxy->mapFromSource( i ) );
	}
}

void UIQMakeProjectSettings::querySettings()
{
	// set maximum progressbar value
	pbProgress->setMaximum( cbScopes->count() *cbOperators->count() );
	// read all possible combinaison settings
	for ( int sc = 0; sc < cbScopes->count(); sc++ )
	{
		const QString Scope = cbScopes->itemText( sc );
		for ( int oc = 0; oc < cbOperators->count(); oc++ )
		{
			const QString Operator = cbOperators->itemText( oc );
			// set current scope in progressbar
			pbProgress->setFormat( tr( "scanning scope %1, operator %2 : %p%" ).arg( Scope, Operator ) );
			// update progress bar
			pbProgress->setValue( pbProgress->value() +1 );
			// process application event except user inputContext
			QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
			// set key
			const QString key = QString( "%1|%2|%3" ).arg( Scope ).arg( Operator );
			// check application page
			mOriginalSettings[ key.arg( leTitle->statusTip() ) ] = mProject->getListValues( leTitle->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( leIcon->statusTip() ) ] = mProject->getListValues( leIcon->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( leHelpFile->statusTip() ) ] = mProject->getListValues( leHelpFile->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( leAuthor->statusTip() ) ] = mProject->getListValues( leAuthor->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( cbTemplate->statusTip() ) ] = mProject->getListValues( cbTemplate->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( cbLanguage->statusTip() ) ] = mProject->getListValues( cbLanguage->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( lwCompilerFlags->statusTip() ) ] = mProject->getListValues( lwCompilerFlags->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( lwQtModules->statusTip() ) ] = mProject->getListValues( lwQtModules->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( gbVersion->statusTip() ) ] = mProject->getListValues( gbVersion->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( cbBuildAutoIncrement->statusTip() ) ] = mProject->getListValues( cbBuildAutoIncrement->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( cbEncodings->statusTip() ) ] = mProject->getListValues( cbEncodings->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( leOutputPath->statusTip() ) ] = mProject->getListValues( leOutputPath->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( leOutputName->statusTip() ) ] = mProject->getListValues( leOutputName->statusTip(), Operator, Scope );
			mOriginalSettings[ key.arg( leTranslationsPath->statusTip() ) ] = mProject->getListValues( leTranslationsPath->statusTip(), Operator, Scope );
			for ( int i = 0; i < cbVariables->count(); i++ )
				mOriginalSettings[ key.arg( cbVariables->itemText( i ) ) ] = mProject->getListValues( cbVariables->itemText( i ), Operator, Scope );
		}
	}
	// set settings original one
	mSettings = mOriginalSettings;
	// hide progressbar
	pbProgress->hide();
}

void UIQMakeProjectSettings::loadModules()
{
	// load modules informations
	QtItemList mModules = UISettingsQMake::readQtModules();
	// fill lwQtModules
	foreach ( QtItem* i, mModules )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwQtModules );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setData( QtItem::HelpRole, i->Help );
		it->setCheckState( Qt::Unchecked );
	}
	// free memory
	qDeleteAll( mModules );
}

void UIQMakeProjectSettings::loadConfigs()
{
	// load configs informations
	QtItemList mConfigs = UISettingsQMake::readSettings();
	// fill lwCompilerFlags
	QFont fo( lwCompilerFlags->font() );
	fo.setPointSize( fo.pointSize() +2 );
	fo.setBold( true );
	foreach ( QtItem* i, mConfigs )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwCompilerFlags );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setData( QtItem::HelpRole, i->Help );
		if ( it->text().toLower().contains( "only" ) )
		{
			it->setFlags( 0 );
			it->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
			it->setBackground( Qt::darkBlue );
			it->setForeground( Qt::white );
			it->setFont( fo );
		}
		else
			it->setCheckState( Qt::Unchecked );
	}
	// free memory
	qDeleteAll( mConfigs );
	mConfigs.clear();
}

void UIQMakeProjectSettings::loadLanguages()
{
	/*
	leTranslationsPath->setText( getStringValues( "TRANSLATIONS_PATH" ) );
	leTranslationsPath->setModified( false );
	QStringList t = mProject->getListValues( "TRANSLATIONS" );
	QListWidgetItem*  it;
	bool b = false;
	for ( int i = QLocale::C +1; i < QLocale::Zulu +1; i++ )
	{
		QString s = QLocale::languageToString( ( QLocale::Language )i );
		QString f = mTranslationMask.arg( projectName() ).arg( s );
		it = new QListWidgetItem( s, lwTranslations );
		it->setCheckState( t.contains( f, Qt::CaseInsensitive ) ? Qt::Checked : Qt::Unchecked );
		if ( !b )
			b = it->checkState() == Qt::Checked;
	}
	*/
}

void UIQMakeProjectSettings::setDir( const QString& s )
{ setDir( mDirs->index( s ) ); }

void UIQMakeProjectSettings::setDir( const QModelIndex& i )
{
	if ( !i.isValid() )
		return;
	QString p = QDir( mDirs->filePath( i ) ).canonicalPath();
	lvDirs->setRootIndex( mDirs->index( p ) );
	QString s = leDir->text();
	if ( s.endsWith( "/" ) )
		s.chop( 1 );
	if ( s != p )
	{
		leDir->setText( p );
		lwFiles->clear();
		QDir d( p );
		foreach ( s, d.entryList( QDir::Files, QDir::Name ) )
			lwFiles->addItem( new QListWidgetItem( mDirs->iconProvider()->icon( QFileIconProvider::File ), s ) );
	}
}

void UIQMakeProjectSettings::addValue( const QString& s )
{
	// check if value already exists
	if ( lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).count() )
		return;
	// check translations
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
	{
		// add project translation if needed
		// got language
		QString f = QFileInfo( s ).baseName().remove( mProject->name() +'_' );
		// look if it's a project translation
		if ( s == mTranslationMask.arg( mProject->name() ).arg( f ) )
		{
			// find item to check and check it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Checked );
		}
	}
	// add item
	lwValues->addItem( s );
}

void UIQMakeProjectSettings::editValue( const QString& s )
{
	// got item to edit, else return
	QListWidgetItem* it = lwValues->currentItem();
	if ( !it )
		return;
	// check if value already exists
	if ( lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).count() )
		return;
	// check translations
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
	{
		QString f;
		// remove the edited translation if needed
		// got language
		f = QFileInfo( it->text() ).baseName().remove( mProject->name() +'_' );
		// look if it's a project translation
		if ( it->text() == mTranslationMask.arg( mProject->name() ).arg( f ) )
		{
			// find item and uncheck it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Unchecked );
		}
		// add project translation if needed
		// got language
		f = QFileInfo( s ).baseName().remove( mProject->name() +'_' );
		// look if it's a project translation
		if ( s == mTranslationMask.arg( mProject->name() ).arg( f ) )
		{
			// find item to check and check it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Checked );
		}
	}
	// edit item
	it->setText( s );
}

void UIQMakeProjectSettings::removeValue( const QString& s )
{
	// got item to delete, else return
	QListWidgetItem* cit = s.isEmpty() ? lwValues->currentItem() : lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
	if ( !cit )
		return;
	// check translations
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
	{
		// got language
		QString f = QFileInfo( cit->text() ).baseName().remove( mProject->name() +'_' );
		// look if it's a project translation
		if ( cit->text() == mTranslationMask.arg( mProject->name() ).arg( f ) )
		{
			// find item and uncheck it
			QListWidgetItem* it = lwTranslations->findItems( f, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			if ( it )
				it->setCheckState( Qt::Unchecked );
		}
	}
	// delete item
	delete cit;
}

void UIQMakeProjectSettings::tb_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QString s;
	// Application
	if ( tb == tbIcon )
	{
		s = QFileDialog::getOpenFileName( this, tr( "Choose your application icon" ), mProject->canonicalFilePath( leIcon->text() ), tr( "Images (*.png *.xpm *.jpg *.ico)" ) );
		if ( !s.isEmpty() )
		{
			leIcon->setText( mProject->relativeFilePath( s ) );
			leIcon->setModified( true );
			lPixmap->setPixmap( QPixmap( s ) );
		}
	}
	else if ( tb == tbHelpFile )
	{
		s = QFileDialog::getOpenFileName( this, tr( "Choose your application help root file" ), mProject->canonicalFilePath( leHelpFile->text() ), tr( "HTMLs (*.htm *.html);;PDFs (*.pdf);;Windows Help Files (*.chm *.hlp)" ) );
		if ( !s.isEmpty() )
		{
			leHelpFile->setText( mProject->relativeFilePath( s ) );
			leHelpFile->setModified( true );
		}
	}
	else if ( tb == tbOutputPath )
	{
		s = QFileDialog::getExistingDirectory( this, tr( "Choose your application output path" ), mProject->canonicalFilePath( leOutputPath->text() ) );
		if ( !s.isEmpty() )
		{
			leOutputPath->setText( mProject->relativeFilePath( s ) );
			leOutputPath->setModified( true );
		}
	}
	else if ( tb == tbTranslationsPath )
	{
		s = QFileDialog::getExistingDirectory( this, tr( "Choose your translations path" ), mProject->canonicalFilePath( tbTranslationsPath->text() ) );
		if ( !s.isEmpty() )
		{
			leTranslationsPath->setText( mProject->relativeFilePath( s ) );
			leTranslationsPath->setModified( true );
		}
	}
}

void UIQMakeProjectSettings::sb_valueChanged( int )
{
	sbBuild->setValue( 0 );
}

void UIQMakeProjectSettings::on_cbTemplate_currentIndexChanged( const QString& s )
{
	// enable / disable widget according to template == subdirs
	bool b = s.toLower() == "subdirs";
	cbOrdered->setEnabled( b );
	leTitle->setDisabled( b );
	leIcon->setDisabled( b );
	tbIcon->setDisabled( b );
	leHelpFile->setDisabled( b );
	tbHelpFile->setDisabled( b );
	gbVersion->setDisabled( b );
	fBuildType->setDisabled( b );
	fWarn->setDisabled( b );
	cbBuildAll->setDisabled( b ? b : !rbDebugRelease->isChecked() );
	wLibraries->setDisabled( b );
	wSettings->setDisabled( b );
	wTranslations->setDisabled( b );
}

void UIQMakeProjectSettings::lw_currentItemChanged( QListWidgetItem* it, QListWidgetItem* )
{
	tbInformations->setHtml( it ? it->data( QtItem::HelpRole ).toString() : "" );
}

void UIQMakeProjectSettings::cb_highlighted( int )
{
	// backup all datas for current scope , operator
	QString s;
	QStringList l;
	const QString key = QString( "%1|%2|%3" ).arg( cbScopes->currentText() ).arg( cbOperators->currentText() );
	
	if ( leTitle->text() != mSettings[ key.arg( leTitle->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( leTitle->statusTip() ) ] = QStringList( leTitle->text() );
	if ( leIcon->text() != mSettings[ key.arg( leIcon->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( leIcon->statusTip() ) ] = QStringList( leIcon->text() );
	if ( leHelpFile->text() != mSettings[ key.arg( leHelpFile->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( leHelpFile->statusTip() ) ] = QStringList( leHelpFile->text() );
	if ( leAuthor->text() != mSettings[ key.arg( leAuthor->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( leAuthor->statusTip() ) ] = QStringList( leAuthor->text() );
	if ( cbTemplate->currentText() != mSettings[ key.arg( cbTemplate->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( cbTemplate->statusTip() ) ] = QStringList( cbTemplate->currentText() );
	if ( cbLanguage->currentText() != mSettings[ key.arg( cbLanguage->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( cbLanguage->statusTip() ) ] = QStringList( cbLanguage->currentText() );
	l = currentCONFIG();
	if ( l != mSettings[ key.arg( lwCompilerFlags->statusTip() ) ] )
		mSettings[ key.arg( lwCompilerFlags->statusTip() ) ] = l;
	l = currentQT();
	if ( l != mSettings[ key.arg( lwQtModules->statusTip() ) ] )
		mSettings[ key.arg( lwQtModules->statusTip() ) ] = l;
	s = QString( "%1.%2.%3.%4" ).arg( sbMajor->value() ).arg( sbMinor->value() ).arg( sbRelease->value() ).arg( sbBuild->value() );
	if ( s == "0.0.0.0" )
		s.clear();
	if ( s != mSettings[ key.arg( gbVersion->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( gbVersion->statusTip() ) ] = QStringList( QString( "%1.%2.%3.%4" ).arg( sbMajor->value() ).arg( sbMinor->value() ).arg( sbRelease->value() ).arg( sbBuild->value() ) );
	if ( !gbVersion->isChecked() )
		mSettings[ key.arg( gbVersion->statusTip() ) ].clear();
	if ( cbBuildAutoIncrement->isChecked() != mSettings[ key.arg( cbBuildAutoIncrement->statusTip() ) ].join( " " ).toInt() )
		mSettings[ key.arg( cbBuildAutoIncrement->statusTip() ) ] = QStringList( QString::number( cbBuildAutoIncrement->isChecked() ) );
	if ( !gbVersion->isChecked() )
		mSettings[ key.arg( cbBuildAutoIncrement->statusTip() ) ].clear();
	if ( cbEncodings->currentText() != mSettings[ key.arg( cbEncodings->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( cbEncodings->statusTip() ) ] = QStringList( cbEncodings->currentText() );
	if ( leOutputPath->text() != mSettings[ key.arg( leOutputPath->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( leOutputPath->statusTip() ) ] = QStringList( leOutputPath->text() );
	if ( leOutputName->text() != mSettings[ key.arg( leOutputName->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( leOutputName->statusTip() ) ] = QStringList( leOutputName->text() );
	if ( leTranslationsPath->text() != mSettings[ key.arg( leTranslationsPath->statusTip() ) ].join( " " ) )
		mSettings[ key.arg( leTranslationsPath->statusTip() ) ] = QStringList( leTranslationsPath->text() );
	l.clear();
	for ( int i = 0; i < lwValues->count(); i++ )
		l << lwValues->item( i )->text();
	if ( l != mSettings[ key.arg( cbVariables->currentText() ) ] )
		mSettings[ key.arg( cbVariables->currentText() ) ] = l;
}

void UIQMakeProjectSettings::on_cbScopes_currentIndexChanged( const QString& )
{ on_cbOperators_currentIndexChanged( cbOperators->currentText() ); }

void UIQMakeProjectSettings::on_cbOperators_currentIndexChanged( const QString& )
{
	QStringList l;
	const QString key = QString( "%1|%2|%3" ).arg( cbScopes->currentText() ).arg( cbOperators->currentText() );
	//
	leTitle->setText( mSettings[ key.arg( leTitle->statusTip() ) ].join( " " ) );
	leIcon->setText( mSettings[ key.arg( leIcon->statusTip() ) ].join( " " ) );
	lPixmap->setPixmap( QPixmap( mProject->canonicalFilePath( leIcon->text() ) ) );
	leHelpFile->setText( mSettings[ key.arg( leHelpFile->statusTip() ) ].join( " " ) );
	leAuthor->setText( mSettings[ key.arg( leAuthor->statusTip() ) ].join( " " ) );
	cbTemplate->setCurrentIndex( cbTemplate->findText( mSettings[ key.arg( cbTemplate->statusTip() ) ].join( " " ) ) );
	cbLanguage->setCurrentIndex( cbLanguage->findText( mSettings[ key.arg( cbLanguage->statusTip() ) ].join( " " ) ) );
	setCurrentCONFIG( mSettings[ key.arg( lwCompilerFlags->statusTip() ) ].join( " " ) );
	setCurrentQT( mSettings[ key.arg( lwQtModules->statusTip() ) ].join( " " ) );
	l = mSettings[ key.arg( gbVersion->statusTip() ) ];
	sbMajor->setValue( 0 );
	sbMinor->setValue( 0 );
	sbRelease->setValue( 0 );
	sbBuild->setValue( 0 );
	gbVersion->setChecked( !l.isEmpty() );
	if ( !l.isEmpty() )
	{
		l = l.value( 0 ).split( "." );
		sbMajor->setValue( l.value( 0 ).toInt() );
		sbMinor->setValue( l.value( 1 ).toInt() );
		sbRelease->setValue( l.value( 2 ).toInt() );
		sbBuild->setValue( l.value( 3 ).toInt() );
	}
	cbBuildAutoIncrement->setChecked( mSettings[ key.arg( cbBuildAutoIncrement->statusTip() ) ].join( " " ).toInt() );
	cbEncodings->setCurrentIndex( cbEncodings->findText( mSettings[ key.arg( cbEncodings->statusTip() ) ].join( " " ) ) );
	leOutputPath->setText( mSettings[ key.arg( leOutputPath->statusTip() ) ].join( " " ) );
	leOutputName->setText( mSettings[ key.arg( leOutputName->statusTip() ) ].join( " " ) );
	leTranslationsPath->setText( mSettings[ key.arg( leTranslationsPath->statusTip() ) ].join( " " ) );
	// load variables values
	on_cbVariables_currentIndexChanged( cbVariables->currentText() );
}

void UIQMakeProjectSettings::on_lwFiles_itemDoubleClicked( QListWidgetItem* i )
{
	if ( !i )
		return;
	QFileInfo f( leDir->text().append( "/" ).append( i->text() ) );
	QString s = f.completeSuffix().toLower();
	// TODO : add entry to qmake plugin so user can configure list himself
	QStringList libraries = QStringList() << "lib" << "dll" << "a" << "la" << "so" << "dylib";
	QString v = cbVariables->currentText().toLower();
	if ( v == "libs" && ( libraries.contains( s ) || s.startsWith( "so." ) ) )
	{
		// lib path
		s = mProject->relativeFilePath( f.path() );
		if ( s.contains( " " ) )
			s.prepend( '"' ).append( '"' );
		s.prepend( "-L" );
		addValue( s );
		// file
		s = f.baseName();
		if ( s.startsWith( "lib", Qt::CaseInsensitive ) )
			s.remove( 0, 3 );
		if ( s.contains( " " ) )
			s.prepend( '"' ).append( '"' );
		s.prepend( "-l" );
		addValue( s );
	}
	else if ( v == "defines" )
		addValue( f.baseName().toUpper() );
	else if ( v.contains( "path" ) )
		addValue( mProject->relativeFilePath( f.path() ) );
	else if ( ( v == "translations" && s == "ts" ) || ( v == "resources" && s == "qrc" ) || ( v == "def_file" && s == "def" ) || ( v == "rc_file" && s == "rc" ) || ( v == "res_file" && s == "res" ) )
		addValue( mProject->relativeFilePath( f.filePath() ) );
}

void UIQMakeProjectSettings::on_cbVariables_currentIndexChanged( const QString& s )
{
	lwValues->clear();
	QString k = QString( "%1|%2|%3" ).arg( cbScopes->currentText(), cbOperators->currentText(), s );
	lwValues->addItems( mSettings[k] );
}

void UIQMakeProjectSettings::on_pbAddValue_clicked()
{
	bool b;
	QString s = QInputDialog::getText( this, tr( "Add value..." ), tr( "Set your value content :" ), QLineEdit::Normal, QString::null, &b );
	if ( b && !s.isEmpty() )
		addValue( s );
}

void UIQMakeProjectSettings::on_pbEditValue_clicked()
{
	QListWidgetItem* it = lwValues->currentItem();
	if ( !it )
		return;
	bool b;
	QString s = QInputDialog::getText( this, tr( "Edit value..." ), tr( "Edit your value content :" ), QLineEdit::Normal, it->text(), &b );
	if ( b && !s.isEmpty() )
		editValue( s );
}

void UIQMakeProjectSettings::on_pbRemoveValue_clicked()
{ removeValue(); }

void UIQMakeProjectSettings::on_pbClearValues_clicked()
{
	if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
		while ( lwValues->count() )
			removeValue( lwValues->item( 0 )->text() );
	else
		lwValues->clear();
}

void UIQMakeProjectSettings::on_lwTranslations_itemChanged( QListWidgetItem* it )
{
	if ( !it )
		return;
	// if translations checked and no translations path, set one
	if ( leTranslationsPath->text().isEmpty() )
	{
		leTranslationsPath->setText( "translations" );
		leTranslationsPath->setModified( true );
	}
	if ( !lwTranslations->isVisible() )
		return;
	// get translation file
	QString s = mTranslationMask.arg( mProject->name() ).arg( it->text() );
	QString k = "|=|TRANSLATIONS";
	// check translation
	switch ( it->checkState() )
	{
		case Qt::Checked:
			if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
				lwValues->addItem( s );
			else
			{
				if ( !mSettings.contains( k ) )
					mSettings[ k ] = mProject->getListValues( "TRANSLATIONS" );
				if ( !mSettings.value( k ).contains( s, Qt::CaseInsensitive ) )
					mSettings[ k ] << s;
			}
			break;
		case Qt::Unchecked:
			if ( cbVariables->currentText().toLower() == "translations" && cbScopes->currentText().isEmpty() && cbOperators->currentText() == "=" )
				delete lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
			else
			{
				if ( !mSettings.contains( k ) )
					mSettings[ k ] = mProject->getListValues( "TRANSLATIONS" );
				if ( mSettings.value( k ).contains( s, Qt::CaseInsensitive ) )
					mSettings[ k ].removeAll( s );
			}
			break;
		default:
			break;
	}
}

void UIQMakeProjectSettings::on_tvScopes_clicked( const QModelIndex& i )
{
	// TODO : Fix me
	setCurrentIndex( mScopesProxy->mapToSource( i ) );
}

void UIQMakeProjectSettings::on_tvScopes_doubleClicked( const QModelIndex& i )
{
	if ( i.isValid() )
		UIItemSettings::edit( mModel, mModel->itemFromIndex( mScopesProxy->mapToSource( i ) ), this )->exec();
}

void UIQMakeProjectSettings::on_lvContents_doubleClicked( const QModelIndex& i )
{
	if ( i.isValid() )
		UIItemSettings::edit( mModel, mModel->itemFromIndex( mContentProxy->mapToSource( i ) ), this )->exec();
}

void UIQMakeProjectSettings::on_tbAdd_clicked()
{
	UIItemSettings::edit( mModel, 0, this )->exec();
}

void UIQMakeProjectSettings::on_tbEdit_clicked()
{
	QModelIndex i = currentIndex();
	if ( i.isValid() )
		UIItemSettings::edit( mModel, mModel->itemFromIndex( i ), this )->exec();
}

void UIQMakeProjectSettings::on_tbRemove_clicked()
{
	ProjectItem* it = mModel->itemFromIndex( currentIndex() );
	if ( it )
		it->remove();
}

void UIQMakeProjectSettings::on_tbClear_clicked()
{ mProject->removeRows( 0, mProject->rowCount() ); }

void UIQMakeProjectSettings::on_tbUp_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mModel->itemFromIndex( i )->moveUp() )
			setCurrentIndex( i.sibling( i.row() -1, i.column() ) );
}

void UIQMakeProjectSettings::on_tbDown_clicked()
{
	// got index to move
	QModelIndex i = currentIndex();
	// check if valid to move
	if ( i.isValid() )
		if ( mModel->itemFromIndex( i )->moveDown() )
			setCurrentIndex( i.sibling( i.row() +1, i.column() ) );
}

void UIQMakeProjectSettings::on_dbbButtons_helpRequested()
{
	QString s;
	switch ( lwMenu->currentRow() )
	{
		case 0:
			s = tr( "<b>Application</b>: Here you can configure global project settings." );
			break;
		case 1:
			s = tr( "<b>Libraries</b>: Here you can easily check the Qt module you want, and add differents configuration flag." );
			break;
		case 2:
			s = tr( "<b>Settings</b>: Here you can configure differents settings for many scopes and operators." );
			break;
		case 3:
			s = tr( "<b>Translations</b>: Here you can easily check the languages you want to provide for your project." );
			break;
		case 4:
			s = tr( "<b>Advanced</b>: This is a direct access to the project model, be carrefull while editing items as you can break your project." );
			break;
	}
	if ( !s.isEmpty() )
		QWhatsThis::showText( dbbButtons->button( QDialogButtonBox::Help )->mapToGlobal( QPoint( 0, 0 ) ), s );
}

void UIQMakeProjectSettings::accept()
{
	// backup current settings state
	cb_highlighted( 0 );
	// applications
	QStringList s, c;
	// if subdirs only fill these and return
	if ( !cbEncodings->currentText().isEmpty() )
		mProject->setStringValues( cbEncodings->currentText(), "ENCODING" );
	if ( !cbTemplate->currentText().isEmpty() )
		mProject->setStringValues( cbTemplate->currentText(), "TEMPLATE" );
	if ( !cbLanguage->currentText().isEmpty() )
		mProject->setStringValues( cbLanguage->currentText(), "LANGUAGE" );
	if ( leAuthor->isModified() )
		mProject->setStringValues( leAuthor->text(), "APP_AUTHOR" );
	if ( cbTemplate->currentText().toLower() == "subdirs" )
	{
		if ( cbOrdered->isChecked() )
			s << "ordered";
		mProject->setListValues( s, "CONFIG", "+=" );
		// close dialog
		QDialog::accept();
		return;
	}
	//
	if ( leTitle->isModified() )
		mProject->setStringValues( leTitle->text(), "APP_TITLE" );
	if ( leIcon->isModified() )
		mProject->setStringValues( leIcon->text(), "APP_ICON" );
	if ( leHelpFile->isModified() )
		mProject->setStringValues( leHelpFile->text(), "APP_HELP_FILE" );
	if ( gbVersion->isChecked() )
	{
		mProject->setStringValues( QString( "%1.%2.%3.%4" ).arg( sbMajor->value() ).arg( sbMinor->value() ).arg( sbRelease->value() ).arg( sbBuild->value() ), "VERSION" );
		mProject->setStringValues( QString::number( cbBuildAutoIncrement->isChecked() ), "APP_AUTO_INCREMENT" );
	}
	else
	{
		mProject->setStringValues( QString::null, "VERSION" );
		mProject->setStringValues( QString::null, "APP_AUTO_INCREMENT" );
	}
	// reading config variable
	if ( rbDebug->isChecked() )
		s << "debug";
	else if ( rbRelease->isChecked() )
		s << "release";
	else if ( rbDebugRelease->isChecked() )
	{
		s << "debug_and_release";
		if ( cbBuildAll->isChecked() )
			s << "build_all";
	}
	if ( rbWarnOff->isChecked() )
		s << "warn_off";
	else if ( rbWarnOn->isChecked() )
		s << "warn_on";
	// read qt modules / lwCompilerFlags
	QList<QListWidgetItem*> l = lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive );
	l << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive );
	foreach ( QListWidgetItem* it, l )
	{
		QString v = it->data( QtItem::VariableRole ).toString().toLower();
		if ( it->checkState() == Qt::Checked )
		{
			if ( v == "config" )
				s << it->data( QtItem::ValueRole ).toString();
			else if ( v == "qt" )
				c << it->data( QtItem::ValueRole ).toString();
		}		
	}
	// add other config
	if ( !leConfig->text().isEmpty() )
		s << leConfig->text().simplified().split( " " );
	mProject->setListValues( s, "CONFIG", "+=" );
	mProject->setListValues( c, "QT" );
	s.clear();
	c.clear();
	// translations
	if ( leTranslationsPath->isModified() )
		mProject->setStringValues( leTranslationsPath->text(), "TRANSLATIONS_PATH" );
	// settings
	foreach ( QString v, mSettings.keys() )
	{
		QStringList l = v.split( "|" );
		mProject->setListValues( mSettings.value( v ), l.at( 2 ), l.at( 1 ), l.at( 0 ) );
	}
	// close dialog
	QDialog::accept();
}

void UIQMakeProjectSettings::reject()
{
	setResult( QDialog::Rejected );
	close();
}
