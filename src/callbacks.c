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
   \file callbacks.c
   \brief GTK callbacks
   \author Tim Rightnour
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>
#if defined(__linux__)
#include "queue.h"
#else
#include <sys/queue.h>
#endif

#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gchore.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "utils.h"

extern options_t *options;
extern GtkWidget *calendar_today, *calendar_window;
extern TAILQ_HEAD(, _task_t) tasktable;
extern TAILQ_HEAD(, _todo_t) todotable;
extern char *tzname[2];

void on_properties1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *dialog_options;

    dialog_options = create_dialog_options();
    gtk_widget_show(dialog_options);
}


void on_new1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    clean_all_tasks();
    clean_all_todos();
    /* clear the textarea on the calendar */
}


void on_open1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *filechooserdialog1;

    filechooserdialog1 = create_filechooserdialog1();
    gtk_widget_show(filechooserdialog1);
}

/**
   \brief open was selected
*/

gboolean button3_button_press_event(GtkWidget *widget,
				    GdkEventButton *event,
				    gpointer user_data)
{
    GtkWidget *fc;
    char *filename;

    fc = (GtkWidget *)user_data;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fc));
    if (filename != NULL) {
	clean_all_tasks();
	clean_all_todos();
	parse_taskfile(filename);
	free(filename);
	if (options->tododb != NULL)
	    parse_todofile(options->tododb);
    }
    return FALSE;
}


void on_save1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    /* if we have a taskdb, save it */
    if (options->taskdb != NULL)
	write_taskfile(options->taskdb);
    else 
	on_save_as1_activate(menuitem, user_data);

    /* if we have a tododb, save it */
    if (options->tododb != NULL)
	write_todofile(options->tododb);
    else
	on_save_as1_activate(menuitem, user_data);
}


void on_save_as1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *dialog, *entry;
    char *text;

    dialog = gtk_dialog_new_with_buttons("Save As",
					 GTK_WINDOW(calendar_window),
					 GTK_DIALOG_DESTROY_WITH_PARENT,
					 GTK_STOCK_SAVE_AS, GTK_RESPONSE_OK,
					 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					 NULL);
    entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), entry);
    gtk_widget_show(entry);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
	text = (char *)gtk_entry_get_text(GTK_ENTRY(entry));
	if (strlen(text) > 1)
	    write_taskfile(text);
	free(text);
    }
    gtk_widget_destroy(dialog);
}


void on_quit1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    quit_program();
}


void add_new_task(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *dialog_taskentry;

    dialog_taskentry = create_dialog_taskentry(NULL, -1);
    gtk_widget_show(dialog_taskentry);
}


void on_delete_task1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *dialog_taskdelete;
    GtkWidget *dialog_vbox5;
    GtkWidget *fixed2;
    GtkWidget *label14;
    GtkWidget *combobox1;
    GtkWidget *dialog_action_area5;
    GtkWidget *button_canceldelete;
    GtkWidget *button_okdelete;
    task_t *task;

    dialog_taskdelete = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog_taskdelete), "Delete Task");
    gtk_window_set_type_hint(GTK_WINDOW(dialog_taskdelete),
			     GDK_WINDOW_TYPE_HINT_DIALOG);

    dialog_vbox5 = GTK_DIALOG(dialog_taskdelete)->vbox;
    gtk_widget_show(dialog_vbox5);

    fixed2 = gtk_fixed_new();
    gtk_widget_show(fixed2);
    gtk_box_pack_start(GTK_BOX(dialog_vbox5), fixed2, TRUE, TRUE, 0);

    label14 = gtk_label_new("Task to delete");
    gtk_widget_show(label14);
    gtk_fixed_put(GTK_FIXED(fixed2), label14, 24, 8);
    gtk_widget_set_size_request(label14, 128, 24);

    combobox1 = gtk_combo_box_new_text();
    gtk_widget_show(combobox1);
    gtk_fixed_put(GTK_FIXED(fixed2), combobox1, 24, 32);
    gtk_widget_set_size_request(combobox1, 184, 24);

    dialog_action_area5 = GTK_DIALOG(dialog_taskdelete)->action_area;
    gtk_widget_show(dialog_action_area5);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area5),
			      GTK_BUTTONBOX_END);

    button_canceldelete = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_show(button_canceldelete);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_taskdelete),
				 button_canceldelete, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(button_canceldelete, GTK_CAN_DEFAULT);

    button_okdelete = gtk_button_new_from_stock("gtk-ok");
    gtk_widget_show(button_okdelete);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_taskdelete),
				 button_okdelete, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(button_okdelete, GTK_CAN_DEFAULT);

    /* now fill in the combo box */
    TAILQ_FOREACH(task, &tasktable, next)
	gtk_combo_box_append_text(GTK_COMBO_BOX(combobox1), task->name);

    if (gtk_dialog_run(GTK_DIALOG(dialog_taskdelete)) == GTK_RESPONSE_OK) {
	char *text;

	text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox1));
	task = find_task_byname(text);
	free(text);
	delete_task(task->taskid);
    }
    save_some_files();
    gtk_widget_destroy(dialog_taskdelete);
}

