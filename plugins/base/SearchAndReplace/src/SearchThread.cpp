#include "SearchThread.h"

#include <QMutexLocker>
#include <QTextCodec>
#include <QTime>
#include <QDebug>

int SearchThread::mMaxTime = 125;

SearchThread::SearchThread( QObject* parent )
	: QThread( parent )
{
	mReset = false;
	mExit = false;
	
	qRegisterMetaType<SearchResultsModel::ResultList>( "SearchResultsModel::ResultList" );
}

SearchThread::~SearchThread()
{
	stop();
	wait();
}

void SearchThread::search( const SearchWidget::Properties& properties )
{
	{
		QMutexLocker locker( &mMutex );
		mProperties = properties;
		mReset = isRunning() ? true : false;
		mExit = false;
	}
	
	if ( !isRunning() )
	{
		start();
	}
}

void SearchThread::stop()
{
	{
		QMutexLocker locker( &mMutex );
		mReset = false;
		mExit = true;
	}
}

SearchWidget::Properties* SearchThread::properties() const
{
	QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
	return &const_cast<SearchThread*>( this )->mProperties;
}

QStringList SearchThread::getFiles( QDir fromDir, const QStringList& filters, bool recursive ) const
{
	QStringList files;
	
	foreach ( const QFileInfo& file, fromDir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name ) )
	{
		if ( file.isFile() && ( filters.isEmpty() || QDir::match( filters, file.fileName() ) ) )
		{
			files << file.absoluteFilePath();
		}
		else if ( file.isDir() && recursive )
		{
			fromDir.cd( file.filePath() );
			files << getFiles( fromDir, filters, recursive );
			fromDir.cdUp();
		}
		
		{
			QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
			
			if ( mReset || mExit )
			{
				return files;
			}
		}
	}
	
	return files;
}

QStringList SearchThread::getFilesToScan() const
{
	QSet<QString> files;
	SearchAndReplaceV2::Mode mode = SearchAndReplaceV2::ModeNo;
	
	{
		QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
		mode = mProperties.mode;
	}
	
	switch ( mode )
	{
		case SearchAndReplaceV2::ModeNo:
		case SearchAndReplaceV2::ModeSearch:
		case SearchAndReplaceV2::ModeReplace:
			qWarning() << "Invalid mode used.";
			Q_ASSERT( 0 );
			return files.toList();
		case SearchAndReplaceV2::ModeSearchDirectory:
		case SearchAndReplaceV2::ModeReplaceDirectory:
		{
			QString path;
			QStringList mask;
			
			{
				QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
				path = mProperties.searchPath;
				mask = mProperties.mask;
			}
			
			QDir dir( path );
			files = getFiles( dir, mask, true ).toSet();
			break;
		}
		case SearchAndReplaceV2::ModeSearchProjectFiles:
		case SearchAndReplaceV2::ModeReplaceProjectFiles:
		{
			QStringList sources;
			QStringList mask;
			
			{
				QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
				sources = mProperties.sourcesFiles;
				mask = mProperties.mask;
			}
			
			foreach ( const QString& fileName, sources )
			{
				if ( QDir::match( mask, fileName ) )
				{
					files << fileName;
				}
				
				{
					QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
					
					if ( mReset || mExit )
					{
						return files.toList();
					}
				}
			}
			break;
		}
		case SearchAndReplaceV2::ModeSearchOpenedFiles:
		case SearchAndReplaceV2::ModeReplaceOpenedFiles:
		{
			QStringList sources;
			QStringList mask;
			
			{
				QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
				sources = mProperties.openedFiles.keys();
				mask = mProperties.mask;
			}
			
			foreach ( const QString& fileName, sources )
			{
				if ( QDir::match( mask, fileName ) )
				{
					files << fileName;
				}
				
				{
					QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
					
					if ( mReset || mExit )
					{
						return files.toList();
					}
				}
			}
			break;
		}
	}
	
	return files.toList();
}

