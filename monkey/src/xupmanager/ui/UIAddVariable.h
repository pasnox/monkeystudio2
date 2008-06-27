#ifndef UIADDVARIABLE_H
#define UIADDVARIABLE_H

#include <fresh.h>

#include "ui_UIAddVariable.h"

class Q_MONKEY_EXPORT UIAddVariable : public QDialog, public Ui::UIAddVariable
{
	Q_OBJECT

public:
	UIAddVariable( QWidget* parent = 0 );
	
	void setVariablesName( const QStringList& vars );
	QString getVariableName() const;
	void setCurrentVariableName( const QString& var );
	void setOperators( const QStringList& ops );
	QString getOperator() const;
	void setCurrentOperator( const QString& op );

protected:
	QStringList mVariables;
	QStringList mOperators;
};

#endif // UIADDVARIABLE_H
