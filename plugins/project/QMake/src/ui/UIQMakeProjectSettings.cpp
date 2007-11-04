/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIQMakeProjectSettings.cpp
** Date      : 2007-11-04T22:53:37
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "UIQMakeProjectSettings.h"
#include "QMakeProxy.h"
#include "ProjectsModel.h"
#include "QMakeItem.h"
#include "UIItemSettings.h"
#include "pMonkeyStudio.h"
#include "PluginsManager.h"

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

uint qHash( const ProjectKey& k )
{ return qHash( (QStringList() << ( k.getScope() ? k.getScope()->getValue() : QString::null ) << k.getVariable() << k.getOperator() ).join( QString::null ) ); }

UIQMakeProjectSettings::UIQMakeProjectSettings( ProjectItem* m, QWidget* p )
	: QDialog( p )
{
	Q_ASSERT( m );
	mInit = false;
	mProject = m;
	mModel = m->model();
	mDirs = new QDirModel( this );
	//
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	// set window title
	setWindowTitle( QString( "Project Settings - %1" ).arg( mProject->name() ) );
	// set button pixmap
	dbbButtons->button( QDialogButtonBox::Ok )->setIcon( QPixmap( ":/icons/icons/ok.png" ) );
	dbbButtons->button( QDialogButtonBox::Cancel )->setIcon( QPixmap( ":/icons/icons/cancel.png" ) );
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QPixmap( ":/icons/icons/help.png" ) );
	// scopes proxy
	mScopesProxy = new QMakeProxy( mModel, mProject );
	mScopesProxy->setFilterRoles( QList<int>() << ProjectItem::ProjectType << ProjectItem::ScopeType << ProjectItem::NestedScopeType );
	mScopesProxy->setNegateFilter( false );
	mScopesProxy->setFiltering( true );
	cbScopes->setModel( mScopesProxy );
	cbScopes->view()->setRootIndex( mScopesProxy->mapFromSource( mProject->index().parent() ) );
	cbScopes->setCurrentIndex( mScopesProxy->mapFromSource( mProject->index() ) );
	//
	mDirs->setReadOnly( true );
	mDirs->setFilter( QDir::AllDirs );
	mDirs->setSorting( QDir::Name );
	leDir->setCompleter( new QCompleter( mDirs, leDir ) );
	lvDirs->setModel( mDirs );
#ifndef Q_OS_WIN
	// all os except windows got only one root drive, so don't need to show root
	lvDirs->setRootIndex( mDirs->index( "/" ) );
#endif
	setDir( mDirs->index( mProject->canonicalPath() ) );
	// advanced page
	mVariablesProxy = new QMakeProxy( mModel, mProject );
	mVariablesProxy->setFilterRoles( QList<int>() << ProjectItem::ValueType );
	mVariablesProxy->setFiltering( true );
	tvScopes->setModel( mVariablesProxy );
	//
	tvScopes->header()->hide();
	tvScopes->setRootIndex( mVariablesProxy->mapFromSource( mProject->index() ) );
	// scope content
	mContentProxy = new QMakeProxy( mModel, mProject );
	mContentProxy->setFilterRoles( QList<int>() << ProjectItem::ValueType );
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
			if ( it->data( QtItem::VariableRole ).toString() == lwCompilerFlags->statusTip() )
				l << it->data( QtItem::ValueRole ).toString();
	if ( !leConfig->text().isEmpty() )
		l << leConfig->text();
	return l.isEmpty() ? l : QStringList( l.join( " " ) );
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
		if ( it->data( QtItem::VariableRole ).toString() == lwCompilerFlags->statusTip() )
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
			if ( it->data( QtItem::VariableRole ).toString() == lwQtModules->statusTip() )
				l << it->data( QtItem::ValueRole ).toString();
	return l;
}

void UIQMakeProjectSettings::setCurrentQT( const QString& s )
{
	foreach ( QListWidgetItem* it, lwQtModules->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) << lwCompilerFlags->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		if ( it->data( QtItem::VariableRole ).toString() == lwQtModules->statusTip() )
			it->setCheckState( s.contains( it->data( QtItem::ValueRole ).toString(), Qt::CaseInsensitive ) ? Qt::Checked : Qt::Unchecked );
}

