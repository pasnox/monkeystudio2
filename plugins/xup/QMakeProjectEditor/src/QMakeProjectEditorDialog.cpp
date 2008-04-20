#include "QMakeProjectEditorDialog.h"

#include <QLocale>
#include <QHeaderView>

#include <coremanager.h>
#include <pluginsmanager.h>

const QString QtTranslationMask( "translations/%1.ts" );
//wProjectFiles
QMakeProjectEditorDialog::QMakeProjectEditorDialog( XUPItem* project, QWidget* parent )
	: UIXUPProjectEditor( project, parent )
{
	lOperator->setVisible( true );
	cbOperator->setVisible( true );
	// init Qt configuration widget
	mQtConfigurationPage = new QWidget( this );
	Ui::UIQtConfiguration::setupUi( mQtConfigurationPage );
	// insert qt conf page into project settings editor
	insertPage( 0, mQtConfigurationPage );
	cbPages->setCurrentIndex( 0 );
	// get qt version of project
	mQtVersion = mQtManager.version( mProject->projectSettingsValue( "QT_VERSION" ) );
	// create views
	createQtModulesConfigurations();
	createLanguages();
	// load qt version
	loadsQtVersions();
	// load qt configuration
	loadsQtConfigurations();
	// connections
	connect( this, SIGNAL( currentScopeChanged( XUPItem* ) ), this, SLOT( currentScopeChanged( XUPItem* ) ) );
	connect( this, SIGNAL( currentOperatorChanged( const QString& ) ), this, SLOT( currentOperatorChanged( const QString& ) ) );
	connect( lwQtVersions, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lwQtVersions_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lQtVersionInformations, SIGNAL( linkActivated( const QString& ) ), this, SLOT( lQtVersionInformations_linkActivated( const QString& ) ) );
	connect( cbTemplate, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( cbTemplate_currentIndexChanged( const QString& ) ) );
	foreach ( QAbstractButton* ab, gbBuild->findChildren<QAbstractButton*>() )
		connect( ab, SIGNAL( clicked() ), this, SLOT( gbBuild_changed() ) );
	foreach ( QLineEdit* le, gbApplication->findChildren<QLineEdit*>() )
		connect( le, SIGNAL( textChanged( const QString& ) ), this, SLOT( gbApplication_changed() ) );
	foreach ( QLineEdit* le, gbOutput->findChildren<QLineEdit*>() )
		connect( le, SIGNAL( textChanged( const QString& ) ), this, SLOT( gbOutput_changed() ) );
	connect( tbOutputPath, SIGNAL( clicked() ), this, SLOT( tbOutputPath_clicked() ) );
	connect( gbVersion, SIGNAL( clicked( bool ) ), this, SLOT( gbVersion_clicked( bool ) ) );
	foreach ( QSpinBox* sb, gbVersion->findChildren<QSpinBox*>() )
		connect( sb, SIGNAL( valueChanged( int ) ), this, SLOT( gbVersion_changed() ) );
	connect( cbBuildAutoIncrement, SIGNAL( clicked( bool ) ), this, SLOT( cbBuildAutoIncrement_clicked( bool ) ) );
	connect( lwQtModules, SIGNAL( itemSelectionChanged() ), this, SLOT( qtModulesConfigurations_itemSelectionChanged() ) );
	connect( lwCompilerFlags, SIGNAL( itemSelectionChanged() ), this, SLOT( qtModulesConfigurations_itemSelectionChanged() ) );
	connect( lwQtModules, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( qtModulesConfigurations_changed() ) );
	connect( lwCompilerFlags, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( qtModulesConfigurations_changed() ) );
	connect( leConfig, SIGNAL( textChanged( const QString& ) ), this, SLOT( qtModulesConfigurations_changed() ) );
	connect( twTranslations, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( twTranslations_itemChanged( QTreeWidgetItem*, int ) ) );
}

