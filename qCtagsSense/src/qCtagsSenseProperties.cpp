#include "qCtagsSenseProperties.h"

#include <ctags.h>

#include <QDebug>

using namespace qCtagsSense;

void Properties::initialize()
{
	const char* kinds = languageKinds( "C++" );
	qWarning() << "kinds" << kinds;
}
