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

GdbBridgeEditor::GdbBridgeEditor( GdbParser *p) :  GdbCore( p)
{
	mWidget = new QTextEdit();
	mWidget->setEnabled(false);

	getContainer()->setWidget(mWidget);
	getContainer()->setWindowTitle(name());

	cmd.setClass(this);

	cmd.connectEventNotify("notify-back-trace", &GdbBridgeEditor::processBackTrace);
	cmd.connectEventNotify("notify-breakpoint-moved", &GdbBridgeEditor::processBreakpointMoved);
	cmd.connectEventNotify("notify-breakpoint-add", &GdbBridgeEditor::processBreakpointAdd);
	cmd.connectEventNotify("notify-breakpoint-deleted", &GdbBridgeEditor::processBreakpointDeleted);
	cmd.connectEventNotify("notify-breakpoint-enabled", &GdbBridgeEditor::processBreakpointEnabled);
	cmd.connectEventNotify("notify-breakpoint-disabled", &GdbBridgeEditor::processBreakpointDisabled);
	cmd.connectEventNotify("notify-breakpoint-conditionned", &GdbBridgeEditor::processBreakpointConditionned);
	cmd.connectEventNotify("notify-breakpoint-unconditionned", &GdbBridgeEditor::processBreakpointUnConditionned);
	cmd.connectEventNotify("notify-goto-breakpoint", &GdbBridgeEditor::processGotoBreakpoint);
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
void GdbBridgeEditor::gdbStarted()
{
	GdbCore::gdbStarted();
}
//
void GdbBridgeEditor::gdbFinished()
{
	GdbCore::gdbFinished();
	mWidget->setEnabled(false);
}
//
void GdbBridgeEditor::targetLoaded()
{
	GdbCore::targetLoaded();
}
//
void GdbBridgeEditor::targetRunning()
{
	GdbCore::targetRunning();
	mWidget->setEnabled(false);
}
//
void GdbBridgeEditor::targetStopped()
{
	GdbCore::targetStopped();
	mWidget->setEnabled(true);
}
//
void GdbBridgeEditor::targetExited()
{
	GdbCore::targetExited();
	mWidget->setEnabled(false);
}
//
int GdbBridgeEditor::process(QGdbMessageCore m)
{
	return cmd.dispatchProcess(m);
}
//
int GdbBridgeEditor::processError(QGdbMessageCore m)
{

	// TODO
	mWidget->append(getParametre("answerGdb=", m.msg));

	return PROCESS_TERMINED;
}
//
void GdbBridgeEditor::processExit()
{
}
//
int GdbBridgeEditor::processBackTrace(QGdbMessageCore m)
{	
	emit backtrace(getParametre("fileName=", m.msg), getParametre("line=", m.msg).toInt());
	mWidget->append("move icon back trace in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointMoved(QGdbMessageCore m)
{
	emit breakpointMoved(getParametre("fileName=", m.msg), getParametre("beforLine=", m.msg).toInt(), getParametre("afterLine=", m.msg).toInt());
	mWidget->append("move icon breakpoint in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointAdd(QGdbMessageCore m)
{
	emit breakpoint(getParametre("fileName=", m.msg), getParametre("line=", m.msg).toInt(), true);
	mWidget->append("add icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointDeleted(QGdbMessageCore m)
{
	emit breakpoint(getParametre("fileName=", m.msg), getParametre("line=", m.msg).toInt(), false);
	mWidget->append("delete icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointEnabled(QGdbMessageCore m)
{
	emit breakpointEnabled(getParametre("fileName=", m.msg), getParametre("line=", m.msg).toInt(), true);
	mWidget->append("enable icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointDisabled(QGdbMessageCore m)
{
	emit breakpointEnabled(getParametre("fileName=", m.msg), getParametre("line=", m.msg).toInt(), false);
	mWidget->append("disable icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointConditionned(QGdbMessageCore m)
{
	emit breakpointConditionnaled(getParametre("fileName=", m.msg), getParametre("line=", m.msg).toInt(), true);
	mWidget->append("conditionned icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processBreakpointUnConditionned(QGdbMessageCore m)
{
	emit breakpointConditionnaled(getParametre("fileName=", m.msg), getParametre("line=", m.msg).toInt(), false);
	mWidget->append("unconditionned icon in editor");
	return PROCESS_TERMINED;
}
//
int GdbBridgeEditor::processGotoBreakpoint(QGdbMessageCore m)
{
	emit gotoBreakpoint(getParametre("fileName=", m.msg), getParametre("line=", m.msg).toInt());
	mWidget->append("goto breakpoint : " + m.msg);
	return PROCESS_TERMINED;
}