void QMakeProjectEditorDialog::createLanguages()
{
	// create items
	for ( int i = QLocale::Abkhazian; i < QLocale::Chewa +1; i++ )
	{
		const QLocale::Language lng = (QLocale::Language)i;
		const QLocale lc = QLocale( lng ).name();
		// create language item
		QTreeWidgetItem* it = new QTreeWidgetItem( twTranslations );
		it->setText( 0, QLocale::languageToString( lng ) );
		it->setCheckState( 0, Qt::Unchecked );
		it->setData( 0, Qt::UserRole, lc.name().split( "_" )[0] );
		it->setToolTip( 0, lc.name().split( "_" )[0].prepend( "Locale: " ) );
		// create country for language items
		foreach ( QLocale::Country cny, QLocale::countriesForLanguage( lng ) )
		{
			const QLocale clc = QLocale( lng, cny );
			QTreeWidgetItem* cit = new QTreeWidgetItem( it );
			cit->setText( 0, QLocale::countryToString( cny ) );
			cit->setCheckState( 0, Qt::Unchecked );
			cit->setData( 0, Qt::UserRole, clc.name() );
			cit->setToolTip( 0, clc.name().prepend( "Locale: " ) );
		}
	}
	// sort items
	twTranslations->sortItems( 0, Qt::AscendingOrder );
	// expand all items
	twTranslations->expandAll();
	// hide header
	twTranslations->header()->hide();
}

void QMakeProjectEditorDialog::createQtModulesConfigurations()
{
	// qt modules
	lwQtModules->clear();
	foreach ( const QtItem& item, mQtManager.modules() )
	{
		QListWidgetItem* it = new QListWidgetItem( item.Text, lwQtModules );
		it->setCheckState( Qt::Unchecked );
		it->setData( Qt::UserRole, QVariant::fromValue( item ) );
	}
	// qt configuration
	lwCompilerFlags->clear();
	foreach ( const QtItem& item, mQtManager.configurations() )
	{
		QListWidgetItem* it = new QListWidgetItem( item.Text, lwCompilerFlags );
		it->setData( Qt::UserRole, QVariant::fromValue( item ) );
		if ( item.Value.isEmpty() && item.Variable.isEmpty() )
		{
			QFont font( it->font() );
			font.setBold( true );
			it->setFont( font );
			it->setBackground( QBrush( QColor( Qt::darkBlue ) ) );
			it->setForeground( QBrush( QColor( Qt::white ) ) );
			it->setTextAlignment( Qt::AlignCenter );
			it->setFlags( it->flags() ^ Qt::ItemIsUserCheckable );
		}
		else
			it->setCheckState( Qt::Unchecked );
	}
}

void QMakeProjectEditorDialog::loadsQtVersions()
{
	lwQtVersions->clear();
	foreach ( const QtVersion& v, mQtManager.versions() )
	{
		QListWidgetItem* it = new QListWidgetItem( v.Version, lwQtVersions );
		it->setData( Qt::UserRole, QVariant::fromValue( v ) );
		if ( v.Default )
			it->setBackground( QBrush( Qt::green ) );
		if ( v == mQtVersion )
			it->setSelected( true );
	}
}

