/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pKeySequenceInput.h
 * LICENSE      : GPL
 * COMMENTARY   : This class allow to track shortcut and write them
 ********************************************************************************************************/
#ifndef PKEYSEQUENCEINPUT_H
#define PKEYSEQUENCEINPUT_H

#include "MonkeyExport.h"

#include <QLineEdit>

class Q_MONKEY_EXPORT pKeySequenceInput : public QLineEdit
{
public:
	pKeySequenceInput( QWidget* );

protected:
	QString checkKeyEvent( QKeyEvent*, bool );
	void keyPressEvent( QKeyEvent* );
	void keyReleaseEvent( QKeyEvent* );

};

#endif // PKEYSEQUENCEINPUT_H
