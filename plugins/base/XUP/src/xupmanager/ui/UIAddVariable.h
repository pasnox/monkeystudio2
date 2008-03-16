#ifndef UIADDVARIABLE_H
#define UIADDVARIABLE_H

#include "ui_UIAddVariable.h"

class UIAddVariable : public QDialog, public Ui::UIAddVariable
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
};

#endif // UIADDVARIABLE_H