void on_edit_task1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *dialog_taskedit;
    GtkWidget *dialog_vbox5;
    GtkWidget *fixed2;
    GtkWidget *label14;
    GtkWidget *combobox1;
    GtkWidget *dialog_action_area5;
    GtkWidget *button_canceledit;
    GtkWidget *button_okedit;
    task_t *task;

    dialog_taskedit = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog_taskedit), "Edit Task");
    gtk_window_set_type_hint(GTK_WINDOW(dialog_taskedit),
			     GDK_WINDOW_TYPE_HINT_DIALOG);

    dialog_vbox5 = GTK_DIALOG(dialog_taskedit)->vbox;
    gtk_widget_show(dialog_vbox5);

    fixed2 = gtk_fixed_new();
    gtk_widget_show(fixed2);
    gtk_box_pack_start(GTK_BOX(dialog_vbox5), fixed2, TRUE, TRUE, 0);

    label14 = gtk_label_new("Task to edit");
    gtk_widget_show(label14);
    gtk_fixed_put(GTK_FIXED(fixed2), label14, 24, 8);
    gtk_widget_set_size_request(label14, 128, 24);

    combobox1 = gtk_combo_box_new_text();
    gtk_widget_show(combobox1);
    gtk_fixed_put(GTK_FIXED(fixed2), combobox1, 24, 32);
    gtk_widget_set_size_request(combobox1, 184, 24);

    dialog_action_area5 = GTK_DIALOG(dialog_taskedit)->action_area;
    gtk_widget_show(dialog_action_area5);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area5),
			      GTK_BUTTONBOX_END);

    button_canceledit = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_show(button_canceledit);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_taskedit),
				 button_canceledit, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(button_canceledit, GTK_CAN_DEFAULT);

    button_okedit = gtk_button_new_from_stock("gtk-ok");
    gtk_widget_show(button_okedit);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_taskedit),
				 button_okedit, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(button_okedit, GTK_CAN_DEFAULT);

    /* now fill in the combo box */
    TAILQ_FOREACH(task, &tasktable, next)
	gtk_combo_box_append_text(GTK_COMBO_BOX(combobox1), task->name);

    if (gtk_dialog_run(GTK_DIALOG(dialog_taskedit)) == GTK_RESPONSE_OK) {
	char *text;
	GtkWidget *dialog_taskentry;

	text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox1));
	task = find_task_byname(text);
	free(text);

	dialog_taskentry = create_dialog_taskentry(NULL, task->taskid);
	gtk_widget_show(dialog_taskentry);
    }
    gtk_widget_destroy(dialog_taskedit);
}

void on_about1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *dialog, *parent;
    extern const gchar *version;
    const gchar *authors[] = {
	"Tim Rightnour (root@garbled.net)",
	NULL
    };
    char *text =
	"Gchore is a simple tool that tracks your chores, and"
	" reminds you to do them on a daily basis.  It is not a general"
	" todo list, it is better suited for repeating tasks, such as"
	" taking out the garbage on mondays.";
    char *gpl =
	"    This program is free software; you can redistribute it \n"
	"and/or modify it under the terms of the GNU General Public License\n"
	"as published by the Free Software Foundation; either version 2 of\n"
	"the License, or (at your option) any later version."
	"\n\nThis program is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	"GNU General Public License for more details. You should have\n"
	"received a copy of the GNU General Public License along with\n"
	"this program; if not, write to the "
	"Free Software Foundation, Inc., \n59 Temple Place, Suite 330, \n"
	"Boston, MA  02111-1307  USA";

    parent = (GtkWidget *)user_data;

    dialog = gtk_about_dialog_new();
    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Gchore");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), version);
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),
				   "Copyright 2005 Tim Rightnour");
    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), gpl);
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),
				 "http://gchore.sourcefore.net/");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), text);
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), authors);
    
    gtk_widget_show(dialog);
}


