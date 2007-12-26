#ifndef PDOCKWIDGET_H
#define PDOCKWIDGET_H

#include "MonkeyExport.h"

#include <QDockWidget>

class Q_MONKEY_EXPORT pDockWidget : public QDockWidget
{
	Q_OBJECT
	
public:
	pDockWidget( const QString&, QWidget* = 0, Qt::WindowFlags = 0 );
	pDockWidget( QWidget* = 0, Qt::WindowFlags = 0 );
	QSize sizeHint() const;

public slots:    void setVisible( bool );

protected:
	QSize contentsSize() const;
	QSize mSize;

};

#endif // PDOCKWIDGET_H
