#include <QDebug>

#include "GccParser.h"

GccParser::GccParser()
{
	Pattern ps[] = 
	{
		{
			QRegExp("^(.+):(\\d+):(\\d+:)?\\serror:\\s(.+)$"), //reg exp
			"Error in the file/line", //desctiption
			"%1", //file name
			"0", //column
			"%2", //row
			pConsoleManager::stError, //type
			"%1:%2: %4", //text
			"%0", //full text
		},
		{
			QRegExp("^(.+):(\\d+):(\\d+:)?\\serror:\\s(.+)$"), //reg exp
			"Warning in the file/line", //desctiption
			"%1", //file name
			"0", //column
			"%2", //row
			pConsoleManager::stWarning, //type
			"%1:%2: %4", //text
			"%0" //full text
		},
		{
			QRegExp("^g\\+\\+.+([^\\s]+\\.cpp)"), //reg exp
			"Building file", //desctiption
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stCompiling, //type
			"Compiling %1...", //text
			"%0" //full text
		},
		{
			QRegExp("^.*(In\\sfunction\\s.*:.*:).+(\\sundefined\\sreference\\sto.+)$"), //reg exp
			"Undedined reference", //desctiption
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stError, //type
			"%1%2", //text
			"%0" //full text
		},
		{QRegExp(), "", "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
	};
	for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
		patterns.append (ps[i]);
		
	rxErrWarn.setPattern ("^(.+):(\\d+):(\\d+:)?\\s((warning)|(error)):\\s(.+)$");
	rxBuild.setPattern ("^g\\+\\+.+([^\\s]+\\.cpp)");
	rxUndefRef.setPattern ("^.*(In\\sfunction\\s.*:.*:).+(\\sundefined\\sreference\\sto.+)$");
	rxNoRule.setPattern ("^make: \\*\\*\\* (No rule to make target `.+', needed by `.+')\\.  Stop\\.$" );  // FIXME not a Gcc error
}

GccParser::~GccParser()
{
}

bool GccParser::processParsing(const QByteArray& arr)
{
	QStringList l = QString(arr).split( '\n' );
	foreach (QString s, l)
	{
		foreach ( Pattern p, patterns)
		{
			if (p.regExp.indexIn(s) != -1)
			{
				pConsoleManager::Step m;
				m.mFileName = replaceWithMatch(p.regExp,p.FileName);
				m.mPosition = QPoint( replaceWithMatch(p.regExp,p.row).toInt(),replaceWithMatch(p.regExp,p.col).toInt());
				m.mType = p.Type;
				m.mText = replaceWithMatch(p.regExp,p.Text);
				m.mFullText = replaceWithMatch(p.regExp,p.FullText);
				// emit signal
				emit newStepAvailable( m );
				return true;
			}
			
		}
	}
// 		// compiling file
// 		if ( rxBuild.indexIn( s )  != -1 )
// 		{
// 			pConsoleManager::Message m;
// 			m.mFullText = rxBuild.cap( 0 );
// 			m.mFileName = QString::null;
// 			m.mPosition = QPoint( 0, 0 );
// 			m.mType = pConsoleManager::Compiling;
// 			m.mText =  tr( "Compiling %1..." ).arg( rxBuild.cap( 1 ) );
// 			// emit signal
// 			emit newMessageAvailible( m );
// 			return true;
// 		}
// 		// error, warning in the source
// 		if (   rxErrWarn.indexIn( s ) != -1 )
// 		{
// 			pConsoleManager::Message m;
// 			m.mFullText = rxErrWarn.cap( 0 );
// 			m.mFileName = rxErrWarn.cap( 1 );
// 			m.mPosition = QPoint( 0, rxErrWarn.cap( 2 ).toInt() );
// 			if ( rxErrWarn.cap( 4 ).toLower() == "error" )
// 				m.mType = pConsoleManager::Error;
// 			else if ( rxErrWarn.cap( 4 ).toLower() == "warning" )
// 				m.mType = pConsoleManager::Warning;
// 			m.mText = QString( "%1:%2: %3" ).arg( m.mFileName ).arg( m.mPosition.y() ).arg( rxErrWarn.cap( 7 ) );
// 			// emit signal
// 			emit newMessageAvailible( m );
// 			return true;
// 		}
// 		// undefined reference
// 		if (   rxUndefRef.indexIn( s ) != -1 )
// 		{
// 			pConsoleManager::Message m;
// 			m.mFullText = rxUndefRef.cap( 0 );
// 			m.mFileName = QString::null;
// 			m.mPosition = QPoint( 0, 0 );
// 			m.mType = pConsoleManager::Error;
// 			m.mText =   rxUndefRef.cap( 1 )   //  in function 'main':main.cpp:
// 				+ rxUndefRef.cap( 2 ) ;  //undefined reference to ...
// 			// emit signal
// 			emit newMessageAvailible( m );
// 			return true;
// 		}
// 		// no rule to make target ...
// 		if (   rxNoRule.indexIn( s ) != -1 )
// 		{
// 			pConsoleManager::Message m;
// 			m.mFullText = rxNoRule.cap( 0 );
// 			m.mFileName = QString::null;
// 			m.mPosition = QPoint( 0, 0 );
// 			m.mType = pConsoleManager::Error;
// 			m.mText =   rxNoRule.cap( 1 ) ;
// 			// emit signal
// 			emit newMessageAvailible( m );
// 			return true;
// 		}
//	}
return false;
}

QString GccParser::name() const
{
	return QString ("Gcc");
}

QString GccParser::replaceWithMatch(QRegExp rex, QString s)
{
	int i = 0;
	while ( (i = s.indexOf("%")) != -1)
		s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
	return s;
}