#include "UIAddVariable.h"

UIAddVariable::UIAddVariable( QWidget* parent )
	: QDialog( parent )
{ setupUi( this ); }

void UIAddVariable::setVariablesName( const QStringList& variables )
{
	cbVariableName->clear();
	cbVariableName->addItems( variables );
}

QString UIAddVariable::getVariableName() const
{ return cbVariableName->currentText(); }

void UIAddVariable::setCurrentVariableName( const QString& var )
{ cbVariableName->setCurrentIndex( cbVariableName->findText( var ) ); }

void UIAddVariable::setOperators( const QStringList& operators )
{
	cbOperator->clear();
	cbOperator->addItems( operators );
}

QString UIAddVariable::getOperator() const
{ return cbOperator->currentText(); }

void UIAddVariable::setCurrentOperator( const QString& op )
{ cbOperator->setCurrentIndex( cbOperator->findText( op ) ); }
