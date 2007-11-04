/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : compiler_defines.h
** Date      : 2007-11-04T22:50:28
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
/*
 * Copyright (c) 1998,1999,2000,2001,2002 Tal Davidson. All rights reserved.
 *
 * compiler_defines.h   (1 January 1999)
 * by Tal Davidson (davidsont@bigfoot.com)
 * This file is a part of "Artistic Style" - an indentater and reformatter
 * of C, C++, C# and Java source files.
 *
 * The "Artistic Style" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published 
 * by the Free Software Foundation; either version 2 of the License, 
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */





/*
 * comment out the line below if your compiler does NOT understand NAMESPACES
 */
#define USES_NAMESPACE


#if defined(__GNUC__) && __GNUC__ < 3 
// for G++ implementation of string.compare:
#define COMPARE(place, length, str)       compare((str), (place), (length))
#else
// for standard implementation of string.compare:
#define COMPARE(place, length, str)       compare((place), (length), (str))
#endif


// Fix by John A. McNamara
// Get rid of annoying MSVC warnings on debug builds about lengths of
// identifiers in template instantiations.
#ifdef _MSC_VER
#pragma warning( disable:4786 )
#endif

