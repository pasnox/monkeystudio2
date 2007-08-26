/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pKeySequenceInput.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class allow to track shortcut and write them
 ********************************************************************************************************/
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
	
	return QKeySequence( mKeys ).toString();
}
