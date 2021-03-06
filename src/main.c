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
   \file main.c
   \brief main() for gchore, and assorted globals
   \author Tim Rightnour
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gchore.h"
#include "interface.h"
#include "support.h"
#include "xmlparse.h"

GtkWidget *dialog_options;
GtkWidget *calendar_window;
guint timeout;
const char *firsttime = 
    "This appears to be the first time you have ever run <b>gchore</b>.\n"
    "I have gone ahead and created a <i>.gchorerc</i> file in your"
    " home directory, as well as the <i>gchore.task</i>, <i>gchore.todo</i>"
    " and <i>gchore.log</i>"
    " files, which contain your task, todo and log databases respectively."
    "\n\nPlease take a look at the options under <i>File->Properties</i>"
    " and edit them to your liking before adding any tasks";
const gchar *version = "1.0";

extern options_t *options;

int main (int argc, char *argv[])
{
    GtkWidget *dialog_reminder;
    GtkWidget *dialog_taskdelete;
    GtkWidget *dialog, *tmpw;
    char file[MAX_PATH], *home;

    gtk_set_locale();
    gtk_init(&argc, &argv);

    tzset();

    /*xmlSetGenericErrorFunc(NULL, xml_error_handler);*/

    add_pixmap_directory(PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

    calendar_window = create_window1();
    gtk_widget_show(calendar_window);

    /* if none exists, write it */
    if (!parse_optionsfile()) {
	dialog = gtk_message_dialog_new_with_markup(
	    (GtkWindow *)calendar_window, GTK_DIALOG_DESTROY_WITH_PARENT,
	    GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, firsttime);
	g_signal_connect_swapped(dialog, "response",
				 G_CALLBACK(gtk_widget_destroy), dialog);
	gtk_widget_show(dialog);
	home = getenv("HOME");
	if (home == NULL)
	    sprintf(file, "./gchore.task");
	else
	    sprintf(file, "%s/gchore.task", home);
	options->taskdb = strdup(file);

	if (home == NULL)
	    sprintf(file, "./gchore.todo");
	else
	    sprintf(file, "%s/gchore.todo", home);
	options->tododb = strdup(file);

	if (home == NULL)
	    sprintf(file, "./gchore.log");
	else
	    sprintf(file, "%s/gchore.log", home);
	options->logdb = strdup(file);

	write_optionsfile();
    } else {
	parse_taskfile(options->taskdb);
	parse_todofile(options->tododb);
	parse_logfile(options->logdb);
	tmpw = lookup_widget(calendar_window, "calendar1");
	if (tmpw)
	    on_calendar1_day_selected(GTK_CALENDAR(tmpw), NULL);
    }

    (void)scan_tasktable(NULL);

    timeout = g_timeout_add(options->checkfrequency*1000,
			    scan_tasktable, NULL);

    gtk_main();
    return 0;
}