void UIQMakeProjectSettings::setCurrentTRANSLATIONS( const QStringList& l )
{
	foreach ( QListWidgetItem* it, lwTranslations->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		it->setCheckState( l.contains( mTranslationMask.arg( mProject->name(), it->text() ), Qt::CaseInsensitive ) ? Qt::Checked : Qt::Unchecked );
}

const QString UIQMakeProjectSettings::checkTranslationsPath()
{
	if ( cbTemplate->currentText() == "subdirs" )
		return QString();
	const ProjectKey k = ProjectKey( mProject, ProjectVariable( "TRANSLATIONS_PATH", "=" ) );
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

void UIQMakeProjectSettings::checkOthersVariables()
{
	// clear lists
	lwOthersVariables->clear();
	lwOthersValues->clear();
	// iterate other all variables
	ProjectKey ck = currentKey( QString::null );
	foreach ( ProjectKey k, mSettings.keys() )
	{
		// scope, operator
		if ( k.mScope == ck.getScope() && k.getOperator() == ck.getOperator() && !mBlackList.contains( k.getVariable() ) )
		{
			QListWidgetItem* it = new QListWidgetItem( k.getVariable() );
			it->setData( Qt::UserRole +1, values( k ) );
			it->setHidden( values( k ).isEmpty() );
			lwOthersVariables->addItem( it );
		}
	}
	// show values of first variable
	if ( lwOthersVariables->count() )
		lwOthersVariables->setCurrentItem( lwOthersVariables->item( 0 ) );
}

void UIQMakeProjectSettings::updateOthersValues()
{
	if ( QListWidgetItem* vit = lwOthersVariables->selectedItems().value( 0 ) )
	{
		QStringList l;
		foreach ( QListWidgetItem* it, lwOthersValues->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
			l << it->text();
		vit->setData( Qt::UserRole +1, l );
	}
}

ProjectKey UIQMakeProjectSettings::currentKey( const QString& s ) const
{ return ProjectKey( mModel->itemFromIndex( mScopesProxy->mapToSource( cbScopes->currentIndex() ) ), ProjectVariable( s, cbOperators->currentText() ) ); }

void UIQMakeProjectSettings::addValue( const ProjectKey& k, const QString& v )
{
	if ( !mSettings.value( k ).contains( v ) && !v.isEmpty() )
		mSettings[k] << v;
}

void UIQMakeProjectSettings::addValues( const ProjectKey& k, const QStringList& v )
{
	foreach ( const QString s, v )
		addValue( k, s );
}

void UIQMakeProjectSettings::setValue( const ProjectKey& k, const QString& v )
{
	if ( !mSettings.contains( k ) && v.isEmpty() )
		return;
	mSettings[k] = v.isEmpty() ? QStringList() : QStringList( v );
}

void UIQMakeProjectSettings::setValues( const ProjectKey& k, const QStringList& v )
{
	if ( !mSettings.contains( k ) && v.isEmpty() )
		return;
	mSettings[k] = v;
}

void UIQMakeProjectSettings::removeValue( const ProjectKey& k, const QString& v )
{
	if ( mSettings.value( k ).contains( v ) )
		mSettings[k].removeAll( v );
}

void UIQMakeProjectSettings::removeValues( const ProjectKey& k, const QStringList& v )
{
	foreach ( const QString& s, v )
		removeValue( k, s );
}

void UIQMakeProjectSettings::clearValues( const ProjectKey& k )
{ mSettings.remove( k ); }

QStringList UIQMakeProjectSettings::values( const ProjectKey& k ) const
{ return mSettings.value( k ); }

QString UIQMakeProjectSettings::value( const ProjectKey& k ) const
{ return mSettings.value( k ).join( " " ); }

QModelIndex UIQMakeProjectSettings::currentIndex()
{
	QModelIndex i;
	if ( lvContents->selectionModel()->selectedIndexes().count() )
		i = mContentProxy->mapToSource( lvContents->selectionModel()->selectedIndexes().at( 0 ) );
	if ( !i.isValid() && tvScopes->selectionModel()->selectedIndexes().count() )
		i = mVariablesProxy->mapToSource( tvScopes->selectionModel()->selectedIndexes().at( 0 ) );
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
	else if ( i.data( ProjectItem::TypeRole ).toInt() == ProjectItem::ValueType )
	{
		tvScopes->setCurrentIndex( mVariablesProxy->mapFromSource( i.parent() ) );
		lvContents->setRootIndex( mContentProxy->mapFromSource( i.parent() ) );
		lvContents->setCurrentIndex( mContentProxy->mapFromSource( i ) );
	}
	else
	{
		tvScopes->setCurrentIndex( mVariablesProxy->mapFromSource( i ) );
		lvContents->setRootIndex( mContentProxy->mapFromSource( i ) );
	}
}

void UIQMakeProjectSettings::querySettings()
{
	// get black list variables
	foreach ( QWidget* w, findChildren<QWidget*>() )
		if ( !w->statusTip().isEmpty() )
			mBlackList << w->statusTip();
	// add variables list
	for ( int i = 0; i < cbVariables->count(); i++ )
		mBlackList << cbVariables->itemText( i );
	// add to blacklist filtered items
	foreach ( const QString s, UISettingsQMake::readFilters() )
		if ( !mBlackList.contains( s ) )
			mBlackList << s;
	// load global operators
	cbOperators->addItems( availableOperators() );
	// get all variable of project
	ProjectItemList il = mProject->getAllItemList( ProjectItem::VariableType, "*", "*" );
	// set maximum progressbar value
	pbProgress->setMaximum( il.count() -1 );
	// inform reading project
	pbProgress->setFormat( tr( "Reading project datas... %p%" ) );
	pbProgress->setValue( 0 );
	//
	foreach ( ProjectItem* v, il )
	{
		// process application event except user inputContext
		QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
		// get key
		ProjectKey k = ProjectKey( v->parent(), ProjectVariable( v->getValue(), v->getOperator() ) );
		// add values to key
		foreach ( ProjectItem* it, v->children() )
			addValue( k, it->getValue() );
		// update progress bar
		pbProgress->setValue( pbProgress->value() +1 );
	}
	// set settings original one
	mOriginalSettings = mSettings;
	// set some ui settings
	cbEncodings->addItems( availableTextCodecs() );
	loadModules();
	loadConfigs();
	loadLanguages();
	loadPlugins();
	// hide progressbar
	pbProgress->hide();
	// apply current settings
	on_cbOperators_currentIndexChanged( cbOperators->currentText() );
	// connections
	// combox
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
	// fill lwQtModules
	foreach ( QtItem i, UISettingsQMake::readQtModules() )
	{
		QListWidgetItem* it = new QListWidgetItem( i.Text, lwQtModules );
		it->setData( QtItem::ValueRole, i.Value );
		it->setData( QtItem::VariableRole, i.Variable );
		it->setData( QtItem::HelpRole, i.Help );
		it->setCheckState( Qt::Unchecked );
	}
}

void UIQMakeProjectSettings::loadConfigs()
{
	// fill lwCompilerFlags
	QFont fo( lwCompilerFlags->font() );
	fo.setBold( true );
	foreach ( QtItem i, UISettingsQMake::readSettings() )
	{
		QListWidgetItem* it = new QListWidgetItem( i.Text, lwCompilerFlags );
		it->setData( QtItem::ValueRole, i.Value );
		it->setData( QtItem::VariableRole, i.Variable );
		it->setData( QtItem::HelpRole, i.Help );
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

void UIQMakeProjectSettings::loadPlugins()
{
	// builders
	foreach ( BuilderPlugin* bp, PluginsManager::instance()->plugins<BuilderPlugin*>( PluginsManager::stAll ) )
	{
		cbBuilders->addItem( bp->infos().Name );
		swBuilders->addWidget( bp->settingsWidget() );
	}
	
	// choose project builder
	BuilderPlugin* bp = mProject->builder();
	cbBuilders->setCurrentIndex( cbBuilders->findText( bp ? bp->infos().Name : QString() ) );
	
	// compilers
	foreach ( CompilerPlugin* cp, PluginsManager::instance()->plugins<CompilerPlugin*>( PluginsManager::stAll ) )
	{
		cbCompilers->addItem( cp->infos().Name );
		swCompilers->addWidget( cp->settingsWidget() );
	}
	
	// choose project compiler
	CompilerPlugin* cp = mProject->compiler();
	cbCompilers->setCurrentIndex( cbCompilers->findText( cp ? cp->infos().Name : QString() ) );
	
	// debugger
	foreach ( DebuggerPlugin* dp, PluginsManager::instance()->plugins<DebuggerPlugin*>( PluginsManager::stAll ) )
	{
		cbDebuggers->addItem( dp->infos().Name );
		swDebuggers->addWidget( dp->settingsWidget() );
	}
	
	// choose project debugger
	DebuggerPlugin* dp = mProject->debugger();
	cbDebuggers->setCurrentIndex( cbDebuggers->findText( dp ? dp->infos().Name : QString() ) );
	
	// interpreters
	foreach ( InterpreterPlugin* ip, PluginsManager::instance()->plugins<InterpreterPlugin*>( PluginsManager::stAll ) )
	{
		cbInterpreters->addItem( ip->infos().Name );
		swInterpreters->addWidget( ip->settingsWidget() );
	}
	
	// choose project interpreter
	InterpreterPlugin* ip = mProject->interpreter();
	cbInterpreters->setCurrentIndex( cbInterpreters->findText( ip ? ip->infos().Name : QString() ) );
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

void UIQMakeProjectSettings::on_tbAddScope_clicked()
{
	bool b;
	// get new scope
	QString s = QInputDialog::getItem( this, tr( "Add Scope..." ), tr( "Enter the scope name :" ), UISettingsQMake::readScopes(), -1, true, &b );
	if ( b && !s.trimmed().isEmpty() )
	{
		// get current projectitem
		ProjectItem* cit = currentKey( QString::null ).getScope();
		// ask where to add item
		QMessageBox mb( QMessageBox::Question, tr( "Add Scope..." ), tr( "According to current scope, where i need to create scope ?" ), QMessageBox::NoButton, this );
		QPushButton* bChild = mb.addButton( tr( "&Child" ), QMessageBox::AcceptRole );
		QPushButton* bUnder = mb.addButton( tr( "&Under" ), QMessageBox::AcceptRole );
		QPushButton* bEnd = mb.addButton( tr( "&End" ), QMessageBox::AcceptRole );
		bUnder->setEnabled( cit != mProject );
		bEnd->setEnabled( cit != mProject );
		mb.addButton( QMessageBox::Cancel );
		// execute dialog
		mb.exec();
		// create scope
		ProjectItem* sit = new QMakeItem( ProjectItem::NestedScopeType );
		sit->setValue( s.trimmed() );
		sit->setOperator( ":" );
		// add scope to correct place
		if ( mb.clickedButton() == bChild )
			cit->appendRow( sit );
		else if ( mb.clickedButton() == bUnder )
			cit->parent()->insertRow( cit->row() +1, sit );
		else if ( mb.clickedButton() == bEnd )
			cit->parent()->appendRow( sit );
		else
		{
			delete sit;
			return;
		}
		// create end scope item
		(void) new QMakeItem( ProjectItem::ScopeEndType, sit );
		// transform parent nested scope to simple scope
		if ( !sit->parent()->isProject() && sit->parent()->isNested() )
		{
			sit->parent()->setType( ProjectItem::ScopeType );
			sit->parent()->setOperator( QString::null );
		}
		// change current index to new scope
		cbScopes->setCurrentIndex( mScopesProxy->mapFromSource( sit->index() ) );
		// update datas
		on_cbOperators_currentIndexChanged( cbOperators->currentText() );
	}
}

void UIQMakeProjectSettings::on_tbRemoveScope_clicked()
{
	// user confirmation
	if ( question( tr( "Remove Scope..." ), tr( "Are you sure you want to delete this scope" ) ) )
	{
		// get scope to remove
		ProjectItem* cs = currentKey( QString::null ).getScope();
		// can t remove root scope
		if ( cs == mProject )
		{
			warning( tr( "Remove Scope..." ), tr( "Can't remove root scope, aborting." ) );
			return;
		}
		// remove each variable that have scope s
		foreach ( const ProjectKey k, mSettings.keys() )
			if ( k.getScope() == cs )
				clearValues( k );
		// hide scope from combobox tree
		cbScopes->view()->setRowHidden( cbScopes->currentIndex().row(), cbScopes->currentIndex().parent(), true );
	}
}

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
	for ( int i = 0; i < lwOthersVariables->count(); i++ )
	{
		QListWidgetItem* it = lwOthersVariables->item( i );
		setValues( currentKey( it->text() ), it->data( Qt::UserRole +1 ).toStringList() );
	}
}

void UIQMakeProjectSettings::tb_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QString s;
	//
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

void UIQMakeProjectSettings::on_cbScopes_highlighted( const QModelIndex& )
{ cb_highlighted( 0 ); }

void UIQMakeProjectSettings::on_cbScopes_currentChanged( const QModelIndex& )
{ on_cbOperators_currentIndexChanged( cbOperators->currentText() ); }

void UIQMakeProjectSettings::on_cbOperators_currentIndexChanged( const QString& )
{
	// cancel if loading
	if ( pbProgress->isVisible() )
		return;
	//
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
	// update others variables
	checkOthersVariables();
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
	QStringList libraries =  QStringList() << "lib" << "dll" << "a" << "la" << "so" << "dylib";
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
		// add values
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
	const ProjectKey k = currentKey( "TRANSLATIONS" );
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

void UIQMakeProjectSettings::on_tbOthersVariablesAdd_clicked()
{
	bool b;
	const QString s = QInputDialog::getText( this, tr( "Add Variable..." ), tr( "Enter the name of the new variable :" ), QLineEdit::Normal, QString::null, &b );
	if ( b && !s.isEmpty() )
	{
		QListWidgetItem* it = lwOthersVariables->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
		if ( it && it->isHidden() )
			it->setHidden( false );
		else if ( !it )
			it = new QListWidgetItem( s, lwOthersVariables );
		lwOthersVariables->setCurrentItem( it );
	}
}

void UIQMakeProjectSettings::on_tbOthersVariablesEdit_clicked()
{
	if ( QListWidgetItem* it = lwOthersVariables->selectedItems().value( 0 ) )
	{
		bool b;
		const QString s = QInputDialog::getText( this, tr( "Edit Variable..." ), tr( "Enter a new name for this variable :" ), QLineEdit::Normal, it->text(), &b );
		if ( !b )
			return;
		QListWidgetItem* eit = lwOthersVariables->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).value( 0 );
		if ( eit && eit->isHidden() )
		{
			// show item and assigna datas
			eit->setData( Qt::UserRole +1, it->data( Qt::UserRole +1 ) );
			eit->setHidden( false );
			// clear data and hide item
			it->setData( Qt::UserRole +1, QStringList() );
			it->setHidden( true );
		}
		else
		{
			// create unvisible item with old name and no values
			eit = new QListWidgetItem( it->text(), lwOthersVariables );
			eit->setHidden( true );
			// rename current item
			it->setText( s );
		}
	}
}

void UIQMakeProjectSettings::on_tbOthersVariablesRemove_clicked()
{
	if ( QListWidgetItem* it = lwOthersVariables->selectedItems().value( 0 ) )
	{
		it->setData( Qt::UserRole +1, QStringList() );
		it->setHidden( true );
	}
}

void UIQMakeProjectSettings::on_lwOthersVariables_itemSelectionChanged()
{
	// clear values, and add item values
	lwOthersValues->clear();
	QListWidgetItem* it = lwOthersVariables->selectedItems().value( 0 );
	lwOthersValues->addItems( it ? it->data( Qt::UserRole +1 ).toStringList() : QStringList() );
}

void UIQMakeProjectSettings::on_tbOthersValuesAdd_clicked()
{
	if ( lwOthersVariables->selectedItems().value( 0 ) )
	{
		bool b;
		const QString s = QInputDialog::getText( this, tr( "Add Value..." ), tr( "Enter a value :" ), QLineEdit::Normal, QString::null, &b );
		if ( b && !s.isEmpty() && !lwOthersValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).count() )
		{
			lwOthersValues->addItem( s );
			updateOthersValues();
		}
	}
}

void UIQMakeProjectSettings::on_tbOthersValuesEdit_clicked()
{
	if ( lwOthersVariables->selectedItems().value( 0 ) )
		if ( QListWidgetItem* it = lwOthersValues->selectedItems().value( 0 ) )
		{
			bool b;
			const QString s = QInputDialog::getText( this, tr( "Edit Value..." ), tr( "Edit the value :" ), QLineEdit::Normal, it->text(), &b );
			if ( b && !s.isEmpty() && s != it->text() && !lwOthersValues->findItems( s, Qt::MatchFixedString | Qt::MatchRecursive ).count() )
			{
				it->setText( s );
				updateOthersValues();
			}
		}
}

void UIQMakeProjectSettings::on_tbOthersValuesRemove_clicked()
{
	if ( lwOthersVariables->selectedItems().value( 0 ) )
		if ( QListWidgetItem* it = lwOthersValues->selectedItems().value( 0 ) )
		{
			if ( question( tr( "Remove Value..." ), tr( "Remove value ?" ), this ) )
			{
				delete it;
				updateOthersValues();
			}
		}
}

void UIQMakeProjectSettings::on_tbOthersValuesClear_clicked()
{
	if ( lwOthersVariables->selectedItems().value( 0 ) )
	{
		lwOthersValues->clear();
		updateOthersValues();
	}
}

void UIQMakeProjectSettings::on_tbOthersValuesUp_clicked()
{
	if ( QListWidgetItem* it = lwOthersValues->selectedItems().value( 0 ) )
	{
		int i = lwOthersValues->row( it );
		if ( i != 0 )
			lwOthersValues->insertItem( i -1, lwOthersValues->takeItem( i ) );
		lwOthersValues->setCurrentItem( it );
		updateOthersValues();
	}
}

void UIQMakeProjectSettings::on_tbOthersValuesDown_clicked()
{
	if ( QListWidgetItem* it = lwOthersValues->selectedItems().value( 0 ) )
	{
		int i = lwOthersValues->row( it );
		if ( i != lwOthersValues->count() -1 )
			lwOthersValues->insertItem( i +1, lwOthersValues->takeItem( i ) );
		lwOthersValues->setCurrentItem( it );
		updateOthersValues();
	}
}

void UIQMakeProjectSettings::on_tvScopes_clicked( const QModelIndex& i )
{ setCurrentIndex( mVariablesProxy->mapToSource( i ) ); }

void UIQMakeProjectSettings::on_tvScopes_doubleClicked( const QModelIndex& i )
{
	if ( ProjectItem* it = mModel->itemFromIndex( mVariablesProxy->mapToSource( i ) ) )
		UIItemSettings::edit( it, this );
}

void UIQMakeProjectSettings::on_lvContents_doubleClicked( const QModelIndex& i )
{
	if ( ProjectItem* it = mModel->itemFromIndex( mVariablesProxy->mapToSource( i ) ) )
		UIItemSettings::edit( it, this );
}

void UIQMakeProjectSettings::on_tbAdd_clicked()
{
	// new item
	QMakeItem* it = new QMakeItem( ProjectItem::EmptyType );
	// get current item
	ProjectItem* ci = mModel->itemFromIndex( currentIndex() );
	// ask user where to create item
	if ( question( tr( "New Item.." ), tr( "Create item as a child of the current item ?" ) ) )
		ci = ci ? ci : mProject;
	else
		ci = ci ? ci->parent() : mProject;
	// append item
	ci->appendRow( it );
	// edit item
	if ( UIItemSettings::edit( it, this ) )
	{
		// if scope, add scope end
		if ( it->getType() == ProjectItem::ScopeType || it->getType() == ProjectItem::NestedScopeType )
			(void) new QMakeItem( ProjectItem::ScopeEndType, it );
		// redo layout
		it->refresh();
		// select it
		setCurrentIndex( it->index() );
	}
	// delete item
	else
		it->remove();
}

void UIQMakeProjectSettings::on_tbEdit_clicked()
{
	ProjectItem* it = mModel->itemFromIndex( currentIndex() );
	if ( it )
		UIItemSettings::edit( it, this );
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
	switch ( swMenu->currentIndex() )
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
			s = tr( "<b>Others Variables</b>: Here you can modify variables values that GUI don't provide." );
			break;
		case 5:
			s = tr( "<b>Advanced</b>: This is a direct access to the project model, be carrefull while editing items as you can break your project." );
			break;
	}
	if ( !s.isEmpty() )
		QWhatsThis::showText( mapToGlobal( rect().center() ), s );
}

void UIQMakeProjectSettings::accept()
{
	// backup current settings state
	cb_highlighted( 0 );
	// write each key if needed
	if ( mSettings != mOriginalSettings )
		foreach ( const ProjectKey k, mSettings.keys() )
				mProject->setValues( k.getScope(), k.getVariable(), k.getOperator(), mSettings[k] );
	// set plugins for projects
	mProject->setBuilder( PluginsManager::instance()->plugin<BuilderPlugin*>( PluginsManager::stAll, cbBuilders->currentText() ) );
	mProject->setCompiler( PluginsManager::instance()->plugin<CompilerPlugin*>( PluginsManager::stAll, cbCompilers->currentText() ) );
	mProject->setDebugger( PluginsManager::instance()->plugin<DebuggerPlugin*>( PluginsManager::stAll, cbDebuggers->currentText() ) );
	mProject->setInterpreter( PluginsManager::instance()->plugin<InterpreterPlugin*>( PluginsManager::stAll, cbInterpreters->currentText() ) );
	// close dialog
	QDialog::accept();
}

void UIQMakeProjectSettings::reject()
{
	// backup current settings state
	cb_highlighted( 0 );
	setResult( QDialog::Rejected );
	close();
}
