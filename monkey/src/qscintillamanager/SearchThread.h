#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include "../consolemanager/pConsoleManager.h"

#include <QThread>
#include <QStringList>

class QFile;

class SearchThread : public QThread
{
    Q_OBJECT
public:
	enum Mode
	{
		SEARCH,
		REPLACE
	};
	
    SearchThread(Mode mode, const QString &dir, QString &mask, const QString &text, const QString& replace, bool caseSensitive, bool regExp, QObject* parent);
    ~SearchThread();
    void run();
    void setTermEnabled(bool t) {mTerm = t;};
    bool isTermEnabled() {return mTerm;};

		
private:
    QString mDir;
    QString mMask;
    QString mSearch;
	QString mReplace;
    bool mIsWhole;
    bool mIsMatch;
    bool mIsReg;
	bool mCaseSensetive;
	
	Mode mMode;
	
    bool mTerm;
	
	// Heuristics: if first 1k of file contains '\0' - file is binary
	// NOTE: procedure moving current pos in the file
	bool isBinary (QFile& file);
	
	void search (QFile& file);
	void replace (QFile& file);
	
signals:
    void appendSearchResult( const pConsoleManager::Step& );
    void changeProgress(int);
};

#endif //SEARCHTHREAD_H