void on_calendar1_day_selected(GtkCalendar *calendar, gpointer user_data)
{
    GtkTextBuffer *buffer;
    gchar *text, **tv;
    int year, month, day, i, k;
    task_t *task;
    caltime_t *tod;

    buffer = gtk_text_view_get_buffer((GtkTextView *)calendar_today);
    gtk_calendar_get_date(calendar, &year, &month, &day);
    tod = calc_caltime(day, month, year);

    tv = calloc(MAX_ITEMS_PER_DAY, sizeof(gchar *));
    i = 0;

    /* we know have midnight, now find tasks that occur on this day */

    TAILQ_FOREACH(task, &tasktable, next) {
	if (task->time) {
	    if (calc_midnight(task->time) == tod->midnight) {
		if (i < MAX_ITEMS_PER_DAY) {
		    tv[i] = g_strdup((gchar *)task->name);
		    i++;
		}
	    }
	} else {
	    if (task->dowe[0] || task->dow[tod->sday.tm_wday]) {
		if (i < MAX_ITEMS_PER_DAY) {
		    tv[i] = g_strdup((gchar *)task->name);
		    i++;
		}
	    } else if ((task->dowe[1] && tod->sday.tm_wday == 1) ||
		       (task->dowe[2] && tod->sday.tm_wday == 1 &&
			((tod->sday.tm_yday/7)%2 == 0)) ||
		       (task->dowe[3] && tod->sday.tm_mday == 1) ||
		       (task->dowe[4] && tod->sday.tm_mday == 1 &&
			tod->sday.tm_mon%2 == 0)) {
		if (i < MAX_ITEMS_PER_DAY) {
		    tv[i] = g_strdup((gchar *)task->name);
		    i++;
		}
	    }
	}
    }
    text = g_strjoinv("\n", tv);
    for (k=0; k<i; k++)
	if (tv[k] != NULL)
	    free(tv[k]);
    free(tv);
    free(tod);
    gtk_text_buffer_set_text(buffer, text, -1);
}


void on_calendar1_day_selected_double_click(GtkCalendar *calendar,
					    gpointer user_data)
{
    GtkWidget *dialog_taskentry;
    caltime_t *tod;
    int year, month, day;

    gtk_calendar_get_date(calendar, &year, &month, &day);
    tod = calc_caltime(day, month, year);
    dialog_taskentry = create_dialog_taskentry(tod, -1);
    gtk_widget_show(dialog_taskentry);
}

/**
   \brief handle the apply for taskentry dialog
*/

