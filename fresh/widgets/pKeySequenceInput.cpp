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
{
	controlPressed = false;
	altPressed = false;
	winPressed = false;
	shiftPressed = false;
	installEventFilter( this );
}

bool pKeySequenceInput::eventFilter( QObject* o, QEvent* e )
{
	// get type
	int i = e->type();

	// if keypress
	if ( i == QEvent::KeyPress )
	{
		QKeyEvent* ke = static_cast<QKeyEvent*>( e );
		switch ( ke->key() )
		{
		case Qt::Key_Control:
			controlPressed = true;
			break;
		case Qt::Key_Alt:
			altPressed = true;
			break;
		case Qt::Key_Shift:
			shiftPressed = true;
			break;
		case Qt::Key_Super_R:
			winPressed = true;
			break;
		case Qt::Key_Super_L:
			winPressed = true;
			break;
		default:
			QString s;
			if ( shiftPressed )
				s += "Shift+";
			if ( controlPressed )
				s += "Ctrl+";
			if ( altPressed )
				s +="Alt+";
			if ( winPressed )
				s +="Win+";
			s += QKeySequence( ke->key() ).toString( QKeySequence::PortableText );
			setText( s );
		}
	}
	// if keyrelease
	else if ( i == QEvent::KeyRelease )
	{
		QKeyEvent* ke = static_cast<QKeyEvent*>( e );
		switch ( ke->key() )
		{
		case Qt::Key_Control:
			controlPressed = false;
			break;
		case Qt::Key_Alt:
			altPressed = false;
			break;
		case Qt::Key_Shift:
			shiftPressed = false;
			break;
		case Qt::Key_Super_R:
			winPressed = false;
			break;
		case Qt::Key_Super_L:
			winPressed = false;
			break;
		}
	}

	// standard event processing
	return QLineEdit::eventFilter( o, e );
}
