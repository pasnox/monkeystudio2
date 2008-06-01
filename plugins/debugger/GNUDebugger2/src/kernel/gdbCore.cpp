
#include "gdbCore.h"



GdbCore::GdbCore(QObject * parent, QPointer<GdbParser> pa , 	QPointer<GdbProcess> pr) : QObject (parent)
{ mParser = pa; mProcess = pr;}

GdbCore::~GdbCore()
{
	setEnabled(true);
	setWaitEndProcess(false);
}

void GdbCore::gdbStarted(){}
void GdbCore::gdbFinished(){}
void GdbCore::gdbError(){}

void GdbCore::targetLoaded(const int &, const QString &){}
void GdbCore::targetNoLoaded(const int &, const QString &){}
void GdbCore::targetRunning(const int &, const QString &){}
void GdbCore::targetStopped(const int &, const QString &){}
void GdbCore::targetExited(const int &, const QString &){}

void GdbCore::error(const int &, const QString &){}
void GdbCore::done(const int &, const QString &){}
void GdbCore::info(const int &, const QString &){}

QString GdbCore::name() { return QString();}
QPointer<QWidget> GdbCore::widget() { return QPointer<QWidget>();}

void GdbCore::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s){}

QString GdbCore::findValue(const QString & st , const QString & key)
{
	QStringList l = st.split("\",");
	
	foreach(QString s, l)
	{
		QString k = s.left(s.indexOf("="));
		QString v = s.right(s.length() - s.indexOf("=") - 1);
		if(k == key ) return v.remove("\"");
	}
	return QString();
}