gboolean on_applybutton1_button_press_event(GtkWidget *widget,
					    GdkEventButton *event,
					    gpointer user_data)
{
    task_t *task, *tmptask;
    GtkWidget *tmpw, *parent;
    gchar *text;
    char buf[256];
    struct tm date;
    int i, clkd, new;

    parent = (GtkWidget *)user_data;

    task = new_task();

    tmpw = lookup_widget(parent, "entry_taskname");
    if (tmpw == NULL)
	goto out;
    text = (gchar *)gtk_entry_get_text((GtkEntry *)tmpw);
    if (strlen(text) < 2) {
	errormsg(GTK_MESSAGE_ERROR, "Task name not filled out.");
	goto out2;
    }
    task->name = strdup((char *)text);

    tmpw = lookup_widget(parent, "check_procrastinate");
    if (tmpw == NULL)
	goto out;
    task->procrastinatable = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
    tmpw = lookup_widget(parent, "check_noemail");
    if (tmpw == NULL)
	goto out;
    task->email = !(gtk_toggle_button_get_active((GtkToggleButton *)tmpw));
    tmpw = lookup_widget(parent, "spinbutton1");
    if (tmpw == NULL)
	goto out;
    task->pdays = gtk_spin_button_get_value(GTK_SPIN_BUTTON(tmpw));

    tmpw = lookup_widget(parent, "repeat_false");
    if (tmpw == NULL)
	goto out;
    if (gtk_toggle_button_get_active((GtkToggleButton *)tmpw)) {
	/* figure out day of task */
	memset(&date, 0, sizeof(struct tm));

	tmpw = lookup_widget(parent, "entry_taskdate");
	if (tmpw == NULL)
	    goto out;
	text = (gchar *)gtk_entry_get_text((GtkEntry *)tmpw);
	if (text == NULL)
	    goto out;
	sprintf(buf, "%s", text);

	tmpw = lookup_widget(parent, "entry_taskdate");
	if (tmpw == NULL)
	    goto out;
	text = (gchar *)gtk_entry_get_text((GtkEntry *)tmpw);
	sprintf(buf, "%s %s", buf, text);

	(void)strptime(text, "%m/%d/%Y %R", &date);

	task->time = mktime(&date);
    } else {
	tmpw = lookup_widget(parent, "check_daily");
	if (tmpw == NULL)
	    goto out;
	task->dowe[0] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_weekly");
	if (tmpw == NULL)
	    goto out;
	task->dowe[1] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_bi_weekly");
	if (tmpw == NULL)
	    goto out;
	task->dowe[2] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_monthly");
	if (tmpw == NULL)
	    goto out;
	task->dowe[3] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_bi_monthly");
	if (tmpw == NULL)
	    goto out;
	task->dowe[4] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_sunday");
	if (tmpw == NULL)
	    goto out;
	task->dow[0] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_monday");
	if (tmpw == NULL)
	    goto out;
	task->dow[1] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_tuesday");
	if (tmpw == NULL)
	    goto out;
	task->dow[2] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_wednesday");
	if (tmpw == NULL)
	    goto out;
	task->dow[3] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_thursday");
	if (tmpw == NULL)
	    goto out;
	task->dow[4] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_friday");
	if (tmpw == NULL)
	    goto out;
	task->dow[5] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_saturday");
	if (tmpw == NULL)
	    goto out;
	task->dow[6] = gtk_toggle_button_get_active((GtkToggleButton *)tmpw);
	tmpw = lookup_widget(parent, "check_saturday");
	if (tmpw == NULL)
	    goto out;
	task->time = 0;

	for (clkd=0,i=0; i<7; i++)
	    if (task->dow[i])
		clkd++;
	for (i=0; i<5; i++)
	    if (task->dowe[i])
		clkd++;
	if (!clkd) {
	    errormsg(GTK_MESSAGE_ERROR, "Task repeats but no day picked.");
	    goto out2;
	}

    }

    tmpw = lookup_widget(parent, "entry_taskid");
    if (tmpw == NULL)
	goto out;
    text = (gchar *)gtk_entry_get_text((GtkEntry *)tmpw);
    task->taskid = atoi(text);
    new = 0;

    if (task->taskid == -1) {
	tmptask = find_task_byname(task->name);
	if (tmptask == NULL) {
	    task->taskid = gen_new_taskid();
	    new++;
	} else {
	    task->taskid = tmptask->taskid;
	}
    }
    if (!new) {
	tmptask = find_task_byid(task->taskid);
	if (tmptask != NULL) {
	    TAILQ_REMOVE(&tasktable, tmptask, next);
	    free(tmptask);
	}
    }
    TAILQ_INSERT_TAIL(&tasktable, task, next);

    tmpw = lookup_widget(calendar_window, "calendar1");
    if (tmpw != NULL)
     on_calendar1_day_selected(GTK_CALENDAR(tmpw), NULL);

    save_some_files();
    return(FALSE);

out:
    free(task);
    return(FALSE);
out2:
    free(task);
    return(TRUE);
}


gboolean on_cancelbutton1_button_press_event(GtkWidget *widget, 
					     GdkEventButton *event,
					     gpointer user_data)
{
    GtkWidget *parent;

    parent = (GtkWidget *)user_data;
    gtk_widget_destroy(parent);
    return FALSE;
}


gboolean on_button_oktaskedit_button_press_event(GtkWidget *widget,
						 GdkEventButton *event,
						 gpointer user_data)
{
    GtkWidget *parent;
    gboolean ret;

    parent = (GtkWidget *)user_data;
    ret = on_applybutton1_button_press_event(widget, event, user_data);
    if (!ret)
	gtk_widget_destroy(parent);

    return ret;
}


