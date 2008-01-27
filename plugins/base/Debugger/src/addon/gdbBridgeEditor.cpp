/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : samedi 12 janvier 2008 - 13h43
 * AUTHOR       :  (  )
 * FILENAME     : gdbBridgeEditor.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/

//==========================================

/*
	Class GdbBridgeEditor
*/

#include "gdbBridgeEditor.h"

#include <QFileInfo>

GdbBridgeEditor::GdbBridgeEditor(QWidget *o) :  GdbBase(o), bJustAdd(0), bTargetLoaded(0), bTargetRunning(0), bGdbStarted(0)
{
	mWidget = new QTextEdit(this);
//	mWidget->setEnabled(false);

	cmd.setClass(this);

	cmd.connectEventNotifyToProcess("back-trace", &GdbBridgeEditor::processBackTrace);
	cmd.connectEventNotifyToProcess("breakpoint-moved", &GdbBridgeEditor::processBreakpointMoved);
	cmd.connectEventNotifyToProcess("breakpoint-add", &GdbBridgeEditor::processBreakpointAdd);
	cmd.connectEventNotifyToProcess("breakpoint-deleted", &GdbBridgeEditor::processBreakpointDeleted);
	cmd.connectEventNotifyToProcess("breakpoint-enabled", &GdbBridgeEditor::processBreakpointEnabled);
	cmd.connectEventNotifyToProcess("breakpoint-disabled", &GdbBridgeEditor::processBreakpointDisabled);
	cmd.connectEventNotifyToProcess("breakpoint-conditionned", &GdbBridgeEditor::processBreakpointConditionned);
	cmd.connectEventNotifyToProcess("breakpoint-unconditionned", &GdbBridgeEditor::processBreakpointUnConditionned);
} 
//
GdbBridgeEditor::~GdbBridgeEditor()
{
} 
//
QString GdbBridgeEditor::name()
{
	 return "GdbBridgeEditor"; 
}
//
QWidget * GdbBridgeEditor::widget()
{
	return (QWidget*) mWidget ; 
}
//
void GdbBridgeEditor::gdbStarted()
{
	bGdbStarted = true;
}
//
void GdbBridgeEditor::gdbFinished()
{
	bGdbStarted = false;
	bTargetLoaded = false;
	mWidget->setEnabled(false);
}
//
void GdbBridgeEditor::targetLoaded()
{
	bTargetLoaded = true;
}
//
void GdbBridgeEditor::targetRunning()
{
	bTargetRunning = true;
	mWidget->setEnabled(false);
}
//
void GdbBridgeEditor::targetStopped()
{
	bTargetRunning = false;
	mWidget->setEnabled(true);
}
//
void GdbBridgeEditor::targetExited()
{
	bTargetRunning = false;
	mWidget->setEnabled(false);
}
//
void GdbBridgeEditor::setupDockWidget(QMainWindow *mw)
{
mw = mw;
	setWidget(widget());
	setWindowTitle(name());
	setFeatures (QDockWidget::DockWidgetMovable |QDockWidget::DockWidgetFloatable);
	setAllowedAreas(Qt::AllDockWidgetAreas);
}
//
int GdbBridgeEditor::process(int id,QByteArray data)
{
	if(!bGdbStarted || bTargetRunning || !bTargetLoaded) return PROCESS_TERMINED;

	return cmd.dispatchProcess(id ,data);
}
//
int GdbBridgeEditor::processError(int id, QByteArray data)
{
id = id;
	// TODO
	mWidget->append(getParametre("answerGdb=", data));

	return PROCESS_TERMINED;
}
//
void GdbBridgeEditor::processExit()
{
}
//
int GdbBridgeEditor::processBackTrace(int id , QByteArray data)
{	
id = id;
	emit backtrace(getParametre("fileName=", data), getParametre("line=", data).toInt());
	mWidget->append("move icon back trace in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointMoved(int id , QByteArray data)
{
id = id;
	emit breakpointMoved(getParametre("fileName=", data), getParametre("beforLine=", data).toInt(), getParametre("afterLine=", data).toInt());
	mWidget->append("move icon breakpoint in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointAdd(int id , QByteArray data)
{
id = id;
	emit breakpoint(getParametre("fileName=", data), getParametre("line=", data).toInt(), true);
	mWidget->append("add icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointDeleted(int id , QByteArray data)
{
id = id;
	emit breakpoint(getParametre("fileName=", data), getParametre("line=", data).toInt(), false);
	mWidget->append("delete icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointEnabled(int id , QByteArray data)
{
id = id;
	emit breakpointEnabled(getParametre("fileName=", data), getParametre("line=", data).toInt(), true);
	mWidget->append("enable icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointDisabled(int id , QByteArray data)
{
id = id;
	emit breakpointEnabled(getParametre("fileName=", data), getParametre("line=", data).toInt(), false);
	mWidget->append("disable icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointConditionned(int id , QByteArray data)
{
id = id;
	emit breakpointConditionnaled(getParametre("fileName=", data), getParametre("line=", data).toInt(), true);
	mWidget->append("conditionned icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointUnConditionned(int id , QByteArray data)
{
id = id;
	emit breakpointConditionnaled(getParametre("fileName=", data), getParametre("line=", data).toInt(), false);
	mWidget->append("unconditionned icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processGeneric(int id, QByteArray data)
{
id = id;
data = data;
	return PROCESS_TERMINED;
}
//