void QMakeProjectEditorDialog::loadsQtConfigurations()
{
	XUPItem* curScope = currentScope();
	const QString curOperator = currentOperator();
	XUPItem* vit = 0;
	int id = -1;
	QString value;
	QString config;
	QStringList values;
	// block signals of all children
	foreach ( QWidget* w, mQtConfigurationPage->findChildren<QWidget*>() )
		w->blockSignals( true );
	// *** APPLICATION
	// TEMPLATE
	id = ( vit = curScope->variable( cbTemplate->statusTip(), curOperator ) ) ? cbTemplate->findText( vit->variableValue() ) : -1;
	cbTemplate->setCurrentIndex( id );
	// CONFIG (Build Settings)
	config = ( vit = curScope->variable( "CONFIG", curOperator ) ) ? vit->variableValue() : QString();
	foreach ( QAbstractButton* ab, gbBuild->findChildren<QAbstractButton*>() )
	{
		const QRegExp rx( QString( "\\b%1\\b" ).arg( ab->statusTip() ) );
		ab->setAutoExclusive( false );
		ab->setChecked( rx.indexIn( config ) != -1 );
		if ( ab->inherits( "QRadioButton" ) )
			ab->setAutoExclusive( true );
		// remove entry in config for later set it to other config lineedit
		config.replace( rx, "" );
	}
	// VERSION
	values = mProject->projectSettingsValue( gbVersion->statusTip() ).split( ".", QString::SkipEmptyParts );
	gbVersion->setChecked( !values.isEmpty() );
	sbMajor->setValue( values.value( 0 ).toInt() );
	sbMinor->setValue( values.value( 1 ).toInt() );
	sbRelease->setValue( values.value( 2 ).toInt() );
	sbBuild->setValue( values.value( 3 ).toInt() );
	// APP_AUTO_INCREMENT_BUILD
	cbBuildAutoIncrement->setChecked( mProject->projectSettingsValue( cbBuildAutoIncrement->statusTip() ).toInt() == 1 );
	// DESTDIR
	value = ( vit = curScope->variable( leOutputPath->statusTip(), curOperator ) ) ? vit->variableValue() : QString();
	leOutputPath->setText( value );
	// TARGET
	value = ( vit = curScope->variable( leOutputName->statusTip(), curOperator ) ) ? vit->variableValue() : QString();
	leOutputName->setText( value );
	// APP_TITLE
	value = mProject->projectSettingsValue( leTitle->statusTip() );
	leTitle->setText( value );
	// APP_AUTHOR
	value = mProject->projectSettingsValue( leAuthor->statusTip() );
	leAuthor->setText( value );
	// *** CONFIGURATION
	// QT
	value = ( vit = curScope->variable( lwQtModules->statusTip(), curOperator ) ) ? vit->variableValue() : QString();
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* it = lwQtModules->item( i );
		const QtItem item = it->data( Qt::UserRole ).value<QtItem>();
		// bypass if not usable item
		if ( item.Value.isEmpty() && item.Variable.isEmpty() )
			continue;
		const QRegExp rx( QString( "\\b%1\\b" ).arg( item.Value ) );
		it->setCheckState( rx.indexIn( value ) == -1 ? Qt::Unchecked : Qt::Checked );
	}
	// CONFIG
	for ( int i = 0; i < lwCompilerFlags->count(); i++ )
	{
		QListWidgetItem* it = lwCompilerFlags->item( i );
		const QtItem item = it->data( Qt::UserRole ).value<QtItem>();
		// bypass if not usable item
		if ( item.Value.isEmpty() && item.Variable.isEmpty() )
			continue;
		const QRegExp rx( QString( "\\b%1\\b" ).arg( item.Value ) );
		it->setCheckState( rx.indexIn( config ) == -1 ? Qt::Unchecked : Qt::Checked );
		// remove entry in the value for later set it to other config lineedit
		config.replace( rx, "" );
	}
	leConfig->setText( config.trimmed() );
	// *** TRANSLATIONS
	values = ( vit = mProject->variable( twTranslations->statusTip(), "+=" ) ) ? vit->variableValues() : QStringList();
	foreach ( QTreeWidgetItem* it, twTranslations->findItems( "*", Qt::MatchWildcard | Qt::MatchWrap | Qt::MatchRecursive ) )
	{
		const QString locale = it->data( 0, Qt::UserRole ).toString();
		value = QString( QtTranslationMask ).arg( locale );
		it->setCheckState( 0, values.contains( value ) ? Qt::Checked : Qt::Unchecked );
	}
	// disable widgets
	// ** Application
	cbTemplate->setEnabled( curScope == mProject && curOperator == "=" );
	fBuildType->setEnabled( curScope == mProject && curOperator != "=" );
	fWarn->setEnabled( fBuildType->isEnabled() );
	fBuildAll->setEnabled( fBuildType->isEnabled() );
	gbOutput->setEnabled( curOperator == "=" );
	// ** CONFIGURATION
	lwCompilerFlags->setEnabled( curOperator != "=" );
	leConfig->setEnabled( lwCompilerFlags->isEnabled() );
	// restore signals of all children
	foreach ( QWidget* w, mQtConfigurationPage->findChildren<QWidget*>() )
		w->blockSignals( false );
}

