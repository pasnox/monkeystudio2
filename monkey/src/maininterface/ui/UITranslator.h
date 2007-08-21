#ifndef UITRANSLATOR_H
#define UITRANSLATOR_H

#include "ui_UITranslator.h"
#include "MonkeyExport.h"
#include "QSingleton.h"

class Q_MONKEY_EXPORT UITranslator : public QDialog, public Ui::UITranslator, public QSingleton<UITranslator>
{
	Q_OBJECT
	friend class QSingleton<UITranslator>;

private:
	UITranslator( QWidget* = 0 );

	QString mTranslationsPath;

public slots:
	virtual void accept();
	void on_tbLocate_clicked();
	void on_tbReload_clicked();

};

#endif // UITRANSLATOR_H
