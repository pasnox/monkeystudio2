#include "ReplaceThread.h"

ReplaceThread::ReplaceThread( QObject* parent )
	: QThread( parent )
{
}

ReplaceThread::~ReplaceThread()
{
}

void ReplaceThread::run()
{
	/*
	switch ( mProperties.mode )
	{
		case SearchAndReplaceV2::ModeNo:
		case SearchAndReplaceV2::ModeSearch:
		case SearchAndReplaceV2::ModeReplace:
			Q_ASSERT( 0 );
			break;
		case SearchAndReplaceV2::ModeSearchDirectory:
		case SearchAndReplaceV2::ModeReplaceDirectory:
		case SearchAndReplaceV2::ModeSearchProject:
		case SearchAndReplaceV2::ModeReplaceProject:
		case SearchAndReplaceV2::ModeSearchOpenedFiles:
		case SearchAndReplaceV2::ModeReplaceOpenedFiles:
			break;
	}
	*/
}