QString SearchThread::fileContent( const QString& fileName ) const
{
	QTextCodec* codec = 0;
	
	{
		QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
		
		codec = QTextCodec::codecForName( mProperties.codec.toLocal8Bit() );
		
		if ( mProperties.openedFiles.contains( fileName ) )
		{
			return mProperties.openedFiles[ fileName ];
		}
	}
	
	Q_ASSERT( codec );
	
	QFile file( fileName );
	
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		return QString::null;
	}
	
	return codec->toUnicode( file.readAll() );
}

void SearchThread::search( const QString& fileName, const QString& content ) const
{
	static QRegExp eolRx( "(?:\\r\\n|\\n|\\r)" );
	bool checkable = false;
	QRegExp rx;
	
	{
		QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
		
		const bool isRE = mProperties.options & SearchAndReplaceV2::OptionRegularExpression;
		const bool isWw = mProperties.options & SearchAndReplaceV2::OptionWholeWord;
		const bool isCS = mProperties.options & SearchAndReplaceV2::OptionCaseSensitive;
		const Qt::CaseSensitivity sensitivity = isCS ? Qt::CaseSensitive : Qt::CaseInsensitive;
		checkable = mProperties.mode & SearchAndReplaceV2::ModeFlagReplace;
		QString pattern = isRE ? mProperties.searchText : QRegExp::escape( mProperties.searchText );
		
		if ( !isRE && isWw )
		{
			pattern.prepend( "\\b" ).append( "\\b" );
		}
		
		rx.setPattern( pattern );
		rx.setCaseSensitivity( sensitivity );
	}
	
	int pos = 0;
	int lastEol = 0;
	SearchResultsModel::ResultList results;
	QTime tracker;
	
	tracker.start();
	
	while ( ( pos = rx.indexIn( content, pos ) ) != -1 )
	{
		const int eolStart = content.left( pos ).lastIndexOf( eolRx );
		const int eolEnd = content.indexOf( eolRx, pos );
		const QString capture = content.mid( eolStart, eolEnd -eolStart ).simplified();
		const int line = content.left( eolEnd ).count( eolRx );
		const int column = ( pos -eolStart ) -( eolStart != 0 ? 1 : 0 );
		SearchResultsModel::Result* result = new SearchResultsModel::Result( fileName, capture );
		result->position = QPoint( column, line );
		result->offset = pos;
		result->checkable = checkable;
		result->checkState = checkable ? Qt::Checked : Qt::Unchecked;
		
		results << result;
		
		pos += rx.matchedLength();
		lastEol = eolEnd;
		
		if ( tracker.elapsed() >= mMaxTime )
		{
			emit const_cast<SearchThread*>( this )->resultsAvailable( fileName, results );
			results.clear();
			tracker.restart();
		}
		
		{
			QMutexLocker locker( const_cast<QMutex*>( &mMutex ) );
			
			if ( mReset || mExit )
			{
				return;
			}
		}
	}
	
	if ( !results.isEmpty() )
	{
		emit const_cast<SearchThread*>( this )->resultsAvailable( fileName, results );
	}
}

void SearchThread::run()
{
	QTime tracker;
	
	forever
	{
		{
			QMutexLocker locker( &mMutex );
			mReset = false;
			mExit = false;
		}
		
		emit reset();
		tracker.restart();
		
		QStringList files = getFilesToScan();
		files.sort();
		
		{
			QMutexLocker locker( &mMutex );
			
			if ( mExit )
			{
				return;
			}
			else if ( mReset )
			{
				continue;
			}
		}
		
		foreach ( const QString& fileName, files )
		{
			const QString content = fileContent( fileName );
			search( fileName, content );
			
			{
				QMutexLocker locker( &mMutex );
				
				if ( mExit )
				{
					return;
				}
				else if ( mReset )
				{
					break;
				}
			}
		}
		
		{
			QMutexLocker locker( &mMutex );
			
			if ( mReset )
			{
				continue;
			}
		}
		
		break;
	}
	
	qWarning() << "Search finished in " << tracker.elapsed() /1000.0;
}

void SearchThread::clear()
{
	stop();
	emit reset();
}
