
#ifndef GDBCONTROLER_H
#define GDBCONTROLER_H

#include "gdbCore.h"
#include "gdbParser.h"
#include "gdbProcess.h"
#include "gdbInterpreter.h"
#include "gdbConnectTemplate.h"
//#include "gdbBridgeEditor.h"
#include "gdbKernelDispatcher.h"
#include "gdbSetting.h"

#include <QFileDialog>
#include <QAction>

class GdbControler : public QObject
{
    Q_OBJECT

public : 

    GdbControler(QObject * parent = 0);
    ~GdbControler();

    bool isGdbStarted(){ return mGdbStarted;}
    bool isGdbRunning() { return mGdbRunning;}
    bool isTargetLoaded(){ return mTargetLoaded;}
    bool isTargetStopped(){ return mTargetStopped;}


public slots:

    // from user action
    void onActionLoadTarget();
    void onActionExit();
    void onActionRestart();
    void onActionContinue();
    void onActionStepOver();
    void onActionStepInto();
    void onActionStepFinish();

    // from parser
    void onTargetStopped(int , QString );
    void onTargetRunning(int , QString );
    void onTargetLoaded(int , QString );
    void onTargetNoLoaded(int , QString );
    void onTargetExited(int , QString );
    void onTargetCrashed(int , QString );
    void onNoExecCommand(int , QString);
    void onInterpreter(const QPointer<BaseInterpreter> & , const int &, const QString &);

    // from parser
    void onDone(int , QString);
    void onPrompt(int , QString);

    // from process
    void onGdbStarted();
    void onGdbFinished( int , QProcess::ExitStatus);
    void onGdbError(QProcess::ProcessError);

    QHash<QString , QAction*> getActionsAvailable(){return mActionList;}
    
private:

    void setMenuAction();
    void removeMenuAction();
    QString name();
    void setEnabledActions(bool b);

    bool mGdbStarted;
    bool mGdbRunning;
    bool mTargetLoaded;
    bool mTargetStopped;

    QString mSelectedTarget;

    /*!
    * \details Connect is a GdbConnectTemplate var, it's use for connect interperter to other function
    */
    GdbConnectTemplate<GdbControler> Connect;

    /*!
    * \details Contains interpreterStepOver pointer. This pointer is connect to onTargetStopped function
    */
    QPointer<BaseInterpreter> interpreterStepOver;
    QPointer<BaseInterpreter> interpreterStepInto;
    QPointer<BaseInterpreter> interpreterStepFinish;

    bool mDisableStep;
    
    /*!
        \details List of all actions
    */
    QHash<QString , QAction*> mActionList;

    /*!
        \details Just a separator in toolBar.
    */
    QPointer<QAction> aSeparator;

signals:

    void errorMessage(QString , int time =0);
    void defaultMessage(QString);
};
#endif
