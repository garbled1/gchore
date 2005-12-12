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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/param.h>
#include <assert.h>

#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gchore.h"
#include "xmlparse.h"
#include "interface.h"

/**
   \file xmlparse.c
   \brief XML parsing and writing routines
   \author Tim Rightnour
*/

/**
   \brief Replacement xmlGenericErrorFunc
*/

void xml_error_handler(void *ctx, const char *msg, ...)
{
    va_list args;
    char buf[2040];

    va_start(args, msg);
    vsnprintf(buf, 2040, msg, args);
    va_end(args);
    errormsg(GTK_MESSAGE_ERROR, buf);
}

/** \brief parse a string from the XML stuff */

void parsestring(xmlDocPtr doc, xmlNodePtr cur, char **string)
{
    char *tmp;

    tmp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
    *string = tmp;
}

/** \brief parse an int from the XML stuff */

void parseint(xmlDocPtr doc, xmlNodePtr cur, int *data)
{
    char *tmp;

    tmp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
    *data = atoi(tmp);
    free(tmp);
}

/** \brief parse a long from the XML stuff */

void parselong(xmlDocPtr doc, xmlNodePtr cur, long *data)
{
    char *tmp;

    tmp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
    *data = atol(tmp);
    free(tmp);
}

/* XML WRITERS */

xmlNodePtr write_boolean(xmlNodePtr node, char *elm, int bool)
{
    if (bool)
	return(xmlNewChild(node, NULL, elm, NULL));
    else
	return(NULL);
}

/** \brief write a string */
xmlNodePtr write_string(xmlNodePtr node, char *elm, char *string)
{
    return(xmlNewChild(node, NULL, elm, string));
}

/** \brief write an int */
xmlNodePtr write_int(xmlNodePtr node, char *elm, int i)
{
    char buf[256];

    sprintf(buf, "%d", i);
    return(xmlNewChild(node, NULL, elm, buf));
}

/** \brief write a long */
xmlNodePtr write_long(xmlNodePtr node, char *elm, long l)
{
    char buf[256];

    sprintf(buf, "%ld", l);
    return(xmlNewChild(node, NULL, elm, buf));
}
