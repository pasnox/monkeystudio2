#ifndef PENVIRONMENTVARIABLESMANAGER_H
#define PENVIRONMENTVARIABLESMANAGER_H

#include "pSettings.h"
#include "pEnvironmentVariablesModel.h"

class pEnvironmentVariablesManager : public pSettings
{
	Q_OBJECT
	
public:
	pEnvironmentVariablesManager( QObject* parent = 0 );
	
	pEnvironmentVariablesModel::Variables variables() const;
	void setVariables( const pEnvironmentVariablesModel::Variables& variables );
	
	bool mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const;
	pEnvironmentVariablesModel::Variables mergeNewVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	bool removeUnmodifiedVariables( pEnvironmentVariablesModel::Variables& variables ) const;
	pEnvironmentVariablesModel::Variables removeUnmodifiedVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	QStringList variables( bool keepDisabled ) const;

protected:
	static const QString mSettingsKey;
};

#endif // PENVIRONMENTVARIABLESMANAGER_H
