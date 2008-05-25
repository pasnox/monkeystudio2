#ifndef UISETTING_H
#define UISETTING_H
//
#include "ui_UIsetting.h"
//

class UIsetting : public QWidget, public Ui::UIsetting
{
	Q_OBJECT
	//
public:
	UIsetting( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
};
//
#endif // UISETTING_H
