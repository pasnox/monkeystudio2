#include "ReplaceThread.h"

#include <QMutexLocker>
#include <QTextCodec>
#include <QTime>
#include <QDebug>

int ReplaceThread::mMaxTime = 125;

ReplaceThread::ReplaceThread( QObject* parent )
	: QThread( parent )
{
	mReset = false;
	mExit = false;
}

ReplaceThread::~ReplaceThread()
{
	stop();
	wait();
}

void ReplaceThread::replace( const SearchWidget::Properties& properties, const QHash<QString, SearchResultsModel::ResultList>& results )
{
	{
		QMutexLocker locker( &mMutex );
		mProperties = properties;
		mResults = results;
		mReset = isRunning() ? true : false;
		mExit = false;
	}
	
	if ( !isRunning() )
	{
		start();
	}
}

void ReplaceThread::stop()
{
	{
		QMutexLocker locker( &mMutex );
		mReset = false;
		mExit = true;
	}
}

void ReplaceThread::saveContent( const QString& fileName, const QString& content, const QString& codec )
{
	QFile file( fileName );
	
	if ( !file.open( QIODevice::WriteOnly ) )
	{
		emit error( tr( "Error while saving replaced content: %1" ).arg( file.errorString() ) );
		return;
	}
	
	file.resize( 0 );
	
	QTextCodec* textCodec = QTextCodec::codecForName( codec.toLocal8Bit() );
	
	Q_ASSERT( textCodec );
	
	if ( file.write( textCodec->fromUnicode( content ) ) == -1 )
	{
		emit error( tr( "Error while saving replaced content: %1" ).arg( file.errorString() ) );
		return;
	}
	
	file.close();
}

QString ReplaceThread::fileContent( const QString& fileName ) const
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

void ReplaceThread::replace( const QString& fileName, QString content )
{
	QString replaceText;
	int replaceLength = 0;
	QString codec;
	SearchResultsModel::ResultList results;
	bool isOpenedFile = false;
	SearchResultsModel::ResultList handledResults;
	QHash<QString, QString> handledOpenedFiles;
	
	{
		QMutexLocker locker( &mMutex );
		replaceText = mProperties.replaceText;
		replaceLength = replaceText.length();
		codec = mProperties.codec;
		results = mResults[ fileName ];
		isOpenedFile = mProperties.openedFiles.contains( fileName );
	}
	
	QTime tracker;
	tracker.start();
	
	// count from end to begin because we are replacing by offset in content
	for ( int i = results.count() -1; i > -1; i-- )
	{
		SearchResultsModel::Result* result = results.at( i );
		
		content.replace( result->offset, replaceLength, replaceText );
		
		handledResults << result;
		
		if ( isOpenedFile )
		{
			handledOpenedFiles[ fileName ] = content;
		}
		
		if ( tracker.elapsed() >= mMaxTime )
		{
			if ( !handledResults.isEmpty() )
			{
				if ( !isOpenedFile )
				{
					saveContent( fileName, content, codec );
				}
				
				emit resultsHandled( fileName, handledResults );
			}
			
			if ( !handledOpenedFiles.isEmpty() )
			{
				emit openedFilesHandled( handledOpenedFiles );
			}
			
			handledResults.clear();
			handledOpenedFiles.clear();
			tracker.restart();
		}
		
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
	
	if ( !handledResults.isEmpty() )
	{
		if ( !isOpenedFile )
		{
			saveContent( fileName, content, codec );
		}
		
		emit resultsHandled( fileName, handledResults );
	}
	
	if ( !handledOpenedFiles.isEmpty() )
	{
		emit openedFilesHandled( handledOpenedFiles );
	}
}

void ReplaceThread::run()
{
	/*
	switch ( mProperties.mode )
	{
		case SearchAndReplaceV2::ModeNo:
		case SearchAndReplaceV2::ModeSearch:
		case SearchAndReplaceV2::ModeReplace:
		case SearchAndReplaceV2::ModeSearchDirectory:
		case SearchAndReplaceV2::ModeSearchProjectFiles:
		case SearchAndReplaceV2::ModeSearchOpenedFiles:
			Q_ASSERT( 0 );
			break;
		case SearchAndReplaceV2::ModeReplaceDirectory:
		case SearchAndReplaceV2::ModeReplaceProjectFiles:
		case SearchAndReplaceV2::ModeReplaceOpenedFiles:
			break;
	}
	*/
	
	QTime tracker;
	
	forever
	{
		{
			QMutexLocker locker( &mMutex );
			mReset = false;
			mExit = false;
		}
		
		tracker.restart();
		
		QStringList keys;
		
		{
			QMutexLocker locker( &mMutex );
			keys = mResults.keys();
		}
		
		foreach ( const QString& fileName, keys )
		{
			QString content = fileContent( fileName );
			
			replace( fileName, content );
			
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
			
			if ( mExit )
			{
				return;
			}
			else if ( mReset )
			{
				continue;
			}
		}
		
		break;
	}
	
	qWarning() << "Replace finished in " << tracker.elapsed() /1000.0;
}
