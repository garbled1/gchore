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
   \file callbacks.h
   \brief callback headers
   \author Tim Rightnour
*/

#include <gtk/gtk.h>


void
on_properties1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean button3_button_press_event(GtkWidget *widget,
				    GdkEventButton *event,
				    gpointer user_data);

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
add_new_task                           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_delete_task1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void on_edit_task1_activate(GtkMenuItem *menuitem, gpointer user_data);
void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_calendar1_day_selected              (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_calendar1_day_selected_double_click (GtkCalendar     *calendar,
                                        gpointer         user_data);

gboolean
on_applybutton1_button_press_event     (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_cancelbutton1_button_press_event    (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_oktaskedit_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_applyoptions_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_canceloptions_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_okoptions_button_press_event (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_remindercomplete_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_reminderprocrastinate_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_findtaskdb_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_findtododb_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);
gboolean
on_button_findlogdb_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void toggle_options_emailtime(GtkButton *button, gpointer user_data);
void
on_check_sendemail_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_repeat_false_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_repeat_true_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_button_edit_cancel_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_button_editok_button_press_event    (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_edit_task1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void dialog_taskentry_delete(GtkWidget *widget, GdkEvent *event,
                             gpointer user_data);

void dialog_reminder_delete(GtkWidget *widget, GdkEvent *event,
			    gpointer user_data);
