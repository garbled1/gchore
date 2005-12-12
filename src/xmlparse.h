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

#ifndef __xmlparse_h__
#define __xmlparse_h__

/**
   \file xmlparse.h
   \author Tim Rightnour
   \brief XML parsing definitions
*/

void xml_error_handler(void *ctx, const char *msg, ...);
void parsestring(xmlDocPtr doc, xmlNodePtr cur, char **string);
void parseint(xmlDocPtr doc, xmlNodePtr cur, int *data);
void parselong(xmlDocPtr doc, xmlNodePtr cur, long *data);

/*writers*/
xmlNodePtr write_boolean(xmlNodePtr node, char *elm, int bool);
xmlNodePtr write_string(xmlNodePtr node, char *elm, char *string);
xmlNodePtr write_int(xmlNodePtr node, char *elm, int i);
xmlNodePtr write_long(xmlNodePtr node, char *elm, long l);

#define write_string_if(node, elm, string) \
    if (string != NULL) \
	write_string(node, elm, string)
#define write_int_if(node, elm, i) \
    if (i != 0) \
	write_int(node, elm, i)
#define write_long_if(node, elm, i) \
    if (i != 0) \
	write_int(node, elm, i)

#endif /* __xmlparse_h__ */
