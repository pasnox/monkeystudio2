/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "qSciShortcutsManager.h"
#include "pEditor.h"
#include "workspace/pAbstractChild.h"
#include "coremanager/MonkeyCore.h"
#include "workspace/pWorkspace.h"

#include <pMenuBar.h>

qSciShortcutsManager::qSciShortcutsManager (QObject* parent): QObject(parent)
{
    //Fill with all availible QScintila actions
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDOWNEXTEND", QApplication::translate("QsciCommand","Extend selection down one line"), QIcon(),
        QString("Shift+Down"), QString::null,QsciScintilla::SCI_LINEDOWNEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDOWNRECTEXTEND", QApplication::translate("QsciCommand","Extend rectangular selection down one line"), 
        QIcon(), QString("Shift+Alt+Down"), QString::null,QsciScintilla::SCI_LINEDOWNRECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINESCROLLDOWN", QApplication::translate("QsciCommand","Scroll view down one line"), 
        QIcon(), QString("Ctrl+Down"), QString::null,QsciScintilla::SCI_LINESCROLLDOWN);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEUPEXTEND", QApplication::translate("QsciCommand","Extend selection up"), 
        QIcon(), QString("Shift+Up"), QString::null,QsciScintilla::SCI_LINEUPEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEUPRECTEXTEND", QApplication::translate("QsciCommand","Extend selection up one line"), 
        QIcon(), QString("Shift+Alt+Up"), QString::null,QsciScintilla::SCI_LINEUPRECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINESCROLLUP", QApplication::translate("QsciCommand","Scroll view up one line"), 
        QIcon(), QString("Ctrl+Up"), QString::null,QsciScintilla::SCI_LINESCROLLUP);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PARADOWN", QApplication::translate("QsciCommand","Move down one paragraph"), 
        QIcon(), QString("Ctrl+]"), QString::null,QsciScintilla::SCI_PARADOWN);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PARADOWNEXTEND", QApplication::translate("QsciCommand","Extend selection up one paragraph"), 
        QIcon(), QString("Shift+Ctrl+]"), QString::null,QsciScintilla::SCI_PARADOWNEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PARAUP", QApplication::translate("QsciCommand","Move down one paragraph"), 
        QIcon(), QString("Ctrl+["), QString::null,QsciScintilla::SCI_PARAUP);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PARAUPEXTEND", QApplication::translate("QsciCommand","Extend selection down one paragraph"), 
        QIcon(), QString("Shift+Ctrl+["), QString::null,QsciScintilla::SCI_PARAUPEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARLEFTEXTEND", QApplication::translate("QsciCommand","Extend selection left one character"), 
        QIcon(), QString("Shift+Left"), QString::null,QsciScintilla::SCI_CHARLEFTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARLEFTRECTEXTEND", QApplication::translate("QsciCommand","Extend rectangular selection left one character"), 
        QIcon(), QString("Shift+Alt+Left"), QString::null,QsciScintilla::SCI_CHARLEFTRECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARRIGHTEXTEND", QApplication::translate("QsciCommand","Extend selection right one character"), 
        QIcon(), QString("Shift+Right"), QString::null,QsciScintilla::SCI_CHARRIGHTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_CHARRIGHTRECTEXTEND", QApplication::translate("QsciCommand","Extend rectangular selection right one character"), 
        QIcon(), QString("Shift+Alt+Right"), QString::null,QsciScintilla::SCI_CHARRIGHTRECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDLEFT", QApplication::translate("QsciCommand","Move left one word"), 
        QIcon(), QString("Ctrl+Left"), QString::null,QsciScintilla::SCI_WORDLEFT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDLEFTEXTEND", QApplication::translate("QsciCommand","Extend selection left one word"), 
        QIcon(), QString("Shift+Ctrl+Left"), QString::null,QsciScintilla::SCI_WORDLEFTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDRIGHT", QApplication::translate("QsciCommand","Move right one word"), 
        QIcon(), QString("Ctrl+Right"), QString::null,QsciScintilla::SCI_WORDRIGHT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDRIGHTEXTEND", QApplication::translate("QsciCommand","Extend selection right one word"), 
        QIcon(), QString("Ctrl+Shift+Right"), QString::null,QsciScintilla::SCI_WORDRIGHTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDLEFTEND", QApplication::translate("QsciCommand","Move left one word end"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_WORDLEFTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDLEFTENDEXTEND", QApplication::translate("QsciCommand","Extend selection left one word left"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_WORDLEFTENDEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDRIGHTEND", QApplication::translate("QsciCommand","Move right one word end"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_WORDRIGHTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDRIGHTENDEXTEND", QApplication::translate("QsciCommand","Extend selection right one word end"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_WORDRIGHTENDEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDPARTLEFT", QApplication::translate("QsciCommand","Move left one word part"), 
        QIcon(), QString("Ctrl+/"), QString::null,QsciScintilla::SCI_WORDPARTLEFT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDPARTLEFTEXTEND", QApplication::translate("QsciCommand","Extend selection left one word part "), 
        QIcon(), QString("Shift+Ctrl+/"), QString::null,QsciScintilla::SCI_WORDPARTLEFTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDPARTRIGHT", QApplication::translate("QsciCommand","Move right one word part"), 
        QIcon(), QString("Ctrl+\\"), QString::null,QsciScintilla::SCI_WORDPARTRIGHT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_WORDPARTRIGHTEXTEND", QApplication::translate("QsciCommand","Extend selection right one word part"), 
        QIcon(), QString("Shift+Ctrl+\\"), QString::null,QsciScintilla::SCI_WORDPARTRIGHTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_HOME", QApplication::translate("QsciCommand","Move to line start"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_HOME);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEEXTEND", QApplication::translate("QsciCommand","Extend selection to line start"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_HOMEEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMERECTEXTEND", QApplication::translate("QsciCommand","Extend rectangular selection to line start"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_HOMERECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEDISPLAY", QApplication::translate("QsciCommand","Move to start of displayed line"), 
        QIcon(), QString("Alt+Home"), QString::null,QsciScintilla::SCI_HOMEDISPLAY);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEDISPLAYEXTEND", QApplication::translate("QsciCommand","Extend selection start of displayed line"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_HOMEDISPLAYEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEWRAP", QApplication::translate("QsciCommand","Home wrap"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_HOMEWRAP);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_HOMEWRAPEXTEND", QApplication::translate("QsciCommand","Extend selection on home wrap"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_HOMEWRAPEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOME", QApplication::translate("QsciCommand","Move to firsst VC in line"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_VCHOME);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOMEEXTEND", QApplication::translate("QsciCommand","Extend selection to first VC in line"), 
        QIcon(), QString("Shift+Home"), QString::null,QsciScintilla::SCI_VCHOMEEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOMERECTEXTEND", QApplication::translate("QsciCommand","Extend rectangular selection to firts VC in line"), 
        QIcon(), QString("Shift+Alt+Home"), QString::null,QsciScintilla::SCI_VCHOMERECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOMEWRAP", QApplication::translate("QsciCommand","VC Home wrap"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_VCHOMEWRAP);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_VCHOMEWRAPEXTEND", QApplication::translate("QsciCommand","Extend selection VC Home wrap"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_VCHOMEWRAPEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEEND", QApplication::translate("QsciCommand","Move to end of line"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_LINEEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDEXTEND", QApplication::translate("QsciCommand","Extend selection to end of line"), 
        QIcon(), QString("Shift+End"), QString::null,QsciScintilla::SCI_LINEENDEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDRECTEXTEND", QApplication::translate("QsciCommand","Extend rectangular selection to end of line"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_LINEENDRECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDDISPLAY", QApplication::translate("QsciCommand","Move to end displayed line"), 
        QIcon(), QString("Alt+End"), QString::null,QsciScintilla::SCI_LINEENDDISPLAY);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDDISPLAYEXTEND", QApplication::translate("QsciCommand","Extend selection to end of displayed line"), 
        QIcon(), QString("Shift+Alt+End"), QString::null,QsciScintilla::SCI_LINEENDDISPLAYEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDWRAP", QApplication::translate("QsciCommand","Move to line end wrap"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_LINEENDWRAP);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEENDWRAPEXTEND", QApplication::translate("QsciCommand","Extend selection to line end wrap"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_LINEENDWRAPEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DOCUMENTSTART", QApplication::translate("QsciCommand","Move to document start"), 
        QIcon(), QString("Ctrl+Home"), QString::null,QsciScintilla::SCI_DOCUMENTSTART);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DOCUMENTSTARTEXTEND", QApplication::translate("QsciCommand","Extend selection to document start"), 
        QIcon(), QString("Shift+Ctrl+Home"), QString::null,QsciScintilla::SCI_DOCUMENTSTARTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DOCUMENTEND", QApplication::translate("QsciCommand","Move to document end"), 
        QIcon(), QString("Ctrl+End"), QString::null,QsciScintilla::SCI_DOCUMENTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DOCUMENTENDEXTEND", QApplication::translate("QsciCommand","Extend selection to document end"), 
        QIcon(), QString("Ctrl+Shift+End"), QString::null,QsciScintilla::SCI_DOCUMENTENDEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEUP", QApplication::translate("QsciCommand","Move up one page"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_PAGEUP);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEUPEXTEND", QApplication::translate("QsciCommand","Extend selection up one page"), 
        QIcon(), QString("Shift+PgUp"), QString::null,QsciScintilla::SCI_PAGEUPEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEUPRECTEXTEND", QApplication::translate("QsciCommand","Extend rectangular selection up one page"), 
        QIcon(), QString("Shift+Alt+PgUp"), QString::null,QsciScintilla::SCI_PAGEUPRECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEDOWN", QApplication::translate("QsciCommand","Move down one page"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_PAGEDOWN);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEDOWNEXTEND", QApplication::translate("QsciCommand","Extend selection down one page"), 
        QIcon(), QString("Shift+PgDown"), QString::null,QsciScintilla::SCI_PAGEDOWNEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_PAGEDOWNRECTEXTEND", QApplication::translate("QsciCommand","Extend rectangular selection down one page"), 
        QIcon(), QString("Shift+Alt+PgDown"), QString::null,QsciScintilla::SCI_PAGEDOWNRECTEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_STUTTEREDPAGEUP", QApplication::translate("QsciCommand","Move up one page stuttered"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_STUTTEREDPAGEUP);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_STUTTEREDPAGEUPEXTEND", QApplication::translate("QsciCommand","Extend selection up one page stuttered"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_STUTTEREDPAGEUPEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_STUTTEREDPAGEDOWN", QApplication::translate("QsciCommand","Move down one page stuttered"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_STUTTEREDPAGEDOWN);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_STUTTEREDPAGEDOWNEXTEND", QApplication::translate("QsciCommand","Extend selection down one page stuttered"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_STUTTEREDPAGEDOWNEXTEND);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DELETEBACKNOTLINE", QApplication::translate("QsciCommand","Backspace not a line"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_DELETEBACKNOTLINE);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DELWORDLEFT", QApplication::translate("QsciCommand","Delete previous word"), 
        QIcon(), QString("Ctrl+Backspace"), QString::null,QsciScintilla::SCI_DELWORDLEFT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DELWORDRIGHT", QApplication::translate("QsciCommand","Delete next word"), 
        QIcon(), QString("Ctrl+Del"), QString::null,QsciScintilla::SCI_DELWORDRIGHT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DELLINELEFT", QApplication::translate("QsciCommand","Delete line tp left"), 
        QIcon(), QString("Shift+Ctrl+Backspace"), QString::null,QsciScintilla::SCI_DELLINELEFT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_DELLINERIGHT", QApplication::translate("QsciCommand","Delete line to right"), 
        QIcon(), QString("Shift+Ctrl+Del"), QString::null,QsciScintilla::SCI_DELLINERIGHT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDELETE", QApplication::translate("QsciCommand","Delete line"), 
        QIcon(), QString("Shift+Ctrl+L"), QString::null,QsciScintilla::SCI_LINEDELETE);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINECUT", QApplication::translate("QsciCommand","Cut line"), 
        QIcon(), QString("Ctrl+L"), QString::null,QsciScintilla::SCI_LINECUT);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINECOPY", QApplication::translate("QsciCommand","Copy line"), 
        QIcon(), QString("Shift+Ctrl+T"), QString::null,QsciScintilla::SCI_LINECOPY);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINETRANSPOSE", QApplication::translate("QsciCommand","Swap current and previous line"), 
        QIcon(), QString("Ctrl+T"), QString::null,QsciScintilla::SCI_LINETRANSPOSE);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LINEDUPLICATE", QApplication::translate("QsciCommand","Duplicate line"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_LINEDUPLICATE);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_LOWERCASE", QApplication::translate("QsciCommand","To lower case"), 
        QIcon(), QString("Ctrl+U"), QString::null,QsciScintilla::SCI_LOWERCASE);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_UPPERCASE", QApplication::translate("QsciCommand","To upper case"), 
        QIcon(), QString("Shift+Ctrl+U"), QString::null,QsciScintilla::SCI_UPPERCASE);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_EDITTOGGLEOVERTYPE", QApplication::translate("QsciCommand","Edit toggle over type"), 
        QIcon(), QString("Ins"), QString::null,QsciScintilla::SCI_EDITTOGGLEOVERTYPE);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_FORMFEED", QApplication::translate("QsciCommand","Formfeed"), 
        QIcon(), QString(""), QString::null,QsciScintilla::SCI_FORMFEED);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_BACKTAB", QApplication::translate("QsciCommand","Delete one indent"), 
        QIcon(), QString("Shift+Tab"), QString::null,QsciScintilla::SCI_BACKTAB);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_SELECTIONDUPLICATE", QApplication::translate("QsciCommand","Duplicate selection"), 
        QIcon(), QString("Ctrl+D"), QString::null,QsciScintilla::SCI_SELECTIONDUPLICATE);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_CLEAR", QApplication::translate("QsciCommand","Delete"), 
        QIcon(), QString("Del"), QString::null,QsciScintilla::SCI_CLEAR);
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_SELECTALL", QApplication::translate("QsciCommand","Select All"), 
        QIcon(), QString("Ctrl+A"), QString::null,QsciScintilla::SCI_SELECTALL); 
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_ZOOMIN", QApplication::translate("QsciCommand","Zoom In"), 
        QIcon(), QString("Ctrl++"), QString::null,QsciScintilla::SCI_ZOOMIN); 
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_ZOOMOUT", QApplication::translate("QsciCommand","Zoom Out"), 
        QIcon(), QString("Ctrl+-"), QString::null,QsciScintilla::SCI_ZOOMOUT); 
    
    sactions << SciAction ( "mEdit/mAllCommands/SCI_SETZOOM", QApplication::translate("QsciCommand","Set Zoom"), 
        QIcon(), QString("Ctrl+/"), QString::null,QsciScintilla::SCI_SETZOOM);
    
    // bookmarks actions
    sactions << SciAction( "mEdit/mBookmarks/SCI_MARKERADD", QApplication::translate("QsciCommand","Define"), 
        QIcon( ":/editor/bookmark_add.png" ), QString( "Ctrl+B" ), QString::null, QsciScintilla::SCI_MARKERADD );
    
    sactions << SciAction ( "mEdit/mBookmarks/SCI_MARKERDELETEALL", QApplication::translate("QsciCommand", "Delete All" ), 
        QIcon(), QString(), QString::null, QsciScintilla::SCI_MARKERDELETEALL );
    
    sactions << SciAction( "mEdit/mBookmarks/SCI_MARKERPREVIOUS", QApplication::translate("QsciCommand", "Previous" ), 
        QIcon(), QString( "Alt+Up" ), QString::null, QsciScintilla::SCI_MARKERPREVIOUS );
    
    sactions << SciAction( "mEdit/mBookmarks/SCI_MARKERNEXT", QApplication::translate("QsciCommand", "Next" ), 
        QIcon(), QString( "Alt+Down" ), QString::null, QsciScintilla::SCI_MARKERNEXT );
    
    foreach( SciAction sact, sactions )
    {
        QAction* qact = MonkeyCore::menuBar()->action( sact.name, sact.text, sact.icon, sact.defaultShortcut, sact.toolTip );
        qact->setData( sact.messageCode );
        connect( qact, SIGNAL( triggered() ), this, SLOT( keyBoardShortcutPressed() ) );
    }
}

