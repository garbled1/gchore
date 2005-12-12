/* $Id$ */
/*
    Gchore

    Copyright (C) 2005 Tim Rightnour

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    The author can be reached via e-mail at root@garbled.net
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include "gchore.h"

/**
   \file utils.h
   \author Tim Rightnour
   \brief Utilitiy definitions
*/

/**
   \brief A quick and simple safer malloc
*/
#define smalloc(foo) _safer_malloc(sizeof(foo), __FILE__, __LINE__)

/** \brief a macro interface to safer_malloc */
#define safer_malloc(foo) _safer_malloc(foo, __FILE__, __LINE__)

/** \brief a macro interface to bailout */
#define bailout() _bailout(__FILE__, __LINE__)

void _bailout(char *file, int line);
void *_safer_malloc(size_t size, char *file, int line);
caltime_t *calc_caltime(int day, int month, int year);

#endif /* _UTILS_H_ */

