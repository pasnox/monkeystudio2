#include "QMakeConfigurationEditor.h"
#include "ui_QMakeConfigurationEditor.h"
#include "../QMake.h"
#include "../QtVersionManager.h"

#include <xupmanager/core/XUPProjectItem.h>

#include <pGenericTableModel.h>

#include <QDebug>

QMakeConfigurationEditor::QMakeConfigurationEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent )
	: XUPPageEditor( parent ), mPositiveValues( positive ), mNegativeValues( negative )
{
	mProject = 0;
	mPositiveQtModulesModel = new pGenericTableModel( this );
	mPositiveQtModulesModel->setColumnCount( 1 );
	mPositiveConfigurationModel = new pGenericTableModel( this );
	mPositiveConfigurationModel->setColumnCount( 1 );
	mNegativeQtModulesModel = new pGenericTableModel( this );
	mNegativeQtModulesModel->setColumnCount( 1 );
	mNegativeConfigurationModel = new pGenericTableModel( this );
	mNegativeConfigurationModel->setColumnCount( 1 );
	
	ui = new Ui_QMakeConfigurationEditor;
	ui->setupUi( this );
	ui->lvPositiveQtModules->setModel( mPositiveQtModulesModel );
	ui->lvPositiveConfiguration->setModel( mPositiveConfigurationModel );
	ui->lvNegativeQtModules->setModel( mNegativeQtModulesModel );
	ui->lvNegativeConfiguration->setModel( mNegativeConfigurationModel );
	
	connect( ui->lvPositiveQtModules, SIGNAL( pressed( const QModelIndex& ) ), this, SLOT( showIndexHelp( const QModelIndex& ) ) );
	connect( ui->lvPositiveQtModules, SIGNAL( entered( const QModelIndex& ) ), this, SLOT( showIndexHelp( const QModelIndex& ) ) );
	connect( ui->lvPositiveConfiguration, SIGNAL( pressed( const QModelIndex& ) ), this, SLOT( showIndexHelp( const QModelIndex& ) ) );
	connect( ui->lvPositiveConfiguration, SIGNAL( entered( const QModelIndex& ) ), this, SLOT( showIndexHelp( const QModelIndex& ) ) );
	connect( ui->lvNegativeQtModules, SIGNAL( pressed( const QModelIndex& ) ), this, SLOT( showIndexHelp( const QModelIndex& ) ) );
	connect( ui->lvNegativeQtModules, SIGNAL( entered( const QModelIndex& ) ), this, SLOT( showIndexHelp( const QModelIndex& ) ) );
	connect( ui->lvNegativeConfiguration, SIGNAL( pressed( const QModelIndex& ) ), this, SLOT( showIndexHelp( const QModelIndex& ) ) );
	connect( ui->lvNegativeConfiguration, SIGNAL( entered( const QModelIndex& ) ), this, SLOT( showIndexHelp( const QModelIndex& ) ) );
}

QMakeConfigurationEditor::~QMakeConfigurationEditor()
{
	delete ui;
}

