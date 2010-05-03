#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QSet>
#include <QHash>
#include <QLocale>
#include <QFileInfo>

class QTranslator;

typedef QList<QTranslator*> QTranslatorList;

class TranslationManager : public QObject
{
	Q_OBJECT
	
public:
	TranslationManager( QObject* parent = 0 );
	virtual ~TranslationManager();
	
	QStringList translationsMasks() const;
	QStringList forbiddenTranslationsMasks() const;
	QLocale currentLocale() const;
	QLocale systemLocale() const;
	QStringList availableLocales() const;
	QList<QLocale> availableQLocales() const;
	QString translationsPath() const;
	QString systemTranslationsPath() const;

public slots:
	void reloadTranslations();
	void setTranslationsMasks( const QStringList& masks );
	void addTranslationsMask( const QString& mask );
	void removeTranslationsMask( const QString& mask );
	void setForbiddenTranslationsMasks( const QStringList& masks );
	void addForbiddenTranslationsMask( const QString& mask );
	void removeForbiddenTranslationsMask( const QString& mask );
	void setCurrentLocale( const QLocale& locale );
	void setTranslationsPath( const QString& path );
	void setSystemTranslationsPath( const QString& path );
	
protected:
	QLocale mCurrentLocale;
	QSet<QString> mTranslationsMasks;
	QSet<QString> mForbiddenTranslationsMasks;
	QString mTranslationsPath;
	QString mSystemTranslationsPath;
	QSet<QString> mAvailableLocales;
	QHash<QLocale, QTranslatorList> mTranslators;
	
	bool addTranslator( const QString& filePath, const QLocale& locale );
	void clearTranslators();
};

inline uint qHash( const QLocale& locale )
{
	return qHash( locale.name() );
}

#endif // TRANSLATIONMANAGER_H
