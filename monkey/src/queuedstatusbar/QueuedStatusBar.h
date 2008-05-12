#ifndef QUEUEDSTATUSBAR_H
#define QUEUEDSTATUSBAR_H

#include "fresh.h"
#include "qscintilla.h"

class QLabel;
class QStackedWidget;

class Q_MONKEY_EXPORT QueuedStatusBar : public pQueuedStatusBar
{
	Q_OBJECT
	
public:
	enum LabelType { ltMessage = 0, ltCursorPosition, ltSaveState, ltEOLMode, ltIndentMode };
	QueuedStatusBar( QWidget* parent = 0 );
	
	QLabel* label( QueuedStatusBar::LabelType type );

protected:
	QStackedWidget* mStacked;
	QWidget* mWidgetLabels;
	QLabel* mLabels[5];
	int mHeight;
	
	void resizeEvent( QResizeEvent* event );

public slots:
	void messageShown( const pQueuedMessage& message );
	void messageFinished();
	void setMessage( const QString& message );
	void setModified( bool modified );
	void setEOLMode( QsciScintilla::EolMode mode ); // -1 for none
	void setIndentMode( int mode ); // -1 for none, 0 for spaces, 1 for tabs
	void setCursorPosition( const QPoint& pos ); // QPoint( -1, -1 ) for none
};

#endif // QUEUEDSTATUSBAR_H
