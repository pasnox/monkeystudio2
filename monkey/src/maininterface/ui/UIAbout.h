#ifndef UIABOUT_H
#define UIABOUT_H

#include "ui_UIAbout.h"
#include "MonkeyExport.h"
#include "QSingleton.h"

struct pDatas
{
	pDatas( const QString& n, const QString& l, const QString& p, const QString& e, const QString& c )
		: Name( n ), Login( l ), Pays( p ), Email( e ), Comment( c )
	{}

	QString Name;
	QString Login;
	QString Pays;
	QString Email;
	QString Comment;
};

class Q_MONKEY_EXPORT UIAbout : public QDialog, public Ui::UIAbout, public QSingleton<UIAbout>
{
	Q_OBJECT
	friend class QSingleton<UIAbout>;

public slots:
	void anchorClicked( const QUrl& );

private:
	UIAbout( QWidget* = 0 );

};

#endif // UIABOUT_H
