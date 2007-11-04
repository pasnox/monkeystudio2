/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pTemplatePreviewer.h
** Date      : 2007-11-04T22:46:27
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef PTEMPLATEPREVIEWER_H
#define PTEMPLATEPREVIEWER_H

#include <QWidget>

class QLineEdit;
class pEditor;

class pTemplatePreviewer : public QWidget
{
	Q_OBJECT

protected:
	QLineEdit* mLineEdit;
	pEditor* mEditor;

public:
	pTemplatePreviewer( QWidget* = 0 );
	virtual ~pTemplatePreviewer();

	void setFileName( const QString& );
	QString fileName() const;

	void setDestination( const QString& );
	QString destination() const;

	void setOpen( bool );
	bool open() const;

	bool open( const QString& );

	pEditor* editor() const;

};

#endif // PTEMPLATEPREVIEWER_H
 
