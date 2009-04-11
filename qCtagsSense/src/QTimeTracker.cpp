#include "QTimeTracker.h"

QTimeTracker::QTimeTracker( const QString& name )
	: QTime()
{
	mAutoIncrement = 0;
	mName = name;
	
	start();
}

void QTimeTracker::query() const
{
	const QString text = QString( "*** %1 (%2): %3 seconds." ).arg( mName ).arg( mAutoIncrement++ ).arg( elapsed() /1000.0 );
	
	qWarning( "%s", text.toLocal8Bit().constData() );
}
