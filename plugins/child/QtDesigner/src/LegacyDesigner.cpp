#include "LegacyDesigner.h"

#include <QFormBuilder>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowManagerInterface>

#include <QStyle>
#include <QStyleFactory>

#include <QDockWidget>
#include <QMainWindow>
#include <QBuffer>

Qt::WindowFlags LegacyDesigner::previewWindowFlags( const QWidget* widget )
{
#ifdef Q_WS_WIN
	Qt::WindowFlags windowFlags = ( widget->windowType() == Qt::Window ) ? Qt::Window | Qt::WindowMaximizeButtonHint : Qt::WindowFlags( Qt::Dialog );
#else
	Q_UNUSED( widget )
	// Only Dialogs have close buttons on Mac.
	// On Linux, we don't want an additional task bar item and we don't want a minimize button;
	// we want the preview to be on top.
	Qt::WindowFlags windowFlags = Qt::Dialog;
#endif
	return windowFlags;
}

QWidget* LegacyDesigner::fakeContainer( QWidget* w )
{
	// Prevent a dock widget from trying to dock to Designer's main window
	// (which can be found in the parent hierarchy in MDI mode) by
	// providing a fake mainwindow
	if ( QDockWidget* dock = qobject_cast<QDockWidget*>( w ) )
	{
		// Reparent: Clear modality, propagate title and resize outer container
		const QSize size = w->size();
		w->setWindowModality( Qt::NonModal );
		dock->setFeatures( dock->features() & ~( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable ) );
		dock->setAllowedAreas( Qt::LeftDockWidgetArea );
		QMainWindow* mw = new QMainWindow;
		mw->setWindowTitle( dock->windowTitle() );
		int leftMargin, topMargin, rightMargin, bottomMargin;
		mw->getContentsMargins( &leftMargin, &topMargin, &rightMargin, &bottomMargin );
		mw->addDockWidget( Qt::LeftDockWidgetArea, dock );
		mw->resize( size +QSize( leftMargin +rightMargin, topMargin +bottomMargin ) );
		return mw;
	}
	
	return w;
}

QWidget* LegacyDesigner::createPreview( const QDesignerFormWindowInterface* fw, const QString& style, QString* errorMessage )
{
	Q_UNUSED( errorMessage );
	QByteArray array = fw->contents().toUtf8();
	QBuffer buffer;
	buffer.setData( array );
	
	QFormBuilder builder;
	QWidget* widget = builder.load( &buffer );
	
	if ( !widget )
	{
		return widget;
	}
	
	QStyle* pstyle = QStyleFactory::create( style );
	
	if ( pstyle )
	{
		widget->setStyle( pstyle );
		widget->setPalette( pstyle->standardPalette() );
	}
	
	widget = fakeContainer( widget );
	widget->setParent( fw->window(), previewWindowFlags( widget ) );
	
	return widget;
}

QWidget* LegacyDesigner::showPreview( const QDesignerFormWindowInterface* fw, const QString& style, QString* errorMessage )
{
	enum { Spacing = 10 };
	/*
	if (QWidget *existingPreviewWidget = raise(fw, pc))
		return existingPreviewWidget;
	*/

	QWidget* widget = createPreview( fw, style, errorMessage );
	
	if ( !widget )
	{
		return 0;
	}
	
	// Install filter for Escape key
	widget->setAttribute( Qt::WA_DeleteOnClose, true );
	//widget->installEventFilter( this );

	/*
	switch ( d->m_mode )
	{
	case ApplicationModalPreview:
		// Cannot do this on the Mac as the dialog would have no close button
		widget->setWindowModality(Qt::ApplicationModal);
		break;
	case SingleFormNonModalPreview:
	case MultipleFormNonModalPreview:
	*/
		widget->setWindowModality( Qt::NonModal );
		QObject::connect( fw, SIGNAL( changed() ), widget, SLOT( close() ) );
		QObject::connect( fw, SIGNAL( destroyed() ), widget, SLOT( close() ) );
		//if (d->m_mode == SingleFormNonModalPreview)
			QObject::connect( fw->core()->formWindowManager(), SIGNAL( activeFormWindowChanged( QDesignerFormWindowInterface* ) ), widget, SLOT( close() ) );
		//break;
	//}
	
	// Semi-smart algorithm to position previews:
	// If its the first one, position relative to form.
	// 2nd, attempt to tile right (for comparing styles) or cascade
	const QSize size = widget->size();
	const bool firstPreview = true; //d->m_previews.empty();
	if (firstPreview) {
		widget->move(fw->mapToGlobal(QPoint(Spacing, Spacing)));
	} else {
	/*
		if (QWidget *lastPreview = d->m_previews.back().m_widget) {
			QDesktopWidget *desktop = qApp->desktop();
			const QRect lastPreviewGeometry = lastPreview->frameGeometry();
			const QRect availGeometry = desktop->availableGeometry(desktop->screenNumber(lastPreview));
			const QPoint newPos = lastPreviewGeometry.topRight() + QPoint(Spacing, 0);
			if (newPos.x() +  size.width() < availGeometry.right())
				widget->move(newPos);
			else
				widget->move(lastPreviewGeometry.topLeft() + QPoint(Spacing, Spacing));
		}
	*/
	}
	//d->m_previews.push_back(PreviewData(widget, fw, pc));
	widget->show();
	/*
	if (firstPreview)
		emit firstPreviewOpened();
	*/
	return widget;
}
