/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : DesignerClient.h
** Date : lun aug 28 18:50:31 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _DESIGNER_CLIENT_H_
#define _DESIGNER_CLIENT_H_

#include "main.h"

#include "qmdiwidget.h"

#include <QBasicTimer>

class QString;
class QScrollArea;
class QDesignerFormWindowInterface;

class QDesignerClient : public qmdiWidget
{
	Q_OBJECT
	
	public:
		QDesignerClient(QDesignerFormWindowInterface *i, const QString& f,
						QWidget *w = 0);
		virtual ~QDesignerClient();
		
		QDesignerFormWindowInterface* interface() const;
		
	public slots:
		virtual void save();
		
	protected:
		virtual void timerEvent(QTimerEvent *e);
		
	private slots:
		void updateChanged();
		
		#if QT_VERSION >= 0x040200
		void geometryChanged();
		#endif
		
	private:
		QBasicTimer t;
		
		QAction *aUndo, *aRedo,
				*aCut, *aCopy, *aPaste, *aDelete,
				*aVertical, *aHorizontal, *aGrid, *aBreak,
				*aAdjust,
				*aLower, *aRaise,
				*aSplitH, *aSplitV,
				*aPreview;
		
		QDesignerFormWindowInterface *pInterface;
};

#endif // _DESIGNER_CLIENT_H_
