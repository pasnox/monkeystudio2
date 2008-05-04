#ifndef QUEUEDSTATUSBAR_H
#define QUEUEDSTATUSBAR_H

#include "fresh.h"
#include "qscintilla.h"

class QLabel;

class QueuedStatusBar : public pQueuedStatusBar
{
	Q_OBJECT
	
public:
	enum LabelType { ltSaveState = 0, ltEOLMode, ltIndentMode, ltCursorPosition };
	QueuedStatusBar( QWidget* parent = 0 );
	
	QLabel* label( QueuedStatusBar::LabelType type );

protected:
	QLabel* mLabels[4];
	
	void hideOrShow();

public slots:
	void setModified( bool modified );
	void setEOLMode( QsciScintilla::EolMode mode ); // -1 for none
	void setIndentMode( int mode ); // -1 for none, 0 for spaces, 1 for tabs
	void setCursorPosition( const QPoint& pos ); // QPoint( -1, -1 ) for none
};

#endif // QUEUEDSTATUSBAR_H
