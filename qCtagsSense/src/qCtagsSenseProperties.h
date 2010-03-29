#ifndef QCTAGSSENSEPROPERTIES_H
#define QCTAGSSENSEPROPERTIES_H

#include "qCtagsSenseExport.h"
#include "qCtagsSenseEntry.h"

#include <QSet>
#include <QHash>

namespace qCtagsSense
{

struct QCTAGSSENSE_EXPORT Properties
{
	void initialize();
	/*
	typedef QSet<qCtagsSenseEntry::Kind> KindSet;
	typedef QHash<qCtagsSenseEntry::Language, KindSet> LanguageKindSets;
	typedef QSet<QString> StringSet;
	typedef QHash<qCtagsSenseEntry::Language, StringSet> LanguageStringSets;
	
	qCtagsSenseProperties::LanguageKindSets mDefaultKinds;
	qCtagsSenseProperties::LanguageStringSets mDefaultSuffixes;
	*/
};

}; // namespace qCtagsSense

#endif // QCTAGSSENSEPROPERTIES_H
