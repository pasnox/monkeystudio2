#ifndef QCTAGSSENSESEARCHPOPUP_H
#define QCTAGSSENSESEARCHPOPUP_H

#include <QTreeView>

class qCtagsSenseSearchPopup : public QTreeView
{
	Q_OBJECT
	
public:
	qCtagsSenseSearchPopup( QWidget* widget );
	virtual ~qCtagsSenseSearchPopup();
	
	void setWidget( QWidget* widget );
	inline QWidget* widget() const { return mWidget; }
	
	void showPopup( const QRect& rect = QRect() );

protected:
	bool eatFocusOut;
	QWidget* mWidget;
	
	virtual bool eventFilter( QObject* object, QEvent* event );
	bool searchPopupEventFilter( QEvent* event );
};

#endif // QCTAGSSENSESEARCHPOPUP_H