void QMakeProjectEditorDialog::currentScopeChanged( XUPItem* /*curScope*/ )
{ loadsQtConfigurations(); }

void QMakeProjectEditorDialog::currentOperatorChanged( const QString& /*curOperator*/  )
{ loadsQtConfigurations(); }

void QMakeProjectEditorDialog::lwQtVersions_currentItemChanged( QListWidgetItem* current, QListWidgetItem* )
{
	if ( current )
	{
		QtVersion v = current->data( Qt::UserRole ).value<QtVersion>();
		leQtVersion->setText( v.Version );
		leQtPath->setText( v.Path );
		leQtQMakeSpec->setText( v.QMakeSpec );
		leQtQMakeParameters->setText( v.QMakeParameters );
	}
}

void QMakeProjectEditorDialog::lQtVersionInformations_linkActivated( const QString& link )
{
	if ( link == "plugin_config" )
	{
		XUPPlugin* xp = MonkeyCore::pluginsManager()->plugin<XUPPlugin*>( PluginsManager::stAll, mProject->projectSettingsValue( "EDITOR" ) );
		if ( xp )
		{
			QWidget* cd = xp->settingsWidget();
			cd->setAttribute( Qt::WA_DeleteOnClose );
			cd->setWindowFlags( Qt::Dialog );
			cd->setWindowModality( Qt::ApplicationModal );
			connect( cd->findChild<QDialogButtonBox*>()->button( QDialogButtonBox::Save ), SIGNAL( clicked() ), cd, SLOT( close() ) );
			connect( cd, SIGNAL( destroyed( QObject* ) ), this, SLOT( loadsQtVersions() ) );
			cd->show();
		}
	}
}

void QMakeProjectEditorDialog::cbTemplate_currentIndexChanged( const QString& curTemplate )
{ mProject->variable( cbTemplate->statusTip(), "=", 0, true )->setVariableValue( curTemplate ); }

void QMakeProjectEditorDialog::gbBuild_changed()
{
	// get new values
	QStringList values;
	foreach ( QAbstractButton* ab, gbBuild->findChildren<QAbstractButton*>() )
		if ( ab->isChecked() )
			values << ab->statusTip();
	// add project settings scope
	if ( currentScope() == mProject && currentOperator() == "+=" )
		if ( !values.contains( XUPItem::mProjectSettingsScope ) )
			values << XUPItem::mProjectSettingsScope;
	// get variable
	XUPItem* vit = mProject->variable( gbBuild->statusTip(), currentOperator(), 0, !values.isEmpty() );
	// update variable values if needed
	if ( vit )
		vit->setVariableValue( values.join( " " ) );
}

void QMakeProjectEditorDialog::gbApplication_changed()
{
	if ( QLineEdit* le = qobject_cast<QLineEdit*>( sender() ) )
		mProject->setProjectSettingsValue( le->statusTip(), le->text() );
}

void QMakeProjectEditorDialog::gbOutput_changed()
{
	QLineEdit* le = qobject_cast<QLineEdit*>( sender() );
	// get variable
	XUPItem* vit = currentScope()->variable( le->statusTip(), currentOperator(), 0, !le->text().isEmpty() );
	// update variable values if needed
	if ( vit )
		vit->setVariableValue( le->text() );
}

void QMakeProjectEditorDialog::tbOutputPath_clicked()
{
	const QString s = QFileDialog::getExistingDirectory( window(), tr( "Choose an output path for your binary" ), leOutputPath->text() );
	if ( !s.isEmpty() )
		leOutputPath->setText( mProject->relativeFilePath( s ) );
}

