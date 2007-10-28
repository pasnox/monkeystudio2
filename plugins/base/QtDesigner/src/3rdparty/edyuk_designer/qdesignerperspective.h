/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qtdesignerperspective.h
** Date : lun aug 28 12:01:59 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _QT_DESIGNER_PERSPECTIVE_H_
#define _QT_DESIGNER_PERSPECTIVE_H_

#include "main.h"

#include "edyukperspective.h"

#include <QPointer>
#include <QBasicTimer>

class QAction;
class QActionGroup;

class QDockWidget;

class QDesignerWidgetBox;
class QDesignerActionEditor;
class QDesignerPropertyEditor;
class QDesignerObjectInspector;
class QDesignerSignalSlotEditor;

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;

class QDesignerPerspective : public EdyukPerspective
{
	Q_OBJECT
	
	friend class DefaultCore;
	
	public:
		QDesignerPerspective(QProjectModel *m, QCodeModel *c);
		virtual ~QDesignerPerspective();
		
		virtual void retranslate();
		
		virtual QIcon icon() const;
		virtual QString name() const;
		
		virtual qmdiClient* open(const QString& filename);
		virtual bool canOpen(const QString& filename) const;
		
		virtual bool needsPrivateWorkspace(qmdiClient *c) const;
		
		virtual qmdiClient* createEmptyClient(qmdiClientFactory *f);
		
		QDesignerFormEditorInterface* handler();
		QDesignerFormWindowInterface* createForm();
		
	protected:
		virtual void setMainWindow(qmdiMainWindow *w);
		
	private slots:
		void editWidgets();
		void formChanged(QWidget *w);
		
	private:
		QAction *aEditWidgets;
		QActionGroup *aModes;
		
		QDesignerFormEditorInterface *pDesigner;
		
		QDesignerWidgetBox *pWidgetBox;
		QDesignerActionEditor *pActionEditor;
		QDesignerPropertyEditor *pPropertyEditor;
		QDesignerObjectInspector *pObjectInspector;
		QDesignerSignalSlotEditor *pSignalSlotEditor;
};

#endif // _QT_DESIGNER_PERSPECTIVE_H_
