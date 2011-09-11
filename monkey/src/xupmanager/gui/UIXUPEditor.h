#ifndef UIXUPEDITOR_H
#define UIXUPEDITOR_H

#include "MonkeyExport.h"

#include <QDialog>

class Ui_UIXUPEditor;
class XUPPageEditor;
class XUPProjectItem;

class Q_MONKEY_EXPORT UIXUPEditor : public QDialog
{
	Q_OBJECT
	
public:
	UIXUPEditor( QWidget* parent = 0 );
	virtual ~UIXUPEditor();
	
	int insertPage( int index, XUPPageEditor* page );
	int addPage( XUPPageEditor* page );
	void addPages( const QList<XUPPageEditor*>& pages );
	XUPPageEditor* page( int index ) const;
	void removePage( int index );
	
	QList<XUPPageEditor*> pages() const;
	void setCurrentPage( int index );
	int currentPage() const;
	
	virtual void setupProject( XUPProjectItem* project );
	virtual bool showProjectFilesPage();

protected:
	Ui_UIXUPEditor* ui;
	XUPProjectItem* mProject;
	QList<XUPPageEditor*> mPages;
	
	virtual void setup( XUPProjectItem* project );
	virtual void finalize();

protected slots:
	void on_lwPages_currentRowChanged( int row );
	void accept();
};

#endif // UIXUPEDITOR_H
