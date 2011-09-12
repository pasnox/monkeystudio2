#ifndef COMMANDLINEMANAGER_H
#define COMMANDLINEMANAGER_H

#include "MonkeyExport.h"

#include <QMap>
#include <QStringList>

class Q_MONKEY_EXPORT CommandLineManager
{
public:
    CommandLineManager();
    ~CommandLineManager();
    
    void parse();
    void process();
    const QMap<QString, QStringList>& arguments() const;
    
    void showVersion();
    void showHelp();
    void openProjects( const QStringList& fileNames );
    void openFiles( const QStringList& fileNames );

protected:
    QMap<QString, QStringList> mArguments;
    bool mVersionShown;
};

#endif // COMMANDLINEMANAGER_H
