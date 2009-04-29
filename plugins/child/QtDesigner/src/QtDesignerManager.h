#ifndef QTDESIGNERMANAGER_H
#define QTDESIGNERMANAGER_H

#include <QObject>
#include <QActionGroup>

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QDesignerWidgetBox;
class QDesignerActionEditor;
class QDesignerPropertyEditor;
class QDesignerObjectInspector;
class QDesignerSignalSlotEditor;
class QDesignerResourcesEditor;

class QtDesignerManager : public QObject
{
	Q_OBJECT
	
public:
	QtDesignerManager( QObject* parent = 0 );
	virtual ~QtDesignerManager();
	
	QDesignerFormEditorInterface* core();
	inline QAction* editWidgetsAction() const { return aEditWidgets; }
	inline QAction* previewFormAction() const { return aPreview; }
	inline QList<QAction*> modesActions() const { return aModes->actions(); }
	QDesignerFormWindowInterface* createNewForm( QWidget* parent = 0 );

public slots:
	void addFormWindow( QDesignerFormWindowInterface* form );
	void setActiveFormWindow( QDesignerFormWindowInterface* form );

protected:
	QDesignerFormEditorInterface* mCore;
	
	QActionGroup* aModes;
	QAction* aPreview;
	QAction* aEditWidgets;
	
	QDesignerWidgetBox* pWidgetBox;
	QDesignerActionEditor* pActionEditor;
	QDesignerPropertyEditor* pPropertyEditor;
	QDesignerObjectInspector* pObjectInspector;
	QDesignerSignalSlotEditor* pSignalSlotEditor;
	QDesignerResourcesEditor* pResourcesEditor;

protected slots:
	void editWidgets();
	void previewCurrentForm();
};

#endif // QTDESIGNERMANAGER_H
