#ifndef UIGNUDEBUGGERSETTING_H
#define UIGNUDEBUGGERSETTING_H
#include "ui_UIGNUDebuggerSetting.h"

#include "../../src/kernel/gdbProcess.1.3.h"
#include <QPointer>

class UIGNUDebuggerSetting : public QWidget, public Ui::UIGNUDebuggerSetting
{
	Q_OBJECT

public:
	UIGNUDebuggerSetting( QWidget* = 0 );
	void setKernel(QList<QPointer< class GdbCore> >);

protected slots:

	void on_dbbButtons_clicked( QAbstractButton* button );


	void commandReadyRead( const QByteArray & );
	void started();
	void finished( int , QProcess::ExitStatus  );
	void error( QProcess::ProcessError );
	void bClicked();

	void loadSettings();
	void saveSettings();

private :
	QString mPathGdb;
	QPointer<GdbProcess> Process;
	QStringList mPluginList;
};

#endif // UIGNUDEBUGGERSETTING_H
