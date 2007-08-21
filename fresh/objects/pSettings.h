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
#include "QSingleton.h"

#include <QSettings>
#include <QApplication>

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "My Application"
#endif

#ifndef PROGRAM_VERSION
#define PROGRAM_VERSION "1.0.0"
#endif


class QMainWindow;

class Q_MONKEY_EXPORT pSettings : public QSettings, public QSingleton<pSettings>
{
	Q_OBJECT
	friend class QSingleton<pSettings>;

public:
	pSettings( const QString& = PROGRAM_NAME, const QString& = PROGRAM_VERSION, QObject* = QApplication::instance() );
	~pSettings();

	QString programName() const;
	QString programVersion() const;

	virtual void restoreState( QMainWindow* );
	virtual void saveState( QMainWindow* );

	virtual void setDefaultSettings() {};

private:
	QString mProgramName;
	QString mProgramVersion;

};

#endif // PSETTINGS_H
