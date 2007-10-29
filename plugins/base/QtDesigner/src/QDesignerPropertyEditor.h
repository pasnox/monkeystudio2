#ifndef QDESIGNERPROPERTYEDITOR_H
#define QDESIGNERPROPERTYEDITOR_H

#include <QDockWidget>

class QtDesignerChild;
class QDesignerFormWindowInterface;
class QDesignerPropertyEditorInterface;

class QDesignerPropertyEditor : public QDockWidget
{
	Q_OBJECT
	
public:
	QDesignerPropertyEditor( QtDesignerChild* );
	
private slots:
	void selectionChanged();
	void activeFormWindowChanged( QDesignerFormWindowInterface* );
	
private:
	QDesignerFormWindowInterface* mForm;
	QDesignerPropertyEditorInterface* mInterface;
};

#endif // QDESIGNERPROPERTYEDITOR_H
