#ifndef EXUBERANT_CTAGS
#define EXUBERANT_CTAGS

#include "entry.h"

/*procedure will return list for  file. Every time new list will be generated. For every generated list memory must be freed using Ctags::freeTagEntryList
*/

extern TagEntryListItem* parseFile ( const char* fileName, const char* langName);

#endif // EXUBERANT_CTAGS