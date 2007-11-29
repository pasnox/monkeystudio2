// #ifndef MSVCPARSER_H
// #define MSVCPARSER_H
// 
// #include "pCommandParser.h"
// 
// #include <QList>
// #include <QRegExp>
// 
// class MSVCParser : public pCommandParser
// {
// 	Q_OBJECT
// 	
// 	struct Pattern
// 	{
// 		QRegExp regExp;
// 		QString about;
// 		QString FileName;
// 		QString col;
// 		QString row;
// 		pConsoleManager::StepType Type;
// 		QString Text;
// 		QString FullText;
// 	};
// 
// protected:
// 	QList <Pattern> patterns;
// 	QString replaceWithMatch( QRegExp, QString );
// 
// public:
// 	MSVCParser();
// 	~MSVCParser();
// 
// 	bool processParsing( const QByteArray& );
// 
// };
// 
// #endif // MSVCPARSER_H