void qSciShortcutsManager::keyBoardShortcutPressed ()
{
    Q_ASSERT( sender() );
    int messageCode = qobject_cast<QAction*>( sender() )->data().toInt();
    Q_ASSERT( messageCode );
    pAbstractChild* child = MonkeyCore::workspace()->currentDocument();
    if ( child )
    {
        pEditor* editor = child->editor();
        if ( editor && ( editor->hasFocus() || child->isWindow() ) )
        {
            const QPoint mCursorPos = editor->cursorPosition();
            if ( messageCode == QsciScintilla::SCI_MARKERADD )
            {
                if ( editor->markerAtLine( mCursorPos.y(), pEditor::mdBookmark ) )
                    editor->markerDelete( mCursorPos.y(), pEditor::mdBookmark );
                else
                    editor->markerAdd( mCursorPos.y(), pEditor::mdBookmark );
            }
            else if ( messageCode == QsciScintilla::SCI_MARKERDELETEALL )
                editor->markerDeleteAll( pEditor::mdBookmark );
            else if ( messageCode == QsciScintilla::SCI_MARKERPREVIOUS )
                editor->setCursorPosition( editor->markerFindPrevious( mCursorPos.y() -1, pEditor::mdBookmark ), 0 );
            else if ( messageCode == QsciScintilla::SCI_MARKERNEXT )
                editor->setCursorPosition( editor->markerFindNext( mCursorPos.y() +1, pEditor::mdBookmark ), 0 );
            else
                editor->SendScintilla( messageCode );
        }
    }
}
