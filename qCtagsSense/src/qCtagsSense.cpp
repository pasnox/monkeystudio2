#include "qCtagsSense.h"
#include "qCtagsSenseUtils.h"
#include "qCtagsSenseSQL.h"
#include "qCtagsSenseIndexer.h"

#include <ctags.h>

#include <QDebug>

qCtagsSense::qCtagsSense( QObject* parent )
	: QObject( parent )
{
	initCtags();
	qCtagsSenseUtils::initMaps();
	
	Option.include.fileNames = 1;
	Option.include.qualifiedTags = 0;
	Option.include.fileScope = 1;
	
	Option.extensionFields.access = 1;
	Option.extensionFields.fileScope = 1;
	Option.extensionFields.implementation = 1;
	Option.extensionFields.inheritance = 1;
	Option.extensionFields.kind = 1;
	Option.extensionFields.kindKey = 1;
	Option.extensionFields.kindLong = 1;
	Option.extensionFields.language = 1;
	Option.extensionFields.lineNumber = 1;
	Option.extensionFields.scope = 1;
	Option.extensionFields.signature = 1;
	Option.extensionFields.typeRef = 1;
	
	Option.if0 = 1;
	Option.kindLong = 1;
	
	//setLanguageKinds( "C++", "cdefglmnpstuvx" );
	
	mSQL = new qCtagsSenseSQL( this );
	mIndexer = new qCtagsSenseIndexer( mSQL );
	
	mInitialized = mSQL->initializeDatabase();
	
	connect( mIndexer, SIGNAL( indexingStarted() ), this, SIGNAL( indexingStarted() ) );
	connect( mIndexer, SIGNAL( indexingProgress( int, int ) ), this, SIGNAL( indexingProgress( int, int ) ) );
	connect( mIndexer, SIGNAL( indexingFinished() ), this, SIGNAL( indexingFinished() ) );
	connect( mIndexer, SIGNAL( indexingChanged() ), this, SIGNAL( indexingChanged() ) );
}

qCtagsSense::~qCtagsSense()
{
	deInitCtags();
}

bool qCtagsSense::isValid() const
{
	return mInitialized;
}

qCtagsSenseSQL* qCtagsSense::sql() const
{
	return mSQL;
}

qCtagsSenseIndexer* qCtagsSense::indexer() const
{
	return mIndexer;
}

void qCtagsSense::tagEntry( const QString& fileName )
{	
	if ( !mInitialized )
	{
		qWarning() << "qCtagsSense instance not initialized";
		return;
	}
	
	mIndexer->indexFile( fileName );
}

void qCtagsSense::tagEntries( const QMap<QString, QString>& entries )
{
	if ( !mInitialized )
	{
		qWarning() << "qCtagsSense instance not initialized";
		return;
	}
	
	mIndexer->indexBuffers( entries );
}
