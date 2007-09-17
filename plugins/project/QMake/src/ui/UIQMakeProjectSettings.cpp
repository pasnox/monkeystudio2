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
#include <QInputDialog>
#include <QFileIconProvider>
#include <QTextCodec>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QWhatsThis>

using namespace pMonkeyStudio;

QString mTranslationMask = "$$TRANSLATIONS_PATH/%2_%3.ts";

uint qHash( const Key& k )
{
	uint i = 0;
	foreach ( const QString s, k )
		i += qHash( s );
	return i;
}

UIQMakeProjectSettings::UIQMakeProjectSettings( ProjectItem* m, QWidget* p )
	: QDialog( p ), mInit( false ), mProject( m ), mModel( m->model() ), mDirs( new QDirModel( this ) )
{
	Q_ASSERT( mProject != 0 );
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	// set window title
	setWindowTitle( QString( "Project Settings - %1" ).arg( mProject->name() ) );
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
	mScopesProxy = new QMakeProxy( mModel, mProject );
	mScopesProxy->setFilterRoles( QList<int>() << ProjectsModel::ValueType );
	mScopesProxy->setFiltering( true );
	tvScopes->setModel( mScopesProxy );
	//
	tvScopes->header()->hide();
	tvScopes->setRootIndex( mScopesProxy->mapFromSource( mProject->index() ) );
	// scope content
	mContentProxy = new QMakeProxy( mModel, mProject );
	mContentProxy->setFilterRoles( QList<int>() << ProjectsModel::ValueType );
	mContentProxy->setNegateFilter( false );
	mContentProxy->setFiltering( true );
	lvContents->setModel( mContentProxy );
	// set currentindex
	setCurrentIndex( mProject->child( 0, 0 )->index() );
}

void UIQMakeProjectSettings::showEvent( QShowEvent* )
{
	if ( !mInit )
	{
		mInit = true;
		QTimer::singleShot( 0, this, SLOT( querySettings() ) );
	}
}

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

