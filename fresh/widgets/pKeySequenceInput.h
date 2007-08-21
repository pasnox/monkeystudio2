/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pKeySequenceInput.h
 * LICENSE      : GPL
 * COMMENTARY   : This class allow to track shortcut and write them
 ********************************************************************************************************/
#ifndef KEYSEQUENCEINPUT_H
#define KEYSEQUENCEINPUT_H

#include "MonkeyExport.h"

#include <QLineEdit>

class Q_MONKEY_EXPORT pKeySequenceInput : public QLineEdit
{
private:
	bool controlPressed;
	bool altPressed;
	bool winPressed;
	bool shiftPressed;

public:
	pKeySequenceInput( QWidget* );

protected:
	bool eventFilter( QObject*, QEvent* );

};

#endif // KEYSEQUENCEINPUT_H
