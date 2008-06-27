#ifndef UIREGEXPEDITOR_H
#define UIREGEXPEDITOR_H

#include "ui_UIRegExpEditor.h"

class UIRegExpEditor : public QMainWindow, public Ui::UIRegExpEditor
{
	Q_OBJECT

public:
	UIRegExpEditor( QWidget* parent = 0 );

protected slots:
	void on_tbFind_clicked();
};

Q_DECLARE_METATYPE( QRegExp::PatternSyntax )

#endif // UIREGEXPEDITOR_H
