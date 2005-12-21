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
   \file options.c
   \brief options/preferences loading/saving/basic handling
   \author Tim Rightnour
*/

#include <stdio.h>
#if defined(__linux__)
#include "queue.h"
#else
#include <sys/queue.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gchore.h"
#include "utils.h"
#include "xmlparse.h"

options_t *options;

/**
   \brief parse an options set
   \param doc pointer to xml doc file
   \param cur expected to be at the level of the options tag on entry
   \param options pointer to options structure to be filled out
*/

void parse_options(xmlDocPtr doc, xmlNodePtr cur, options_t *options)
{
    xmlNodePtr child;

    child = cur->xmlChildrenNode;
    while (child != NULL) {
	if (!strcmp(child->name, "maxproc"))
	    parseint(doc, child, &options->maxproc);
	if (!strcmp(child->name, "emailaddr"))
	    parsestring(doc, child, &options->emailaddr);
	if (!strcmp(child->name, "emailfreq"))
	    parseint(doc, child, &options->emailfreq);
	if (!strcmp(child->name, "emailtime"))
	    parseint(doc, child, &options->emailtime);
	if (!strcmp(child->name, "reportproc"))
	    parseint(doc, child, &options->reportproc);
	if (!strcmp(child->name, "taskdb"))
	    parsestring(doc, child, &options->taskdb);
	if (!strcmp(child->name, "tododb"))
	    parsestring(doc, child, &options->tododb);
	if (!strcmp(child->name, "logdb"))
	    parsestring(doc, child, &options->logdb);
	if (!strcmp(child->name, "checkfrequency"))
	    parseint(doc, child, &options->checkfrequency);
	if (!strcmp(child->name, "sendmail"))
	    parsestring(doc, child, &options->sendmail);
	if (!strcmp(child->name, "watchdir"))
	    parsestring(doc, child, &options->watchdir);
	if (!strcmp(child->name, "wakeup"))
	    parseint(doc, child, &options->wakeup);
	child = child->next;
    }
}

/**
   \brief write an options set
   \param node an xmlNodePtr
   \param options options to write out
*/

void write_options(xmlNodePtr node, options_t *options)
{
    write_int_if(node, "maxproc", options->maxproc);
    write_string_if(node, "emailaddr", options->emailaddr);
    write_int_if(node, "emailfreq", options->emailfreq);
    write_int_if(node, "emailtime", options->emailtime);
    write_int_if(node, "reportproc", options->reportproc);
    write_string_if(node, "taskdb", options->taskdb);
    write_string_if(node, "tododb", options->tododb);
    write_string_if(node, "logdb", options->logdb);
    write_string_if(node, "sendmail", options->sendmail);
    write_string_if(node, "watchdir", options->watchdir);
    write_int_if(node, "wakeup", options->wakeup);
    write_int_if(node, "checkfrequency", options->checkfrequency);
}

/**
   \brief parse the options file
   \return true if options file was found
*/

int parse_optionsfile(void)
{
    char file[MAX_PATH], *sf, *home;
    xmlDocPtr doc;
    xmlNodePtr cur;

    sf = ".gchorerc";
    home = getenv("HOME");
    doc = NULL;
    options = smalloc(options_t);
    options->checkfrequency = 60; /* set a default for this */
    options->wakeup = 21600; /* 6am */
    options->sendmail = "sendmail";
    
    if (home != NULL) {
	sprintf(file, "%s/%s", home, sf);
	doc = xmlParseFile(file);
    }
    if (doc == NULL) {
	sprintf(file, "./%s", sf);
	doc = xmlParseFile(file);
    }
    if (doc == NULL) {
	errormsg(GTK_MESSAGE_WARNING, "Warning, cannot find options file %s",
		 sf);
	return(0);
    }
    /* if we got this file, we found one */

    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        errormsg(GTK_MESSAGE_ERROR, "Options file %s is empty\n", file);
        xmlFreeDoc(doc);
	return(0);
    }
    if (strcmp(cur->name, "Options")) {
        errormsg(GTK_MESSAGE_ERROR, "%s: root node != Options\n", file);
        xmlFreeDoc(doc);
	return(0);
    }

    parse_options(doc, cur, options);
    xmlFreeDoc(doc);
    return(1);
}

/**
   \brief write the options file out to $HOME/.gchorerc
*/

void write_optionsfile(void)
{
    xmlDocPtr doc;
    xmlNodePtr tree;
    char file[MAX_PATH], *home;

    doc = xmlNewDoc("1.0");
    doc->children = xmlNewDocNode(doc, NULL, "Options", NULL);
    tree = doc->children;

    write_options(tree, options);

    home = getenv("HOME");
    if (home == NULL)
	sprintf(file, "./.gchorerc");
    else
	sprintf(file, "%s/.gchorerc", home);

    xmlSaveFormatFile(file, doc, 1);
    xmlFreeDoc(doc);
}
