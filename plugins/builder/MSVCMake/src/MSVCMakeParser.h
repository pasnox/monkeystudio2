// #ifndef MSVCMAKEPARSER_H
// #define MSVCMAKEPARSER_H
// 
// #include <QProcess>
// #include <QList>
// #include <QRegExp>
// 
// #include "pCommandParser.h"
// 
// class MSVCMakeParser : public pCommandParser
// {
// struct Pattern
// {
// 	QRegExp regExp;
// 	QString about;
// 	QString FileName;
// 	QString col;
// 	QString row;
// 	pConsoleManager::StepType Type;
// 	QString Text;
// 	QString FullText;
// };
// 
// Q_OBJECT
// protected:
// 	QRegExp rxErrWarn, rxBuild, rxUndefRef, rxNoRule;
// 	QList <Pattern> patterns;
// 	QString replaceWithMatch(QRegExp, QString);
// public:
// 	MSVCMakeParser();
// 	~MSVCMakeParser();
// 
// 	QString name() const;
// 	bool processParsing(const QByteArray&);
// 
// };
// 
// #endif