gboolean on_button_applyoptions_button_press_event(GtkWidget *widget,
						   GdkEventButton *event,
						   gpointer user_data)
{
    GtkWidget *tmpw, *parent;
    char *text;
    char buf[256];
    int hr, min, sec;

    parent = (GtkWidget *)user_data;

    tmpw = lookup_widget(parent, "spin_procrastinate");
    if (tmpw == NULL)
	return(FALSE);
    options->maxproc = gtk_spin_button_get_value(GTK_SPIN_BUTTON(tmpw));
    tmpw = lookup_widget(parent, "entry_taskdb");
    if (tmpw == NULL)
	return(FALSE);
    options->taskdb = strdup(gtk_entry_get_text(GTK_ENTRY(tmpw)));
    tmpw = lookup_widget(parent, "entry_tododb");
    if (tmpw == NULL)
	return(FALSE);
    options->tododb = strdup(gtk_entry_get_text(GTK_ENTRY(tmpw)));
    tmpw = lookup_widget(parent, "entry_logdb");
    if (tmpw == NULL)
	return(FALSE);
    options->logdb = strdup(gtk_entry_get_text(GTK_ENTRY(tmpw)));
    tmpw = lookup_widget(parent, "spin_updatetime");
    if (tmpw == NULL)
	return(FALSE);
    options->checkfrequency = gtk_spin_button_get_value(GTK_SPIN_BUTTON(tmpw));
    options->checkfrequency *= 60;
    tmpw = lookup_widget(parent, "entry_watch_directory");
    if (tmpw == NULL)
	return(FALSE);
    options->watchdir = strdup(gtk_entry_get_text(GTK_ENTRY(tmpw)));
    tmpw = lookup_widget(parent, "entry_wakeup");
    if (tmpw == NULL)
	return(FALSE);
    text = strdup(gtk_entry_get_text(GTK_ENTRY(tmpw)));
    sscanf(text, "%d:%d", &hr, &min);
    options->wakeup = hr*3600 + min*60;
    free(text);
    tmpw = lookup_widget(parent, "entry_emailaddr");
    if (tmpw == NULL)
	return(FALSE);
    options->emailaddr = strdup(gtk_entry_get_text(GTK_ENTRY(tmpw)));
    tmpw = lookup_widget(parent, "radio_email_complete");
    if (tmpw == NULL)
	return(FALSE);
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmpw)))
	options->emailfreq = EMAIL_COMPLETE;
    tmpw = lookup_widget(parent, "radio_email_time");
    if (tmpw == NULL)
	return(FALSE);
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmpw))) {
	/* set the time */
	tmpw = lookup_widget(parent, "entry_emailtime");
	if (tmpw == NULL)
	    return(FALSE);
	text = strdup(gtk_entry_get_text(GTK_ENTRY(tmpw)));
	sscanf(text, "%d:%d", &hr, &min);
	options->emailtime = hr*3600 + min*60;
	free(text);

	tmpw = lookup_widget(parent, "radio_freq_daily");
	if (tmpw == NULL)
	    return(FALSE);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmpw)))
	    options->emailfreq = EMAIL_DAILY;
	tmpw = lookup_widget(parent, "radio_freq_weekly");
	if (tmpw == NULL)
	    return(FALSE);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmpw)))
	    options->emailfreq = EMAIL_WEEKLY;
	tmpw = lookup_widget(parent, "radio_freq_monthly");
	if (tmpw == NULL)
	    return(FALSE);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmpw)))
	    options->emailfreq = EMAIL_MONTHLY;
    }
    tmpw = lookup_widget(parent, "check_reportproc");
    if (tmpw == NULL)
	return(FALSE);
    options->reportproc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmpw));
    tmpw = lookup_widget(parent, "entry_mailer_agent");
    if (tmpw == NULL)
	return(FALSE);
    options->sendmail = strdup(gtk_entry_get_text(GTK_ENTRY(tmpw)));
    tmpw = lookup_widget(parent, "checkbutton1");
    if (tmpw == NULL)
	return(FALSE);
    if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmpw)))
	options->emailfreq = 0;

    write_optionsfile();

    return FALSE;
}


