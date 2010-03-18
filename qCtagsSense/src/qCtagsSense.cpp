#include "qCtagsSense.h"
#include "qCtagsSenseUtils.h"

#include <ctags.h>

qCtagsSense::qCtagsSense( QObject* parent )
	: QObject( parent )
{
	initCtags();
	qCtagsSenseUtils::initialize();
}

qCtagsSense::~qCtagsSense()
{
	deInitCtags();
}
