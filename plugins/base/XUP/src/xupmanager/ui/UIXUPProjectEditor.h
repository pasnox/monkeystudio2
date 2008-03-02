#ifndef UIXUPPROJECTEDITOR_H
#define UIXUPPROJECTEDITOR_H

#include "ui_UIXUPProjectEditor.h"

class XUPItem;

class UIXUPProjectEditor : public QDialog, public Ui::UIXUPProjectEditor
{
	Q_OBJECT

public:
	UIXUPProjectEditor( XUPItem* project, QWidget* parent = 0 );

protected:
	XUPItem* mProject;
};

#endif // UIXUPPROJECTEDITOR_H