gboolean on_button_canceloptions_button_press_event(GtkWidget *widget,
						    GdkEventButton *event,
						    gpointer user_data)
{
    GtkWidget *parent;

    parent = (GtkWidget *)user_data;
    gtk_widget_destroy(parent);
    return FALSE;
}


gboolean on_button_okoptions_button_press_event(GtkWidget *widget,
						GdkEventButton *event,
						gpointer user_data)
{
    GtkWidget *parent;

    parent = (GtkWidget *)user_data;
    on_button_applyoptions_button_press_event(widget, event, user_data);
    gtk_widget_destroy(parent);
    return FALSE;
}


gboolean on_button_remindercomplete_button_press_event(GtkWidget *widget,
						       GdkEventButton *event,
						       gpointer user_data)
{
    GtkWidget *parent, *tmpw;
    char *text;
    task_t *task;
    todo_t *todo;

    parent = (GtkWidget *)user_data;

    tmpw = lookup_widget(parent, "label_taskname");
    if (tmpw == NULL)
	return(FALSE);
    text = (char *)gtk_label_get_text(GTK_LABEL(tmpw));
    task = find_task_byname(text);
    if (task == NULL)
	return(FALSE);
    todo = find_todo_byid(task->taskid);
    if (todo == NULL)
	return(FALSE);
    if (task->email)
	log_task(task->taskid, time(NULL), todo->procdays);

    todo->procdays = 0;
    todo->completed = time(NULL);
    todo->lastalert = time(NULL);
    todo->window = NULL;

    if (task->time) {
	/* one time task, nuke it */
	TAILQ_REMOVE(&tasktable, task, next);
	TAILQ_REMOVE(&todotable, todo, next);
	free(task->name);
	free(task);
	free(todo);
    }

    gtk_widget_destroy(parent);
    save_some_files();
    return(FALSE);
}


gboolean on_button_reminderprocrastinate_button_press_event(GtkWidget *widget,
    GdkEventButton *event, gpointer user_data)
{
    GtkWidget *parent, *tmpw;
    char *text;
    task_t *task;
    todo_t *todo;

    parent = (GtkWidget *)user_data;

    tmpw = lookup_widget(parent, "label_taskname");
    if (tmpw == NULL)
	return(FALSE);
    text = (char *)gtk_label_get_text(GTK_LABEL(tmpw));
    task = find_task_byname(text);
    if (task == NULL)
	return(FALSE);
    todo = find_todo_byid(task->taskid);
    if (todo == NULL)
	return(FALSE);
    todo->procdays++;
    todo->completed = 0;
    todo->window = NULL;

    gtk_widget_destroy(parent);
    save_some_files();
    return(FALSE);
}

gboolean on_button_findtaskdb_button_press_event(GtkWidget *widget,
						 GdkEventButton *event,
						 gpointer user_data)
{
    GtkWidget *fd1, *parent, *tmpw;

    parent = (GtkWidget *)user_data;
    fd1 = gtk_file_chooser_dialog_new("Find Task Database",
				      (GtkWindow *)parent,
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_OK, NULL);
    if (gtk_dialog_run(GTK_DIALOG(fd1)) == GTK_RESPONSE_OK) {
	gchar *filename;

	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fd1));
	tmpw = lookup_widget(parent, "entry_taskdb");
	if (tmpw != NULL)
	    gtk_entry_set_text(GTK_ENTRY(tmpw), filename);
	g_free(filename);
    }

    gtk_widget_destroy(fd1);

    return FALSE;
}


gboolean on_button_findtododb_button_press_event(GtkWidget *widget,
						 GdkEventButton *event,
						 gpointer user_data)
{
    GtkWidget *fd1, *parent, *tmpw;

    parent = (GtkWidget *)user_data;
    fd1 = gtk_file_chooser_dialog_new("Find Todo Database",
				      (GtkWindow *)parent,
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_OK, NULL);
    if (gtk_dialog_run(GTK_DIALOG(fd1)) == GTK_RESPONSE_OK) {
	gchar *filename;

	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fd1));
	tmpw = lookup_widget(parent, "entry_tododb");
	if (tmpw != NULL)
	    gtk_entry_set_text(GTK_ENTRY(tmpw), filename);
	g_free(filename);
    }

    gtk_widget_destroy(fd1);

    return FALSE;
}