void QMakeProjectEditorDialog::gbVersion_clicked( bool checked )
{
	if ( checked )
		gbVersion_changed();
	else
		mProject->setProjectSettingsValue( gbVersion->statusTip(), QString() );
}

void QMakeProjectEditorDialog::gbVersion_changed()
{
	QStringList version;
	version << sbMajor->text();
	version << sbMinor->text();
	version << sbRelease->text();
	version << sbBuild->text();
	mProject->setProjectSettingsValue( gbVersion->statusTip(), version.join( "." ) );
}

void QMakeProjectEditorDialog::cbBuildAutoIncrement_clicked( bool checked )
{ mProject->setProjectSettingsValue( cbBuildAutoIncrement->statusTip(), checked ? "1" : "" ); }

void QMakeProjectEditorDialog::qtModulesConfigurations_itemSelectionChanged()
{
	tbInformations->clear();
	if ( QListWidget* lw = qobject_cast<QListWidget*>( sender() ) )
		if ( QListWidgetItem* it = lw->selectedItems().value( 0 ) )
			tbInformations->setHtml( it->data( Qt::UserRole ).value<QtItem>().Help );
}

void QMakeProjectEditorDialog::qtModulesConfigurations_changed()
{
	// QT
	if ( sender() == lwQtModules )
	{
		QStringList values;
		// lwQtModules
		for ( int i = 0; i < lwQtModules->count(); i++ )
		{
			// get item
			QListWidgetItem* it = lwQtModules->item( i );
			// check state
			if ( it->checkState() == Qt::Checked )
				values << it->data( Qt::UserRole ).value<QtItem>().Value;
		}
		// get variable item
		XUPItem* vit = currentScope()->variable( lwQtModules->statusTip(), currentOperator(), 0, !values.isEmpty() );
		// update variable values if needed
		if ( vit )
			vit->setVariableValue( values.join( " " ) );
	}
	// CONFIG
	else
	{
		QStringList values;
		// gbBuild
		foreach ( QAbstractButton* ab, gbBuild->findChildren<QAbstractButton*>() )
			if ( ab->isChecked() )
				values << ab->statusTip();
		// lwCompilerFlags
		for ( int i = 0; i < lwCompilerFlags->count(); i++ )
		{
			// get item
			QListWidgetItem* it = lwCompilerFlags->item( i );
			// check state
			if ( it->checkState() == Qt::Checked )
				values << it->data( Qt::UserRole ).value<QtItem>().Value;
		}
		// other config
		values << leConfig->text().trimmed();
		// get variable item
		XUPItem* vit = currentScope()->variable( lwCompilerFlags->statusTip(), currentOperator(), 0, !values.isEmpty() );
		// update variable values if needed
		if ( vit )
			vit->setVariableValue( values.join( " " ) );
	}
}

void QMakeProjectEditorDialog::twTranslations_itemChanged( QTreeWidgetItem* item, int )
{
	if ( XUPItem* vit = mProject->variable( twTranslations->statusTip(), "+=", 0, true ) )
	{
		const QString locale = item->data( 0, Qt::UserRole ).toString();
		const QString translation = QString( QtTranslationMask ).arg( locale );
		if ( item->checkState( 0 ) == Qt::Checked )
			vit->addVariableValue( translation );
		else
			vit->removeVariableValue( translation );
	}
}

void QMakeProjectEditorDialog::accept()
{
	// qt version
	if ( QListWidgetItem* it = lwQtVersions->selectedItems().value( 0 ) )
		mQtVersion = it->data( Qt::UserRole ).value<QtVersion>();
	else
		mQtVersion = QtVersion();
	mProject->setProjectSettingsValue( "QT_VERSION", mQtVersion.Version );
	// default XUP Project Editor accept
	UIXUPProjectEditor::accept();
}
