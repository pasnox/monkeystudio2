#include "UIAddVariable.h"

UIAddVariable::UIAddVariable( QWidget* parent )
	: QDialog( parent )
{ setupUi( this ); }

void UIAddVariable::setVariablesName( const QStringList& variables )
{
	cbVariableName->clear();
	mVariables = variables;
	cbVariableName->addItems( mVariables );
}

QString UIAddVariable::getVariableName() const
{ return cbVariableName->currentText(); }

void UIAddVariable::setCurrentVariableName( const QString& var )
{
	if ( !mVariables.contains( var ) )
	{
		mVariables << var;
		cbVariableName->addItem( var );
	}
	cbVariableName->setCurrentIndex( cbVariableName->findText( var ) );
}

void UIAddVariable::setOperators( const QStringList& operators )
{
	cbOperator->clear();
	mOperators = operators;
	cbOperator->addItems( mOperators );
}

QString UIAddVariable::getOperator() const
{ return cbOperator->currentText(); }

void UIAddVariable::setCurrentOperator( const QString& op )
{
	if ( !mOperators.contains( op ) )
	{
		mOperators << op;
		cbOperator->addItem( op );
	}
	cbOperator->setCurrentIndex( cbOperator->findText( op ) );
}