void UIQMakeProjectSettings::setCurrentTRANSLATIONS( const QStringList& l )
{
	foreach ( QListWidgetItem* it, lwTranslations->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		it->setCheckState( l.contains( mTranslationMask.arg( mProject->name(), it->text() ), Qt::CaseInsensitive ) ? Qt::Checked : Qt::Unchecked );
}

const QString UIQMakeProjectSettings::checkTranslationsPath()
{
	const Key k = Key() << "" << "=" << "TRANSLATIONS_PATH";
	QString c = value( k );
	if ( c.isEmpty() )
	{
		c = "translations";
		if ( currentKey( "TRANSLATIONS_PATH" ) == k && leTranslationsPath->text().isEmpty() )
			leTranslationsPath->setText( c );
		else
			setValue( k, c );
	}
	
	// return translations path
	return c;
}

Key UIQMakeProjectSettings::currentKey( const QString& s ) const
{ return Key() << cbScopes->currentText() << cbOperators->currentText() << s ; }

void UIQMakeProjectSettings::addValue( const Key& k, const QString& v )
{
	if ( !mSettings.value( k ).contains( v ) && !v.isEmpty() )
		mSettings[k] << v;
}

void UIQMakeProjectSettings::addValues( const Key& k, const QStringList& v )
{
	foreach ( const QString s, v )
		addValue( k, s );
}

void UIQMakeProjectSettings::setValue( const Key& k, const QString& v )
{
	if ( !mSettings.contains( k ) && v.isEmpty() )
		return;
	mSettings[k] = v.isEmpty() ? QStringList() : QStringList( v );
}

void UIQMakeProjectSettings::setValues( const Key& k, const QStringList& v )
{
	if ( !mSettings.contains( k ) && v.isEmpty() )
		return;
	mSettings[k] = v;
}

void UIQMakeProjectSettings::removeValue( const Key& k, const QString& v )
{
	if ( mSettings.value( k ).contains( v ) )
		mSettings[k].removeAll( v );
}

void UIQMakeProjectSettings::removeValues( const Key& k, const QStringList& v )
{
	foreach ( const QString& s, v )
		removeValue( k, s );
}

void UIQMakeProjectSettings::clearValues( const Key& k )
{ mSettings.remove( k  ); }

QStringList UIQMakeProjectSettings::values( const Key& k ) const
{ return mSettings.value( k ); }

QString UIQMakeProjectSettings::value( const Key& k ) const
{ return mSettings.value( k ).join( " " ); }

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
	// load user operators
	cbOperators->addItems( UISettingsQMake::readOperators() );
	
	// get all scope of project
	QStringList l = mProject->projectScopesList();
	
	// set maximum progressbar value
	pbProgress->setMaximum( l.count() *cbOperators->count() );
	
	// scan values scopes
	foreach ( QString Scope, l )
	{
		// add scope to scope list
		if ( cbScopes->findText( Scope ) == -1 )
			cbScopes->addItem( Scope );
		
		// set current scope in progressbar
		pbProgress->setFormat( tr( "Reading project, scope %2 : %p%" ).arg( Scope ) );
		
		// process application event except user inputContext
		QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
		
		// browse all possible operator for scope
		for ( int oc = 0; oc < cbOperators->count(); oc++ )
		{
			const QString Operator = cbOperators->itemText( oc );
			
			// update progress bar
			pbProgress->setValue( pbProgress->value() +1 );
			
			// set key
			const Key k = Key() << Scope << Operator;
			
			// check application page
			mOriginalSettings[ Key( k ) << leTitle->statusTip() ] = mProject->getListValues( leTitle->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << leIcon->statusTip() ] = mProject->getListValues( leIcon->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << leHelpFile->statusTip() ] = mProject->getListValues( leHelpFile->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << leAuthor->statusTip() ] = mProject->getListValues( leAuthor->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << cbTemplate->statusTip() ] = mProject->getListValues( cbTemplate->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << cbLanguage->statusTip() ] = mProject->getListValues( cbLanguage->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << lwCompilerFlags->statusTip() ] = mProject->getListValues( lwCompilerFlags->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << lwQtModules->statusTip() ] = mProject->getListValues( lwQtModules->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << gbVersion->statusTip() ] = mProject->getListValues( gbVersion->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << cbBuildAutoIncrement->statusTip() ] = mProject->getListValues( cbBuildAutoIncrement->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << cbEncodings->statusTip() ] = mProject->getListValues( cbEncodings->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << leOutputPath->statusTip() ] = mProject->getListValues( leOutputPath->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << leOutputName->statusTip() ] = mProject->getListValues( leOutputName->statusTip(), Operator, Scope );
			mOriginalSettings[ Key( k ) << leTranslationsPath->statusTip() ] = mProject->getListValues( leTranslationsPath->statusTip(), Operator, Scope );
			for ( int i = 0; i < cbVariables->count(); i++ )
				mOriginalSettings[ Key( k ) << cbVariables->itemText( i ) ] = mProject->getListValues( cbVariables->itemText( i ), Operator, Scope );
		}
	}
	
	// set settings original one
	mSettings = mOriginalSettings;
	
	// fill list & combo
	cbScopes->addItems( UISettingsQMake::readScopes() );
	cbEncodings->addItems( availableTextCodecs() );
	loadModules();
	loadConfigs();
	loadLanguages();
	
	// hide progressbar
	pbProgress->hide();
	
	// apply current settings
	on_cbOperators_currentIndexChanged( cbOperators->currentText() );
	
	// connections
	// combox
	connect( cbScopes, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( cbOperators, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	connect( cbVariables, SIGNAL( highlighted( int ) ), this, SLOT( cb_highlighted( int ) ) );
	// buttons
	connect( tbIcon, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( tbHelpFile, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( tbOutputPath, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	connect( tbTranslationsPath, SIGNAL( clicked() ), this, SLOT( tb_clicked() ) );
	// spinbox
	connect( sbMajor, SIGNAL( valueChanged( int ) ), this, SLOT( sb_valueChanged( int ) ) );
	connect( sbMinor, SIGNAL( valueChanged( int ) ), this, SLOT( sb_valueChanged( int ) ) );
	connect( sbRelease, SIGNAL( valueChanged( int ) ), this, SLOT( sb_valueChanged( int ) ) );
	// others
	connect( lwQtModules, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwCompilerFlags, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( leDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( setDir( const QString& ) ) );
	connect( lvDirs, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( setDir( const QModelIndex& ) ) );
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
}

void UIQMakeProjectSettings::loadLanguages()
{
	QListWidgetItem*  it;
	for ( int i = QLocale::C +1; i < QLocale::Zulu +1; i++ )
	{
		QString s = QLocale::languageToString( ( QLocale::Language )i );
		it = new QListWidgetItem( s, lwTranslations );
		it->setCheckState( Qt::Unchecked );
	}
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
	if ( !lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).count() )
		lwValues->addItem( s );
}

void UIQMakeProjectSettings::editValue( const QString& s )
{
	QListWidgetItem* it = lwValues->selectedItems().value( 0 );
	if ( it && !lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).count() )
		it->setText( s );
}

void UIQMakeProjectSettings::removeValue( const QString& s )
{ delete ( s.isEmpty() ? lwValues->selectedItems().value( 0 ) : lwValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 ) ); }

void UIQMakeProjectSettings::on_tbUndo_clicked()
{ on_cbOperators_currentIndexChanged( cbOperators->currentText() ); }

void UIQMakeProjectSettings::cb_highlighted( int )
{
	setValue( currentKey( leTitle->statusTip() ), leTitle->text() );
	setValue( currentKey( leIcon->statusTip() ), leIcon->text() );
	setValue( currentKey( leHelpFile->statusTip() ), leHelpFile->text() );
	setValue( currentKey( leAuthor->statusTip() ), leAuthor->text() );
	setValue( currentKey( cbTemplate->statusTip() ), cbTemplate->currentText() );
	setValue( currentKey( cbLanguage->statusTip() ), cbLanguage->currentText() );
	setValues( currentKey( lwCompilerFlags->statusTip() ), currentCONFIG() );
	setValues( currentKey( lwQtModules->statusTip() ), currentQT() );
	QString s = QString( "%1.%2.%3.%4" ).arg( sbMajor->value() ).arg( sbMinor->value() ).arg( sbRelease->value() ).arg( sbBuild->value() );
	setValue( currentKey( gbVersion->statusTip() ), s != "0.0.0.0" ? s : "" );
	if ( !gbVersion->isChecked() )
		clearValues( currentKey( gbVersion->statusTip() ) );
	setValue( currentKey( cbBuildAutoIncrement->statusTip() ), cbBuildAutoIncrement->isChecked() ? "1" : "" );
	if ( !gbVersion->isChecked() )
		clearValues( currentKey( cbBuildAutoIncrement->statusTip() ) );
	setValue( currentKey( cbEncodings->statusTip() ), cbEncodings->currentText() );
	setValue( currentKey( leOutputPath->statusTip() ), leOutputPath->text() );
	setValue( currentKey( leOutputName->statusTip() ), leOutputName->text() );
	setValue( currentKey( leTranslationsPath->statusTip() ), leTranslationsPath->text() );
	QStringList l;
	for ( int i = 0; i < lwValues->count(); i++ )
		l << lwValues->item( i )->text();
	setValues( currentKey( cbVariables->currentText() ), l );
}

void UIQMakeProjectSettings::tb_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QString s;
	
	if ( tb == tbIcon )
	{
		s = getImageFileName( tr( "Choose your application icon" ), mProject->canonicalFilePath( leIcon->text() ), this );
		if ( !s.isEmpty() )
		{
			leIcon->setText( mProject->relativeFilePath( s ) );
			lPixmap->setPixmap( QPixmap( s ) );
		}
	}
	else if ( tb == tbHelpFile )
	{
		s = getOpenFileName( tr( "Choose your application help root file" ), mProject->canonicalFilePath( leHelpFile->text() ), tr( "HTMLs (*.htm *.html);;PDFs (*.pdf);;Windows Help Files (*.chm *.hlp)" ), this );
		if ( !s.isEmpty() )
			leHelpFile->setText( mProject->relativeFilePath( s ) );
	}
	else if ( tb == tbOutputPath )
	{
		s = getExistingDirectory( tr( "Choose your application output path" ), mProject->canonicalFilePath( leOutputPath->text() ), this );
		if ( !s.isEmpty() )
			leOutputPath->setText( mProject->relativeFilePath( s ) );
	}
	else if ( tb == tbTranslationsPath )
	{
		s = getExistingDirectory( tr( "Choose your translations path" ), mProject->canonicalFilePath( tbTranslationsPath->text() ), this );
		if ( !s.isEmpty() )
			leTranslationsPath->setText( mProject->relativeFilePath( s ) );
	}
}

void UIQMakeProjectSettings::sb_valueChanged( int )
{ sbBuild->setValue( 0 ); }

void UIQMakeProjectSettings::on_cbScopes_currentIndexChanged( const QString& )
{ on_cbOperators_currentIndexChanged( cbOperators->currentText() ); }

void UIQMakeProjectSettings::on_cbOperators_currentIndexChanged( const QString& )
{	
	leTitle->setText( value( currentKey( leTitle->statusTip() ) ) );
	leIcon->setText( value( currentKey( leIcon->statusTip() ) ) );
	lPixmap->setPixmap( QPixmap( mProject->canonicalFilePath( leIcon->text() ) ) );
	leHelpFile->setText( value( currentKey( leHelpFile->statusTip() ) ) );
	leAuthor->setText( value( currentKey( leAuthor->statusTip() ) ) );
	cbTemplate->setCurrentIndex( cbTemplate->findText( value( currentKey( cbTemplate->statusTip() ) ) ) );
	cbLanguage->setCurrentIndex( cbLanguage->findText( value( currentKey( cbLanguage->statusTip() ) ) ) );
	setCurrentCONFIG( value( currentKey( lwCompilerFlags->statusTip() ) ) );
	setCurrentQT( value( currentKey( lwQtModules->statusTip() ) ) );
	sbMajor->setValue( 0 );
	sbMinor->setValue( 0 );
	sbRelease->setValue( 0 );
	sbBuild->setValue( 0 );
	const QString s = value( currentKey( gbVersion->statusTip() ) );
	gbVersion->setChecked( !s.isEmpty() );
	if ( !s.isEmpty() )
	{
		sbMajor->setValue( s.section( '.', 0, 0 ).toInt() );
		sbMinor->setValue( s.section( '.', 1, 1 ).toInt() );
		sbRelease->setValue( s.section( '.', 2, 2 ).toInt() );
		sbBuild->setValue( s.section( '.', 3, 3 ).toInt() );
	}
	cbBuildAutoIncrement->setChecked( value( currentKey( cbBuildAutoIncrement->statusTip() ) ).toInt() );
	cbEncodings->setCurrentIndex( cbEncodings->findText( value( currentKey( cbEncodings->statusTip() ) ) ) );
	leOutputPath->setText( value( currentKey( leOutputPath->statusTip() ) ) );
	leOutputName->setText( value( currentKey( leOutputName->statusTip() ) ) );
	leTranslationsPath->setText( value( currentKey( leTranslationsPath->statusTip() ) ) );
	
	// update variable values
	on_cbVariables_currentIndexChanged( cbVariables->currentText() );
	
	// update translations
	setCurrentTRANSLATIONS( values( currentKey( lwTranslations->statusTip() ) ) );
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
{ tbInformations->setHtml( it ? it->data( QtItem::HelpRole ).toString() : "" ); }

void UIQMakeProjectSettings::on_lwFiles_itemDoubleClicked( QListWidgetItem* i )
{
	if ( !i )
		return;
	
	// get fileinfo
	QFileInfo f( leDir->text().append( "/" ).append( i->text() ) );
	
	// get file suffix
	QString s = f.completeSuffix().toLower();
	
	// get librairies extension
	QStringList libraries = UISettingsQMake::readLibExtensions();
	
	// get current variable
	QString v = cbVariables->currentText().toLower();
	
	// for stocking values to add
	QStringList l;
	
	// switch extension
	if ( libraries.contains( s ) || s.startsWith( "so." ) )
	{
		// lib path
		s = mProject->relativeFilePath( f.path() );
		if ( s.contains( " " ) )
			s.prepend( '"' ).append( '"' );
		s.prepend( "-L" );
		l << s;
		
		// file
		s = f.baseName();
		if ( s.startsWith( "lib", Qt::CaseInsensitive ) )
			s.remove( 0, 3 );
		if ( s.contains( " " ) )
			s.prepend( '"' ).append( '"' );
		s.prepend( "-l" );
		l << s;
		
		if ( v != "libs" )
			addValues( currentKey( "LIBS" ), l );
		else
			foreach ( QString c, l )
				addValue( c );
	}
	else if ( s == "ts" )
	{
		// set default translations path if needed
		const QString p = checkTranslationsPath();
		
		// set file according to translations path
		s = mProject->relativeFilePath( f.filePath() );
		if ( s.startsWith( p ) )
		{
			s.remove( p );
			if ( s.startsWith( "/" ) )
				s.chop( 0 );
			s.prepend( "$$TRANSLATIONS_PATH/" );
		}
		
		if ( v == "translations" )
			addValue( s );
		else
			addValue( currentKey( "TRANSLATIONS" ), s );
	}
	else if ( s == "qrc" )
	{
		if ( v == "resources" )
			addValue( mProject->relativeFilePath( f.filePath() ) );
		else
			addValue( currentKey( "RESOURCES" ), mProject->relativeFilePath( f.filePath() ) );
	}
	else if ( s == "def" )
	{
		if ( v == "def_file" )
			addValue( mProject->relativeFilePath( f.filePath() ) );
		else
			addValue( currentKey( "DEF_FILE" ), mProject->relativeFilePath( f.filePath() ) );
	}
	else if ( s == "rc" )
	{
		if ( v == "rc_file" )
			addValue( mProject->relativeFilePath( f.filePath() ) );
		else
			addValue( currentKey( "RC_FILE" ), mProject->relativeFilePath( f.filePath() ) );
	}
	else if ( s == "res" )
	{
		if ( v == "res_file" )
			addValue( mProject->relativeFilePath( f.filePath() ) );
		else
			addValue( currentKey( "RES_FILE" ), mProject->relativeFilePath( f.filePath() ) );
	}
	else if ( v.contains( "path" ) )
		addValue( mProject->relativeFilePath( f.path() ) );
}

void UIQMakeProjectSettings::on_cbVariables_currentIndexChanged( const QString& s )
{
	lwValues->clear();
	lwValues->addItems( values( currentKey( s ) ) );
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
	if ( QListWidgetItem* it = lwValues->selectedItems().value( 0 ) )
	{
		bool b;
		QString s = QInputDialog::getText( this, tr( "Edit value..." ), tr( "Edit your value content :" ), QLineEdit::Normal, it->text(), &b );
		if ( b && !s.isEmpty() && it->text() != s )
			editValue( s );
	}
}

void UIQMakeProjectSettings::on_pbRemoveValue_clicked()
{ removeValue(); }

void UIQMakeProjectSettings::on_pbClearValues_clicked()
{ lwValues->clear(); }

void UIQMakeProjectSettings::on_lwTranslations_itemChanged( QListWidgetItem* it )
{
	if ( !it )
		return;
	
	// get key
	const Key k = currentKey( "TRANSLATIONS" );
	
	// set default translatin path if needed
	checkTranslationsPath();
	
	// get translation file
	QString s = mTranslationMask.arg( mProject->name() ).arg( it->text() );
	
	// check translation
	switch ( it->checkState() )
	{
		case Qt::Checked:
			if ( cbVariables->currentText().toLower() == "translations" )
				addValue( s );
			else
				addValue( k, s );
			break;
		case Qt::Unchecked:
			if ( cbVariables->currentText().toLower() == "translations" )
				removeValue( s );
			else
				removeValue( k, s );
			break;
		default:
			break;
	}
}

void UIQMakeProjectSettings::on_tvScopes_clicked( const QModelIndex& i )
{ setCurrentIndex( mScopesProxy->mapToSource( i ) ); }

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
	if ( ProjectItem* it = mModel->itemFromIndex( currentIndex() ) )
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
		QWhatsThis::showText( mapToGlobal( geometry().center() ), s );
}

void UIQMakeProjectSettings::accept()
{
	// backup current settings state
	cb_highlighted( 0 );
	
	// write each key if needed
	if ( mSettings != mOriginalSettings )
		foreach ( const Key k, mSettings.keys() )
			if ( k.value( 2 ).toLower() != "config" )
				mProject->setListValues( mSettings[k], k.value( 2 ), k.value( 1 ), k.value( 0 ) );
	
	// close dialog
	QDialog::accept();
}

void UIQMakeProjectSettings::reject()
{
	setResult( QDialog::Rejected );
	close();
}
