#ifndef VARIABLESMANAGER_H
#define VARIABLESMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QApplication>
#include <QHash>

class Q_MONKEY_EXPORT VariablesManager : public QObject, public QSingleton<VariablesManager>
{
    Q_OBJECT
    friend class QSingleton<VariablesManager>;
	
private:
    VariablesManager( QObject* = QApplication::instance() );

public:
    typedef QHash<QString, QString> Dictionary;
    //Returns variable. 
    //Default value = QString::null used, if variable is not set
    QString getVariable( QString name,  Dictionary locals ); 

    //Is variable set
    bool isSet( QString name,  Dictionary& locals ); 

    //Will replace all variables in the string with values
    //If locals are presented, variables will be searched there too
    QString replaceAllVariables (QString str, Dictionary locals);

private:
    //Hash for storing global constand variables. (Kernel version for example)
    Dictionary globals;

};

#endif //VARIABLESMANAGER_H
