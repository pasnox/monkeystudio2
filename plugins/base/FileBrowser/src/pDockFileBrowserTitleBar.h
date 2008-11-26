#ifndef PDOCKFILEBROWSERTITLEBAR_H
#define PDOCKFILEBROWSERTITLEBAR_H

#include <QLabel>
#include <QDockWidget>

class QBoxLayout;

class pDockFileBrowserTitleBar : public QLabel
{
	Q_OBJECT

public:
	pDockFileBrowserTitleBar( QDockWidget* parent = 0 );
	virtual ~pDockFileBrowserTitleBar();
	
	virtual QSize sizeHint() const;
	QSize iconSize() const;
	QWidget* addAction( QAction* action, int index = -1 );
	QWidget* addSeparator( int index = -1 );

protected:
	QDockWidget* mDock;
	QBoxLayout* mBox1;
	QBoxLayout* mBox2;
	QAction* aOrientation;
	QAction* aFloat;
	QAction* aClose;

protected slots:
	void aOrientation_triggered();
	void aFloat_triggered();
	void featuresChanged( QDockWidget::DockWidgetFeatures features );
};

#endif // PDOCKFILEBROWSERTITLEBAR_H
