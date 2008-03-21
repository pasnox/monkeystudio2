#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include "pConsoleManager.h"

#include <QThread>
#include <QStringList>

class QFile;

class SearchThread : public QThread
{
    Q_OBJECT
public:
    SearchThread(const QString &, const QString &, bool, bool, bool, QObject* parent);
    ~SearchThread();
    void run();
    void setTermEnabled(bool t) {mTerm = t;};
    bool isTermEnabled() {return mTerm;};
private:
    QString dir;
    QString text; 
    bool isWhole;
    bool isMatch;
    bool isReg;
    bool mTerm;

	// Heuristics: if first 1k of file contains '\0' - file is binary
	// NOTE: procedure moving current pos in the file
	bool isBinary (QFile& file);
signals:
    void appendSearchResult( const pConsoleManager::Step& );
    void changeProgress(int);
};

#endif //SEARCHTHREAD_H
