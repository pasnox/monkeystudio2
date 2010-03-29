#include "qCtagsSense.h"

#include <ctags.h>

using namespace qCtagsSense;

Instance::Instance( QObject* parent )
	: QObject( parent )
{
	initCtags();
	qCtagsSense::initialize();
	mProperties.initialize();
	//setLanguageKinds( "C++", "cdefglmnpstuvx" );
}

Instance::~Instance()
{
	deInitCtags();
}
