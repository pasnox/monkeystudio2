#ifndef UIMESSAGEBOX_H
#define UIMESSAGEBOX_H

#include "QSingleton.h"
#include "ui_UIMessageBox.h"
#include "pConsoleManager.h"

class UIMessageBox : public QDockWidget, public Ui::UIMessageBox, public QSingleton<UIMessageBox>
{
	Q_OBJECT
	friend class QSingleton<UIMessageBox>;
	
private:
	UIMessageBox();

private slots:
	void commandError( pCommand*, QProcess::ProcessError );
	void commandFinished( pCommand*, int, QProcess::ExitStatus );
	void commandReadyRead( pCommand*, const QByteArray& );
	void commandStarted( pCommand* );
	void commandStateChanged( pCommand*, QProcess::ProcessState );
	
};

#endif // UIMESSAGEBOX_H