gboolean on_button_findlogdb_button_press_event(GtkWidget *widget,
						 GdkEventButton *event,
						 gpointer user_data)
{
    GtkWidget *fd1, *parent, *tmpw;

    parent = (GtkWidget *)user_data;
    fd1 = gtk_file_chooser_dialog_new("Find Log Database",
				      (GtkWindow *)parent,
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_OK, NULL);
    if (gtk_dialog_run(GTK_DIALOG(fd1)) == GTK_RESPONSE_OK) {
	gchar *filename;

	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fd1));
	tmpw = lookup_widget(parent, "entry_logdb");
	if (tmpw != NULL)
	    gtk_entry_set_text(GTK_ENTRY(tmpw), filename);
	g_free(filename);
    }

    gtk_widget_destroy(fd1);

    return FALSE;
}

void toggle_options_emailtime(GtkButton *button, gpointer user_data)
{
    GtkWidget *parent, *time, *entry;
    gboolean state;

    parent = (GtkWidget *)user_data;

    time = lookup_widget(parent, "radio_email_time");
    entry = lookup_widget(parent, "entry_emailtime");
    if (entry == NULL || time == NULL)
	return;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(time));
    gtk_widget_set_sensitive(entry, state);
}

void on_check_sendemail_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *parent, *tmpw;
    gboolean state;

    parent = (GtkWidget *)user_data;

    state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));

    tmpw = lookup_widget(parent, "entry_emailaddr");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, state);
    tmpw = lookup_widget(parent, "radio_email_complete");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, state);
    tmpw = lookup_widget(parent, "radio_email_time");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, state);
    tmpw = lookup_widget(parent, "radio_freq_daily");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, state);
    tmpw = lookup_widget(parent, "radio_freq_weekly");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, state);
    tmpw = lookup_widget(parent, "radio_freq_monthly");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, state);
    tmpw = lookup_widget(parent, "check_reportproc");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, state);
    tmpw = lookup_widget(parent, "entry_mailer_agent");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, state);
    tmpw = lookup_widget(parent, "entry_emailtime");
}


void on_repeat_false_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *parent, *tmpw;

    parent = (GtkWidget *)user_data;

    tmpw = lookup_widget(parent, "entry_taskdate");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, TRUE);
    tmpw = lookup_widget(parent, "entry_tasktime");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, TRUE);

    tmpw = lookup_widget(parent, "check_daily");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_weekly");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_bi_weekly");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_monthly");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_bi_monthly");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_monday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_tuesday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_wednesday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_thursday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_friday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_saturday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_sunday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, FALSE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
}


void on_repeat_true_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *parent, *tmpw;

    parent = (GtkWidget *)user_data;

    tmpw = lookup_widget(parent, "entry_taskdate");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, FALSE);
    tmpw = lookup_widget(parent, "entry_tasktime");
    if (tmpw != NULL)
	gtk_widget_set_sensitive(tmpw, FALSE);

    tmpw = lookup_widget(parent, "check_daily");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_weekly");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_bi_weekly");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_monthly");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_bi_monthly");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_monday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_tuesday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_wednesday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_thursday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_friday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_saturday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
    tmpw = lookup_widget(parent, "check_sunday");
    if (tmpw != NULL) {
	gtk_widget_set_sensitive(tmpw, TRUE);
	gtk_toggle_button_set_active((GtkToggleButton *)tmpw, FALSE);
    }
}

void dialog_taskentry_delete(GtkWidget *widget, GdkEvent *event,
			     gpointer user_data)
{

}

void dialog_reminder_delete(GtkWidget *widget, GdkEvent *event,
			     gpointer user_data)
{
    GtkWidget *tmpw;
    char *text;
    task_t *task;
    todo_t *todo;

    tmpw = lookup_widget(widget, "label_taskname");
    if (tmpw == NULL)
        return;
    text = (char *)gtk_label_get_text(GTK_LABEL(tmpw));
    task = find_task_byname(text);
    if (task == NULL)
        return;
    todo = find_todo_byid(task->taskid);
    if (todo == NULL)
        return;

    todo->procdays++;
    todo->completed = 0;
    todo->window = NULL;
}
