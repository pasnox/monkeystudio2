#include "pDockWidget.h"

#include <QStyle>

pDockWidget::pDockWidget( const QString& t, QWidget* w, Qt::WindowFlags f )
	: QDockWidget( t, w, f )
{}

pDockWidget::pDockWidget( QWidget* w, Qt::WindowFlags f )
	: QDockWidget( w, f )
{}

QSize pDockWidget::contentsSize() const
{
	QSize contents = size();
	int fw = style()->pixelMetric( QStyle::PM_DockWidgetFrameWidth );
	QSize frame( 2 *fw, fw );
#ifdef Q_OS_WIN
	frame -= QSize( 0, 3 );
#endif
	contents -= frame;
	return contents;
}

QSize pDockWidget::sizeHint() const
{ return mSize.isValid() && !isFloating() ? mSize : QDockWidget::sizeHint(); }

void pDockWidget::setVisible( bool b )
{
	if ( !b && !isFloating() )		mSize = contentsSize();	QDockWidget::setVisible( b );
}
