#ifndef PTEMPLATEPREVIEWER_H
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
 
