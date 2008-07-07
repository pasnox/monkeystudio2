#ifndef UIGNUDEBUGGERSETTING_H
#define UIGNUDEBUGGERSETTING_H
#include "ui_UIGNUDebuggerSetting.h"

#include "../../src/kernel/gdbKernelDispatcher.1.3.h"

#include "UIManageParsingFile.h"

#include <QPointer>
#include <QProcess>

class UIGNUDebuggerSetting : public QWidget, public Ui::UIGNUDebuggerSetting
{
	Q_OBJECT

public:

	UIGNUDebuggerSetting( QWidget* = 0 );
	void setKernel(QList<QPointer< class GdbCore> >);

protected slots:

	void on_dbbButtons_clicked( QAbstractButton* button );


	void onReadyRead( );
	void started();
	void finished( int , QProcess::ExitStatus  );
	void error( QProcess::ProcessError );

	void bClickedPathGdb();
	void bClickedPathScript();
	void bClickedPathParsingFile();
	void bClickedManageFile();

	void loadSettings();
	void saveSettings();

private :

	
	QPointer<QProcess> Process;
	QStringList mPluginList;

	QVBoxLayout *vb;
};

#endif // UIGNUDEBUGGERSETTING_H
