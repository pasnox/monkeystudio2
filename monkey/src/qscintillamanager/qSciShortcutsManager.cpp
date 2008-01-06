/****************************************************************************
**
**         Created using Monkey Studio v1.7.0
** Author    : Kopats Andrei aka hlamer <hlamer at tut by>
** Project   : SciShortcutsManager
** FileName  : SciShortcutsManager.cpp
** Date      : 2007-10-27T10:52:20
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QtGlobal>
#include "qSciShortcutsManager.h"
#include "pMenuBar.h"
#include "pWorkspace.h"
#include "pEditor.h"
#include "pAbstractChild.h"
#include "MonkeyCore.h"

qSciShortcutsManager::qSciShortcutsManager (QObject* parent): QObject(parent)
{
    //Fill with all availible QScintila actions
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDOWN", tr("Move down one line"), 
		QIcon(), QString("Down"), tr(""),QsciScintilla::SCI_LINEDOWN);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDOWNEXTEND", tr("Extend selection down one line"), QIcon(),
		QString("Shift+Down"), tr(""),QsciScintilla::SCI_LINEDOWNEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDOWNRECTEXTEND", tr("Extend rectangular selection down one line"), 
		QIcon(), QString("Shift+Alt+Down"), tr(""),QsciScintilla::SCI_LINEDOWNRECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINESCROLLDOWN", tr("Scroll view down one line"), 
		QIcon(), QString("Ctrl+Down"), tr(""),QsciScintilla::SCI_LINESCROLLDOWN);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEUP", tr("Move up one line"), 
		QIcon(), QString("Up"), tr(""),QsciScintilla::SCI_LINEUP);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEUPEXTEND", tr("Extend selection up"), 
		QIcon(), QString("Shift+Up"), tr(""),QsciScintilla::SCI_LINEUPEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEUPRECTEXTEND", tr("Extend selection up one line"), 
		QIcon(), QString("Shift+Alt+Up"), tr(""),QsciScintilla::SCI_LINEUPRECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINESCROLLUP", tr("Scroll view up one line"), 
		QIcon(), QString("Ctrl+Up"), tr(""),QsciScintilla::SCI_LINESCROLLUP);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PARADOWN", tr("Move down one paragraph"), 
		QIcon(), QString("Ctrl+]"), tr(""),QsciScintilla::SCI_PARADOWN);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PARADOWNEXTEND", tr("Extend selection up one paragraph"), 
		QIcon(), QString("Shift+Ctrl+]"), tr(""),QsciScintilla::SCI_PARADOWNEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PARAUP", tr("Move down one paragraph"), 
		QIcon(), QString("Ctrl+["), tr(""),QsciScintilla::SCI_PARAUP);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PARAUPEXTEND", tr("Extend selection down one paragraph"), 
		QIcon(), QString("Shift+Ctrl+["), tr(""),QsciScintilla::SCI_PARAUPEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARLEFT", tr("Move left one character"), 
		QIcon(), QString("Left"), tr(""),QsciScintilla::SCI_CHARLEFT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARLEFTEXTEND", tr("Extend selection left one character"), 
		QIcon(), QString("Shift+Left"), tr(""),QsciScintilla::SCI_CHARLEFTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARLEFTRECTEXTEND", tr("Extend rectangular selection left one character"), 
		QIcon(), QString("Shift+Alt+Left"), tr(""),QsciScintilla::SCI_CHARLEFTRECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARRIGHT", tr("Move right one character"), 
		QIcon(), QString("Right"), tr(""),QsciScintilla::SCI_CHARRIGHT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARRIGHTEXTEND", tr("Extend selection right one character"), 
		QIcon(), QString("Shift+Right"), tr(""),QsciScintilla::SCI_CHARRIGHTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARRIGHTRECTEXTEND", tr("Extend rectangular selection right one character"), 
		QIcon(), QString("Shift+Alt+Right"), tr(""),QsciScintilla::SCI_CHARRIGHTRECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDLEFT", tr("Move left one word"), 
		QIcon(), QString("Ctrl+Left"), tr(""),QsciScintilla::SCI_WORDLEFT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDLEFTEXTEND", tr("Extend selection left one word"), 
		QIcon(), QString("Shift+Ctrl+Left"), tr(""),QsciScintilla::SCI_WORDLEFTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDRIGHT", tr("Move right one word"), 
		QIcon(), QString("Ctrl+Right"), tr(""),QsciScintilla::SCI_WORDRIGHT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDRIGHTEXTEND", tr("Extend selection right one word"), 
		QIcon(), QString("Ctrl+Shift+Right"), tr(""),QsciScintilla::SCI_WORDRIGHTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDLEFTEND", tr("Move left one word end"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_WORDLEFTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDLEFTENDEXTEND", tr("Extend selection left one word left"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_WORDLEFTENDEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDRIGHTEND", tr("Move right one word end"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_WORDRIGHTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDRIGHTENDEXTEND", tr("Extend selection right one word end"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_WORDRIGHTENDEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDPARTLEFT", tr("Move left one word part"), 
		QIcon(), QString("Ctrl+/"), tr(""),QsciScintilla::SCI_WORDPARTLEFT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDPARTLEFTEXTEND", tr("Extend selection left one word part "), 
		QIcon(), QString("Shift+Ctrl+/"), tr(""),QsciScintilla::SCI_WORDPARTLEFTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDPARTRIGHT", tr("Move right one word part"), 
		QIcon(), QString("Ctrl+\\"), tr(""),QsciScintilla::SCI_WORDPARTRIGHT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDPARTRIGHTEXTEND", tr("Extend selection right one word part"), 
		QIcon(), QString("Shift+Ctrl+\\"), tr(""),QsciScintilla::SCI_WORDPARTRIGHTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_HOME", tr("Move to line start"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_HOME);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEEXTEND", tr("Extend selection to line start"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_HOMEEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMERECTEXTEND", tr("Extend rectangular selection to line start"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_HOMERECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEDISPLAY", tr("Move to start of displayed line"), 
		QIcon(), QString("Alt+Home"), tr(""),QsciScintilla::SCI_HOMEDISPLAY);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEDISPLAYEXTEND", tr("Extend selection start of displayed line"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_HOMEDISPLAYEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEWRAP", tr("Home wrap"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_HOMEWRAP);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEWRAPEXTEND", tr("Extend selection on home wrap"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_HOMEWRAPEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOME", tr("Move to firsst VC in line"), 
		QIcon(), QString("Home"), tr(""),QsciScintilla::SCI_VCHOME);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOMEEXTEND", tr("Extend selection to first VC in line"), 
		QIcon(), QString("Shift+Home"), tr(""),QsciScintilla::SCI_VCHOMEEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOMERECTEXTEND", tr("Extend rectangular selection to firts VC in line"), 
		QIcon(), QString("Shift+Alt+Home"), tr(""),QsciScintilla::SCI_VCHOMERECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOMEWRAP", tr("VC Home wrap"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_VCHOMEWRAP);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOMEWRAPEXTEND", tr("Extend selection VC Home wrap"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_VCHOMEWRAPEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEEND", tr("Move to end of line"), 
		QIcon(), QString("End"), tr(""),QsciScintilla::SCI_LINEEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDEXTEND", tr("Extend selection to end of line"), 
		QIcon(), QString("Shift+End"), tr(""),QsciScintilla::SCI_LINEENDEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDRECTEXTEND", tr("Extend rectangular selection to end of line"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_LINEENDRECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDDISPLAY", tr("Move to end displayed line"), 
		QIcon(), QString("Alt+End"), tr(""),QsciScintilla::SCI_LINEENDDISPLAY);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDDISPLAYEXTEND", tr("Extend selection to end of displayed line"), 
		QIcon(), QString("Shift+Alt+End"), tr(""),QsciScintilla::SCI_LINEENDDISPLAYEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDWRAP", tr("Move to line end wrap"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_LINEENDWRAP);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDWRAPEXTEND", tr("Extend selection to line end wrap"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_LINEENDWRAPEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DOCUMENTSTART", tr("Move to document start"), 
		QIcon(), QString("Ctrl+Home"), tr(""),QsciScintilla::SCI_DOCUMENTSTART);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DOCUMENTSTARTEXTEND", tr("Extend selection to document start"), 
		QIcon(), QString("Shift+Ctrl+Home"), tr(""),QsciScintilla::SCI_DOCUMENTSTARTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DOCUMENTEND", tr("Move to document end"), 
		QIcon(), QString("Ctrl+End"), tr(""),QsciScintilla::SCI_DOCUMENTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DOCUMENTENDEXTEND", tr("Extend selection to document end"), 
		QIcon(), QString("Ctrl+Shift+End"), tr(""),QsciScintilla::SCI_DOCUMENTENDEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEUP", tr("Move up one page"), 
		QIcon(), QString("PgUp"), tr(""),QsciScintilla::SCI_PAGEUP);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEUPEXTEND", tr("Extend selection up one page"), 
		QIcon(), QString("Shift+PgUp"), tr(""),QsciScintilla::SCI_PAGEUPEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEUPRECTEXTEND", tr("Extend rectangular selection up one page"), 
		QIcon(), QString("Shift+Alt+PgUp"), tr(""),QsciScintilla::SCI_PAGEUPRECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEDOWN", tr("Move down one page"), 
		QIcon(), QString("PgDown"), tr(""),QsciScintilla::SCI_PAGEDOWN);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEDOWNEXTEND", tr("Extend selection down one page"), 
		QIcon(), QString("Shift+PgDown"), tr(""),QsciScintilla::SCI_PAGEDOWNEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEDOWNRECTEXTEND", tr("Extend rectangular selection down one page"), 
		QIcon(), QString("Shift+Alt+PgDown"), tr(""),QsciScintilla::SCI_PAGEDOWNRECTEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_STUTTEREDPAGEUP", tr("Move up one page stuttered"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_STUTTEREDPAGEUP);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_STUTTEREDPAGEUPEXTEND", tr("Extend selection up one page stuttered"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_STUTTEREDPAGEUPEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_STUTTEREDPAGEDOWN", tr("Move down one page stuttered"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_STUTTEREDPAGEDOWN);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_STUTTEREDPAGEDOWNEXTEND", tr("Extend selection down one page stuttered"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_STUTTEREDPAGEDOWNEXTEND);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DELETEBACK", tr("Backspace"), 
		QIcon(), QString("Backspace"), tr(""),QsciScintilla::SCI_DELETEBACK);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DELETEBACKNOTLINE", tr("Backspace not a line"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_DELETEBACKNOTLINE);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DELWORDLEFT", tr("Delete previous word"), 
		QIcon(), QString("Ctrl+Backspace"), tr(""),QsciScintilla::SCI_DELWORDLEFT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DELWORDRIGHT", tr("Delete next word"), 
		QIcon(), QString("Ctrl+Del"), tr(""),QsciScintilla::SCI_DELWORDRIGHT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DELLINELEFT", tr("Delete line tp left"), 
		QIcon(), QString("Shift+Ctrl+Backspace"), tr(""),QsciScintilla::SCI_DELLINELEFT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_DELLINERIGHT", tr("Delete line to right"), 
		QIcon(), QString("Shift+Ctrl+Del"), tr(""),QsciScintilla::SCI_DELLINERIGHT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDELETE", tr("Delete line"), 
		QIcon(), QString("Shift+Ctrl+L"), tr(""),QsciScintilla::SCI_LINEDELETE);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINECUT", tr("Cut line"), 
		QIcon(), QString("Ctrl+L"), tr(""),QsciScintilla::SCI_LINECUT);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINECOPY", tr("Copy line"), 
		QIcon(), QString("Shift+Ctrl+T"), tr(""),QsciScintilla::SCI_LINECOPY);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINETRANSPOSE", tr("Swap current and previous line"), 
		QIcon(), QString("Ctrl+T"), tr(""),QsciScintilla::SCI_LINETRANSPOSE);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDUPLICATE", tr("Duplicate line"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_LINEDUPLICATE);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_LOWERCASE", tr("To lower case"), 
		QIcon(), QString("Ctrl+U"), tr(""),QsciScintilla::SCI_LOWERCASE);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_UPPERCASE", tr("To upper case"), 
		QIcon(), QString("Shift+Ctrl+U"), tr(""),QsciScintilla::SCI_UPPERCASE);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_EDITTOGGLEOVERTYPE", tr("Edit toggle over type"), 
		QIcon(), QString("Ins"), tr(""),QsciScintilla::SCI_EDITTOGGLEOVERTYPE);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_FORMFEED", tr("Formfeed"), 
		QIcon(), QString(""), tr(""),QsciScintilla::SCI_FORMFEED);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_BACKTAB", tr("Delete one indent"), 
		QIcon(), QString("Shift+Tab"), tr(""),QsciScintilla::SCI_BACKTAB);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_SELECTIONDUPLICATE", tr("Duplicate selection"), 
		QIcon(), QString("Ctrl+D"), tr(""),QsciScintilla::SCI_SELECTIONDUPLICATE);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_CLEAR", tr("Delete"), 
		QIcon(), QString("Del"), tr(""),QsciScintilla::SCI_CLEAR);
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_SELECTALL", tr("Select All"), 
		QIcon(), QString("Ctrl+A"), tr(""),QsciScintilla::SCI_SELECTALL); 
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_ZOOMIN", tr("Zoom In"), 
		QIcon(), QString("Ctrl++"), tr(""),QsciScintilla::SCI_ZOOMIN); 
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_ZOOMOUT", tr("Zoom Out"), 
		QIcon(), QString("Ctrl+-"), tr(""),QsciScintilla::SCI_ZOOMOUT); 
    
	sactions << SciAction ( "mEdit/mAllCommands/SCI_SETZOOM", tr("Set Zoom"), 
		QIcon(), QString("Ctrl+/"), tr(""),QsciScintilla::SCI_SETZOOM); 
    
	foreach (SciAction sact, sactions)
    {
        QAction* qact = MonkeyCore::menuBar()->action( sact.name, sact.text, sact.icon, sact.defaultShortcut, sact.toolTip);
        qact->setProperty ("messageCode", sact.messageCode);
        connect (qact, SIGNAL (triggered()), this, SLOT (keyBoardShortcutPressed ()));
    }
}

void qSciShortcutsManager::keyBoardShortcutPressed ()
{
    Q_ASSERT (sender());
    int messageCode = sender()->property("messageCode").toInt();
    Q_ASSERT (messageCode);
    pAbstractChild* child = MonkeyCore::workspace()->currentChild();
    if (child)
    {
        pEditor* editor = child->currentEditor ();
        if (editor && editor->hasFocus())
        {
            editor->SendScintilla (messageCode);
        }
    }
}


