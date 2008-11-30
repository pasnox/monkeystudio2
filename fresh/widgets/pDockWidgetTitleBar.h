#ifndef PDOCKWIDGETTITLEBAR_H
#define PDOCKWIDGETTITLEBAR_H

#include "pDockWidget.h"

class QBoxLayout;

#include <QToolButton>
#include <QStylePainter>
#include <QStyleOptionToolButton>
#ifdef Q_OS_MAC
#include <QMacStyle>
#endif

class Q_MONKEY_EXPORT pDockWidgetButton : public QToolButton
{
	Q_OBJECT
	friend class pDockWidgetTitleBar;
	
public:
	enum Type { Orientation = 0, Float, Close };
	
	pDockWidgetButton( pDockWidgetButton::Type type, QWidget* parent = 0 )
		: QToolButton( parent )
	{
		mType = type;
		
		setFocusPolicy( Qt::NoFocus );
		
		switch ( mType )
		{
			case Orientation:
				setIcon( style()->standardIcon( QStyle::SP_ToolBarHorizontalExtensionButton ) );
				break;
			case Float:
				setIcon( style()->standardIcon( QStyle::SP_TitleBarNormalButton ) );
				break;
			case Close:
				setIcon( style()->standardIcon( QStyle::SP_TitleBarCloseButton ) );
				break;
		}
	}

protected:
	pDockWidgetButton::Type mType;
	
	void paintEvent( QPaintEvent* event )
	{
		Q_UNUSED( event );
		QPainter p( this );

		QRect r = rect();
		QStyleOptionToolButton opt;
		opt.init( this );
		opt.state |= QStyle::State_AutoRaise;

#ifdef Q_WS_MAC
		if ( !qobject_cast<QMacStyle*>( style() ) )
#endif
		{
			if ( isEnabled() && underMouse() && !isChecked() && !isDown() )
				opt.state |= QStyle::State_Raised;
			if ( isChecked() )
				opt.state |= QStyle::State_On;
			if ( isDown() )
				opt.state |= QStyle::State_Sunken;
			style()->drawPrimitive( QStyle::PE_PanelButtonTool, &opt, &p, this );
		}

		opt.icon = icon();
		opt.subControls = 0;
		opt.activeSubControls = 0;
		opt.features = QStyleOptionToolButton::None;
		opt.arrowType = Qt::NoArrow;
		int size = style()->pixelMetric( QStyle::PM_SmallIconSize, 0, this );
		opt.iconSize = QSize( size, size );
		style()->drawComplexControl( QStyle::CC_ToolButton, &opt, &p, this );
	}
};

class Q_MONKEY_EXPORT pDockWidgetTitleBar : public QWidget
{
	Q_OBJECT

public:
	pDockWidgetTitleBar( pDockWidget* parent = 0 );
	virtual ~pDockWidgetTitleBar();
	
	virtual QSize sizeHint() const;
	QSize buttonSize() const;
	QSize iconSize() const;
	QWidget* addToolButton( QToolButton* tb, int index = -1 );
	QWidget* addAction( QAction* action, int index = -1 );
	QWidget* addSeparator( int index = -1 );

protected:
	pDockWidget* mDock;
	QBoxLayout* mBox1;
	QBoxLayout* mBox2;
	pDockWidgetButton* bOrientation;
	pDockWidgetButton* bFloat;
	pDockWidgetButton* bClose;
	
	void paintEvent( QPaintEvent* event );

protected slots:
	void aOrientation_clicked();
	void aFloat_clicked();
	void featuresChanged( QDockWidget::DockWidgetFeatures features );
};

#endif // PDOCKWIDGETTITLEBAR_H
