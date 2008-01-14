/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pKeySequenceInput.cpp
** Date      : 2008-01-14T00:27:45
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
**
****************************************************************************/



#include "pKeySequenceInput.h"

#include <QKeySequence>
#include <QKeyEvent>

pKeySequenceInput::pKeySequenceInput( QWidget* w )
	: QLineEdit( w )
{}

void pKeySequenceInput::keyPressEvent( QKeyEvent* e )
{	
	// return if auto repeat
	if ( e->isAutoRepeat() )
		return;
	
	// if user press something, sequence is not finished
	setProperty( "Finished", false );
	
	// show current sequence
	setText( checkKeyEvent( e, true ) );
}

void pKeySequenceInput::keyReleaseEvent( QKeyEvent* e )
{
	// return if auto repeat
	if ( e->isAutoRepeat() )
		return;
	
	// check if sequence is finished or not
	if ( property( "Finished" ).toBool() )
		return;
	
	// show current sequence
	setText( checkKeyEvent( e, false ) );
}

QString pKeySequenceInput::checkKeyEvent( QKeyEvent* e, bool b )
{
	// or-ed keys
	int mKeys = 0;
	
	// check modifiers pressed
	if ( e->modifiers() & Qt::ControlModifier )
		mKeys |= Qt::ControlModifier;
	if ( e->modifiers() & Qt::AltModifier )
		mKeys |= Qt::AltModifier;
	if ( e->modifiers() & Qt::ShiftModifier )
		mKeys |= Qt::ShiftModifier;
	if ( e->modifiers() & Qt::MetaModifier )
		mKeys |= Qt::MetaModifier;
	
	// if key press event
	if ( b )
	{
		// get press key
		switch( e->key() )
		{
			// this keys can't be used
			case Qt::Key_Shift:
			case Qt::Key_Control:
			case Qt::Key_Meta:
			case Qt::Key_Alt:
			case Qt::Key_AltGr:
			case Qt::Key_Super_L:
			case Qt::Key_Super_R:
			case Qt::Key_Menu:
			case Qt::Key_Hyper_L:
			case Qt::Key_Hyper_R:
			case Qt::Key_Help:
			case Qt::Key_Direction_L:
			case Qt::Key_Direction_R:
				break;
			default:
				// add pressed key
				mKeys |= e->key();
				
				// set sequence finished
				setProperty( "Finished", true );
				break;
		}
	}
	
	// return human readable key sequence
	return QKeySequence( mKeys ).toString();
}
