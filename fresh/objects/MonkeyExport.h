/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pSettings.h
 * LICENSE      : GPL
 * COMMENTARY   : This header allow to export class under windows
 ********************************************************************************************************/
#ifndef MONKEY_EXPORT_H
#define MONKEY_EXPORT_H

#include <qglobal.h>

#if defined(MONKEY_CORE_BUILD)
#define Q_MONKEY_EXPORT Q_DECL_EXPORT
#else
#define Q_MONKEY_EXPORT Q_DECL_IMPORT
#endif

#endif // MONKEY_EXPORT_H
