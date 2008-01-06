/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pSettings.h
 * LICENSE      : GPL
 * COMMENTARY   : This class allow a singletonized QSettings for your application
 ********************************************************************************************************/
#ifndef PSETTINGS_H
#define PSETTINGS_H

#include "MonkeyExport.h"

#include <QSettings>

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "My Application"
#endif

#ifndef PROGRAM_VERSION
#define PROGRAM_VERSION "1.0.0"
#endif


class QMainWindow;

class Q_MONKEY_EXPORT pSettings : public QSettings
{
	Q_OBJECT

public:
	pSettings( QObject* = 0 );
	~pSettings();
	static void setIniInformations( const QString& = PROGRAM_NAME, const QString& = PROGRAM_VERSION );

	static void remove( const QString& key );
	static void setValue( const QString& key, const QVariant& value );
	static QVariant value( const QString& key, const QVariant& defaultValue = QVariant() );

	static QString programName();
	static QString programVersion();

	virtual void restoreState( QMainWindow* );
	virtual void saveState( QMainWindow* );

	virtual void setDefaultSettings() {};

private:
	static QString mProgramName;
	static QString mProgramVersion;
};

#endif // PSETTINGS_H