void QMakeConfigurationEditor::setup( XUPProjectItem* project )
{
	mProject = project;
	
	const QtVersionManager* qmanager = QMake::versionManager();
	const QtItemList qtItems = qmanager->modules();
	const QtItemList configurationItems = qmanager->configurations();
	const QBrush background = palette().brush( QPalette::Active, QPalette::Highlight );
	const QBrush foreground = palette().brush( QPalette::Active, QPalette::HighlightedText );
	const QSet<QString> positiveQt = mPositiveValues.value( "QT" ).toSet();
	const QSet<QString> negativeQt = mNegativeValues.value( "QT" ).toSet();
	QSet<QString> positiveConfig = mPositiveValues.value( "CONFIG" ).toSet();
	QSet<QString> negativeConfig = mNegativeValues.value( "CONFIG" ).toSet();
	
	// qt modules
	mPositiveQtModulesModel->setRowCount( qtItems.count() );
	mNegativeQtModulesModel->setRowCount( qtItems.count() );
	
	for ( int i = 0; i < qtItems.count(); i++ ) {
		const QtItem item = qtItems[ i ];
		const QModelIndex positiveIndex = mPositiveQtModulesModel->index( i, 0 );
		const QModelIndex negativeIndex = mNegativeQtModulesModel->index( i, 0 );
		Qt::ItemFlags positiveFlags = positiveIndex.flags();
		Qt::ItemFlags negativeFlags = negativeIndex.flags();
		
		mPositiveQtModulesModel->setData( positiveIndex, item.Text, Qt::DisplayRole );
		mPositiveQtModulesModel->setData( positiveIndex, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
		mNegativeQtModulesModel->setData( negativeIndex, item.Text, Qt::DisplayRole );
		mNegativeQtModulesModel->setData( negativeIndex, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
		
		if ( item.Value.isEmpty() && item.Variable.isEmpty() ) {
			QFont font( positiveIndex.data( Qt::FontRole ).value<QFont>() );
			
			font.setBold( true );
			
			mPositiveQtModulesModel->setData( positiveIndex, font, Qt::FontRole );
			mPositiveQtModulesModel->setData( positiveIndex, background, Qt::BackgroundRole );
			mPositiveQtModulesModel->setData( positiveIndex, foreground, Qt::ForegroundRole );
			mNegativeQtModulesModel->setData( negativeIndex, font, Qt::FontRole );
			mNegativeQtModulesModel->setData( negativeIndex, background, Qt::BackgroundRole );
			mNegativeQtModulesModel->setData( negativeIndex, foreground, Qt::ForegroundRole );
		}
		else {
			Qt::CheckState positiveState = Qt::Unchecked;
			Qt::CheckState negativeState = Qt::Unchecked;
			
			if ( item.Variable == "CONFIG" && positiveConfig.contains( item.Value ) ) {
				positiveState = Qt::Checked;
				positiveConfig.remove( item.Value );
			}
			
			if ( item.Variable == "CONFIG" && negativeConfig.contains( item.Value ) ) {
				negativeState = Qt::Checked;
				negativeConfig.remove( item.Value );
			}
			
			if ( item.Variable == "QT" && positiveQt.contains( item.Value ) ) {
				positiveState = Qt::Checked;
			}
			
			if ( item.Variable == "QT" && negativeQt.contains( item.Value ) ) {
				negativeState = Qt::Checked;
			}
			
			mPositiveQtModulesModel->setData( positiveIndex, QVariant::fromValue( positiveFlags | Qt::ItemIsUserCheckable ), pGenericTableModel::ItemFlags );
			mPositiveQtModulesModel->setData( positiveIndex, positiveState, Qt::CheckStateRole );
			mNegativeQtModulesModel->setData( negativeIndex, QVariant::fromValue( negativeFlags | Qt::ItemIsUserCheckable ), pGenericTableModel::ItemFlags );
			mNegativeQtModulesModel->setData( negativeIndex, negativeState, Qt::CheckStateRole );
		}
	}
	
	// configuration
	mPositiveConfigurationModel->setRowCount( configurationItems.count() );
	mNegativeConfigurationModel->setRowCount( configurationItems.count() );
	
	for ( int i = 0; i < configurationItems.count(); i++ ) {
		const QtItem item = configurationItems[ i ];
		const QModelIndex positiveIndex = mPositiveConfigurationModel->index( i, 0 );
		const QModelIndex negativeIndex = mNegativeConfigurationModel->index( i, 0 );
		Qt::ItemFlags positiveFlags = positiveIndex.flags();
		Qt::ItemFlags negativeFlags = negativeIndex.flags();
		
		mPositiveConfigurationModel->setData( positiveIndex, item.Text, Qt::DisplayRole );
		mPositiveConfigurationModel->setData( positiveIndex, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
		mNegativeConfigurationModel->setData( negativeIndex, item.Text, Qt::DisplayRole );
		mNegativeConfigurationModel->setData( negativeIndex, QVariant::fromValue( item ), pGenericTableModel::ExtendedUserRole );
		
		if ( item.Value.isEmpty() && item.Variable.isEmpty() ) {
			QFont font( positiveIndex.data( Qt::FontRole ).value<QFont>() );
			
			font.setBold( true );
			
			mPositiveConfigurationModel->setData( positiveIndex, font, Qt::FontRole );
			mPositiveConfigurationModel->setData( positiveIndex, background, Qt::BackgroundRole );
			mPositiveConfigurationModel->setData( positiveIndex, foreground, Qt::ForegroundRole );
			mNegativeConfigurationModel->setData( negativeIndex, font, Qt::FontRole );
			mNegativeConfigurationModel->setData( negativeIndex, background, Qt::BackgroundRole );
			mNegativeConfigurationModel->setData( negativeIndex, foreground, Qt::ForegroundRole );
		}
		else {
			Qt::CheckState positiveState = Qt::Unchecked;
			Qt::CheckState negativeState = Qt::Unchecked;
			
			if ( item.Variable == "CONFIG" && positiveConfig.contains( item.Value ) ) {
				positiveState = Qt::Checked;
				positiveConfig.remove( item.Value );
			}
			
			if ( item.Variable == "CONFIG" && negativeConfig.contains( item.Value ) ) {
				negativeState = Qt::Checked;
				negativeConfig.remove( item.Value );
			}
			
			if ( item.Variable == "QT" && positiveQt.contains( item.Value ) ) {
				positiveState = Qt::Checked;
			}
			
			if ( item.Variable == "QT" && negativeQt.contains( item.Value ) ) {
				negativeState = Qt::Checked;
			}
			
			mPositiveConfigurationModel->setData( positiveIndex, QVariant::fromValue( positiveFlags | Qt::ItemIsUserCheckable ), pGenericTableModel::ItemFlags );
			mPositiveConfigurationModel->setData( positiveIndex, positiveState, Qt::CheckStateRole );
			mNegativeConfigurationModel->setData( negativeIndex, QVariant::fromValue( negativeFlags | Qt::ItemIsUserCheckable ), pGenericTableModel::ItemFlags );
			mNegativeConfigurationModel->setData( negativeIndex, negativeState, Qt::CheckStateRole );
		}
	}
	
	ui->lePositiveCustomConfiguration->setText( QStringList( positiveConfig.toList() ).join( " " ) );
	ui->leNegativeCustomConfiguration->setText( QStringList( negativeConfig.toList() ).join( " " ) );
}

void QMakeConfigurationEditor::finalize()
{
	const DocumentFilterMap& filters = mProject->documentFilters();
	const QStringList positiveCustomConfiguration = filters.splitValue( ui->lePositiveCustomConfiguration->text() );
	const QStringList negativeCustomConfiguration = filters.splitValue( ui->leNegativeCustomConfiguration->text() );
	
	// generate positive values
	foreach ( const QModelIndex& index, mPositiveQtModulesModel->checkedIndexes() ) {
		const QtItem item = index.data( pGenericTableModel::ExtendedUserRole ).value<QtItem>();
		mPositiveValues[ item.Variable ] << item.Value;
	}
	
	foreach ( const QModelIndex& index, mPositiveConfigurationModel->checkedIndexes() ) {
		const QtItem item = index.data( pGenericTableModel::ExtendedUserRole ).value<QtItem>();
		mPositiveValues[ item.Variable ] << item.Value;
	}
	
	if ( !positiveCustomConfiguration.isEmpty() ) {
		mPositiveValues[ "CONFIG" ] << positiveCustomConfiguration;
	}
	
	// generate negative values
	foreach ( const QModelIndex& index, mNegativeQtModulesModel->checkedIndexes() ) {
		const QtItem item = index.data( pGenericTableModel::ExtendedUserRole ).value<QtItem>();
		mNegativeValues[ item.Variable ] << item.Value;
	}
	
	foreach ( const QModelIndex& index, mNegativeConfigurationModel->checkedIndexes() ) {
		const QtItem item = index.data( pGenericTableModel::ExtendedUserRole ).value<QtItem>();
		mNegativeValues[ item.Variable ] << item.Value;
	}
	
	if ( !negativeCustomConfiguration.isEmpty() ) {
		mNegativeValues[ "CONFIG" ] << negativeCustomConfiguration;
	}
}

void QMakeConfigurationEditor::showIndexHelp( const QModelIndex& index )
{
	const QtItem item = index.data( pGenericTableModel::ExtendedUserRole ).value<QtItem>();
	ui->pteHelp->clear();
	ui->pteHelp->appendHtml( QString( "<b>%1:</b> %2" ).arg( item.Text ).arg( item.Help ) );
}
