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

/**
   \file utils.c
   \brief utility functions
   \author Tim Rightnour
*/

#include <stdio.h>
#include <time.h>

#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "utils.h"

/**
   \brief calculate caltime_t from mon/day/year
   \param mon month
   \param day day of month
   \param year year
   \return pointer to caltime_t that must be freed
*/

caltime_t *calc_caltime(int day, int month, int year)
{
    caltime_t *ret;
    time_t mid;
    struct tm *now;

    ret = smalloc(caltime_t);
    mid = time(NULL);
    now = localtime(&mid);

    ret->sday.tm_sec = 0;
    ret->sday.tm_min = 0;
    ret->sday.tm_hour = 0;
    ret->sday.tm_mday = day;
    ret->sday.tm_mon = month;
    ret->sday.tm_year = year-1900;
    ret->sday.tm_isdst = now->tm_isdst;
    ret->sday.tm_gmtoff = now->tm_gmtoff;
    ret->sday.tm_zone = now->tm_zone;
    mid = mktime(&(ret->sday));
    /* extract this day's weekday */
    now = localtime(&mid);
    ret->sday.tm_wday = now->tm_wday;
    ret->midnight = mid;

    return(ret);
}


/**
   \brief exit on a failure
   \param file file name we bailed in
   \param line line number we bailed on
*/

void _bailout(char *file, int line)
{
    char *pname;
    extern int errno;

#if defined(__NetBSD__)
    pname = (char *)getprogname();
#elif defined(__linux__)
    pname = program_invocation_name;
#endif

    /* the fprintf catches cases where malloc failed, as it will likely fail
       again in LOG() */
    (void)fprintf(stderr, "%s: Failed in %s:%d: %s %d\n",
		  pname, file, line, strerror(errno), errno);
    exit(1);
}

/**
   \brief return a zero'd malloc with error checking
   \param size size to malloc
   \param file filename we were called from
   \param line line number we were called from
   \return malloc'd data
*/

void *_safer_malloc(size_t size, char *file, int line)
{
    void *stuff;

    stuff = malloc(size);
    if (stuff == NULL)
	_bailout(file, line);
    memset(stuff, 0, size);
    return(stuff);
}
