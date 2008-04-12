#include "QMakeProjectEditorDialog.h"

#include <coremanager.h>
#include <pluginsmanager.h>

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
	//
	mQtVersion = mQtManager.version( mProject->projectSettingsValue( "QT_VERSION" ) );
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
	QStringList values;
	// block signals of all children
	foreach ( QWidget* w, mQtConfigurationPage->findChildren<QWidget*>() )
		w->blockSignals( true );
	// *** APPLICATION
	// TEMPLATE
	id = ( vit = curScope->variable( cbTemplate->statusTip(), curOperator ) ) ? cbTemplate->findText( vit->variableValue() ) : -1;
	cbTemplate->setCurrentIndex( id );
	// CONFIG (Build Settings)
	value = ( vit = curScope->variable( "CONFIG", curOperator ) ) ? vit->variableValue() : QString();
	foreach ( QAbstractButton* ab, gbBuild->findChildren<QAbstractButton*>() )
	{
		const QRegExp rx( QString( "\\b%1\\b" ).arg( ab->statusTip() ) );
		ab->setAutoExclusive( false );
		ab->setChecked( rx.indexIn( value ) != -1 );
		if ( ab->inherits( "QRadioButton" ) )
			ab->setAutoExclusive( true );
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
	// disable widgets
	fBuildType->setEnabled( curScope == mProject && curOperator != "=" );
	fWarn->setEnabled( fBuildType->isEnabled() );
	fBuildAll->setEnabled( fBuildType->isEnabled() );
	cbTemplate->setEnabled( curOperator == "=" );
	gbOutput->setEnabled( curOperator == "=" );
	// restore signals of all children
	foreach ( QWidget* w, mQtConfigurationPage->findChildren<QWidget*>() )
		w->blockSignals( false );
}

void QMakeProjectEditorDialog::currentScopeChanged( XUPItem* /*curScope*/ )
{ loadsQtConfigurations(); }

void QMakeProjectEditorDialog::currentOperatorChanged( const QString& /*curOperator*/  )
{ loadsQtConfigurations(); }

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
