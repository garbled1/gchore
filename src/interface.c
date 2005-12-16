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
   \file interface.c
   \brief Dialog and window creation routines
   \author Tim Rightnour
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gchore.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"

GtkWidget *calendar_today;
options_t *options;
extern GtkWidget *calendar_window;

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget *create_window1(void)
{
    GtkWidget *window1;
    GtkWidget *vbox1;
    GtkWidget *menubar1;
    GtkWidget *menuitem4;
    GtkWidget *menuitem4_menu;
    GtkWidget *properties1;
    GtkWidget *new1;
    GtkWidget *open1;
    GtkWidget *save1;
    GtkWidget *save_as1;
    GtkWidget *separatormenuitem1;
    GtkWidget *quit1;
    GtkWidget *tasks;
    GtkWidget *tasks_menu;
    GtkWidget *new_task;
    GtkWidget *image16;
    GtkWidget *edit_task1;
    GtkWidget *image17;
    GtkWidget *delete_task1;
    GtkWidget *image18;
    GtkWidget *menuitem7;
    GtkWidget *menuitem7_menu;
    GtkWidget *about1;
    GtkWidget *calendar1;
    GtkWidget *frame1;
    GtkWidget *alignment1;
    GtkWidget *scrolledwindow1;
    GtkWidget *label5;
    GtkAccelGroup *accel_group;
    GtkTooltips *tooltips;

    tooltips = gtk_tooltips_new();

    accel_group = gtk_accel_group_new();

    window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window1), "gchore");

    vbox1 = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox1);
    gtk_container_add(GTK_CONTAINER(window1), vbox1);

    menubar1 = gtk_menu_bar_new();
    gtk_widget_show(menubar1);
    gtk_box_pack_start(GTK_BOX(vbox1), menubar1, FALSE, FALSE, 0);

    menuitem4 = gtk_menu_item_new_with_mnemonic("_File");
    gtk_widget_show(menuitem4);
    gtk_container_add(GTK_CONTAINER(menubar1), menuitem4);

    menuitem4_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem4), menuitem4_menu);

    properties1 = gtk_image_menu_item_new_from_stock("gtk-properties", accel_group);
    gtk_widget_show(properties1);
    gtk_container_add(GTK_CONTAINER(menuitem4_menu), properties1);

    new1 = gtk_image_menu_item_new_from_stock("gtk-new", accel_group);
    gtk_widget_show(new1);
    gtk_container_add(GTK_CONTAINER(menuitem4_menu), new1);

    open1 = gtk_image_menu_item_new_from_stock("gtk-open", accel_group);
    gtk_widget_show(open1);
    gtk_container_add(GTK_CONTAINER(menuitem4_menu), open1);

    save1 = gtk_image_menu_item_new_from_stock("gtk-save", accel_group);
    gtk_widget_show(save1);
    gtk_container_add(GTK_CONTAINER(menuitem4_menu), save1);

    save_as1 = gtk_image_menu_item_new_from_stock("gtk-save-as", accel_group);
    gtk_widget_show(save_as1);
    gtk_container_add(GTK_CONTAINER(menuitem4_menu), save_as1);

    separatormenuitem1 = gtk_separator_menu_item_new();
    gtk_widget_show(separatormenuitem1);
    gtk_container_add(GTK_CONTAINER(menuitem4_menu), separatormenuitem1);
    gtk_widget_set_sensitive(separatormenuitem1, FALSE);

    quit1 = gtk_image_menu_item_new_from_stock("gtk-quit", accel_group);
    gtk_widget_show(quit1);
    gtk_container_add(GTK_CONTAINER(menuitem4_menu), quit1);

    tasks = gtk_menu_item_new_with_mnemonic("_Tasks");
    gtk_widget_show(tasks);
    gtk_container_add(GTK_CONTAINER(menubar1), tasks);

    tasks_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(tasks), tasks_menu);

    new_task = gtk_image_menu_item_new_with_mnemonic("New Task");
    gtk_widget_show(new_task);
    gtk_container_add(GTK_CONTAINER(tasks_menu), new_task);
    gtk_tooltips_set_tip(tooltips, new_task, "Add a new task", NULL);
    gtk_widget_add_accelerator(new_task, "activate", accel_group,
			       GDK_N, (GdkModifierType) GDK_MOD1_MASK,
			       GTK_ACCEL_VISIBLE);

    image16 = gtk_image_new_from_stock("gtk-add", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image16);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(new_task), image16);

    edit_task1 = gtk_image_menu_item_new_with_mnemonic("Edit Task");
    gtk_widget_show(edit_task1);
    gtk_container_add(GTK_CONTAINER(tasks_menu), edit_task1);
    gtk_widget_add_accelerator(edit_task1, "activate", accel_group,
			       GDK_E, (GdkModifierType) GDK_MOD1_MASK,
			       GTK_ACCEL_VISIBLE);

    image17 = gtk_image_new_from_stock("gtk-edit", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image17);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(edit_task1), image17);

    delete_task1 = gtk_image_menu_item_new_with_mnemonic("Delete Task");
    gtk_widget_show(delete_task1);
    gtk_container_add(GTK_CONTAINER(tasks_menu), delete_task1);
    gtk_widget_add_accelerator(delete_task1, "activate", accel_group,
			       GDK_D, (GdkModifierType) GDK_MOD1_MASK,
			       GTK_ACCEL_VISIBLE);

    image18 = gtk_image_new_from_stock("gtk-delete", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image18);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(delete_task1), image18);

    menuitem7 = gtk_menu_item_new_with_mnemonic("_Help");
    gtk_widget_show(menuitem7);
    gtk_container_add(GTK_CONTAINER(menubar1), menuitem7);

    menuitem7_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem7), menuitem7_menu);

    about1 = gtk_image_menu_item_new_from_stock("gtk-about", accel_group);
    gtk_widget_show(about1);
    gtk_container_add(GTK_CONTAINER(menuitem7_menu), about1);

    calendar1 = gtk_calendar_new();
    gtk_widget_show(calendar1);
    gtk_box_pack_start(GTK_BOX(vbox1), calendar1, TRUE, TRUE, 0);
    gtk_calendar_display_options(GTK_CALENDAR(calendar1),
				 GTK_CALENDAR_SHOW_HEADING
				 | GTK_CALENDAR_SHOW_DAY_NAMES);

    frame1 = gtk_frame_new(NULL);
    gtk_widget_show(frame1);
    gtk_box_pack_start(GTK_BOX(vbox1), frame1, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(frame1), 1);
    gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_IN);

    alignment1 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment1);
    gtk_container_add(GTK_CONTAINER(frame1), alignment1);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment1), 0, 0, 12, 0);

    scrolledwindow1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(scrolledwindow1);
    gtk_container_add(GTK_CONTAINER(alignment1), scrolledwindow1);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow1), GTK_SHADOW_IN);

    calendar_today = gtk_text_view_new();
    gtk_widget_show(calendar_today);
    gtk_container_add(GTK_CONTAINER(scrolledwindow1), calendar_today);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(calendar_today), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(calendar_today), FALSE);

    label5 = gtk_label_new("Tasks on this day");
    gtk_widget_show(label5);
    gtk_frame_set_label_widget(GTK_FRAME(frame1), label5);
    gtk_label_set_use_markup(GTK_LABEL(label5), TRUE);

    g_signal_connect((gpointer)properties1, "activate",
		     G_CALLBACK(on_properties1_activate),
		     NULL);
    g_signal_connect((gpointer)new1, "activate",
		     G_CALLBACK(on_new1_activate),
		     NULL);
    g_signal_connect((gpointer)open1, "activate",
		     G_CALLBACK(on_open1_activate),
		     NULL);
    g_signal_connect((gpointer)save1, "activate",
		     G_CALLBACK(on_save1_activate),
		     NULL);
    g_signal_connect((gpointer)save_as1, "activate",
		     G_CALLBACK(on_save_as1_activate),
		     NULL);
    g_signal_connect((gpointer)quit1, "activate",
		     G_CALLBACK(on_quit1_activate),
		     NULL);
    g_signal_connect((gpointer)new_task, "activate",
		     G_CALLBACK(add_new_task),
		     NULL);
    g_signal_connect((gpointer)edit_task1, "activate",
		     G_CALLBACK(on_edit_task1_activate),
		     NULL);
    g_signal_connect((gpointer)delete_task1, "activate",
		     G_CALLBACK(on_delete_task1_activate),
		     NULL);
    g_signal_connect((gpointer)about1, "activate",
		     G_CALLBACK(on_about1_activate),
		     NULL);
    g_signal_connect((gpointer)calendar1, "day_selected",
		     G_CALLBACK(on_calendar1_day_selected),
		     NULL);
    g_signal_connect((gpointer)calendar1, "day_selected_double_click",
		     G_CALLBACK(on_calendar1_day_selected_double_click),
		     NULL);
    g_signal_connect((gpointer)window1, "delete_event",
		     G_CALLBACK(on_quit1_activate), NULL);


    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(window1, window1, "window1");
    GLADE_HOOKUP_OBJECT(window1, vbox1, "vbox1");
    GLADE_HOOKUP_OBJECT(window1, menubar1, "menubar1");
    GLADE_HOOKUP_OBJECT(window1, menuitem4, "menuitem4");
    GLADE_HOOKUP_OBJECT(window1, menuitem4_menu, "menuitem4_menu");
    GLADE_HOOKUP_OBJECT(window1, properties1, "properties1");
    GLADE_HOOKUP_OBJECT(window1, new1, "new1");
    GLADE_HOOKUP_OBJECT(window1, open1, "open1");
    GLADE_HOOKUP_OBJECT(window1, save1, "save1");
    GLADE_HOOKUP_OBJECT(window1, save_as1, "save_as1");
    GLADE_HOOKUP_OBJECT(window1, separatormenuitem1, "separatormenuitem1");
    GLADE_HOOKUP_OBJECT(window1, quit1, "quit1");
    GLADE_HOOKUP_OBJECT(window1, tasks, "tasks");
    GLADE_HOOKUP_OBJECT(window1, tasks_menu, "tasks_menu");
    GLADE_HOOKUP_OBJECT(window1, new_task, "new_task");
    GLADE_HOOKUP_OBJECT(window1, image16, "image16");
    GLADE_HOOKUP_OBJECT(window1, edit_task1, "edit_task1");
    GLADE_HOOKUP_OBJECT(window1, image17, "image17");
    GLADE_HOOKUP_OBJECT(window1, delete_task1, "delete_task1");
    GLADE_HOOKUP_OBJECT(window1, image18, "image18");
    GLADE_HOOKUP_OBJECT(window1, menuitem7, "menuitem7");
    GLADE_HOOKUP_OBJECT(window1, menuitem7_menu, "menuitem7_menu");
    GLADE_HOOKUP_OBJECT(window1, about1, "about1");
    GLADE_HOOKUP_OBJECT(window1, calendar1, "calendar1");
    GLADE_HOOKUP_OBJECT(window1, frame1, "frame1");
    GLADE_HOOKUP_OBJECT(window1, alignment1, "alignment1");
    GLADE_HOOKUP_OBJECT(window1, scrolledwindow1, "scrolledwindow1");
    GLADE_HOOKUP_OBJECT(window1, calendar_today, "calendar_today");
    GLADE_HOOKUP_OBJECT(window1, label5, "label5");
    GLADE_HOOKUP_OBJECT_NO_REF(window1, tooltips, "tooltips");

    gtk_window_add_accel_group(GTK_WINDOW(window1), accel_group);

    return window1;
}

GtkWidget *create_dialog_taskentry(caltime_t * tod, int taskid)
{
    GtkWidget *dialog_taskentry;
    GtkWidget *dialog_vbox1;
    GtkWidget *vbox2;
    GtkWidget *fixed5;
    GtkWidget *entry_taskname;
    GtkWidget *check_weekly;
    GtkWidget *entry_taskdate;
    GtkWidget *entry_tasktime;
    GtkWidget *label4;
    GtkWidget *label3;
    GtkObject *spinbutton1_adj;
    GtkWidget *spinbutton1;
    GtkWidget *label23;
    GtkWidget *repeat_false;
    GSList *repeat_group = NULL;
    GtkWidget *repeat_true;
    GtkWidget *check_daily;
    GtkWidget *label2;
    GtkWidget *label1;
    GtkWidget *check_noemail;
    GtkWidget *check_procrastinate;
    GtkWidget *entry_taskid;
    GtkWidget *check_sunday;
    GtkWidget *check_saturday;
    GtkWidget *check_friday;
    GtkWidget *check_thursday;
    GtkWidget *check_wednesday;
    GtkWidget *check_tuesday;
    GtkWidget *check_monday;
    GtkWidget *check_bi_weekly;
    GtkWidget *check_monthly;
    GtkWidget *check_bi_monthly;
    GtkWidget *dialog_action_area1;
    GtkWidget *applybutton1;
    GtkWidget *cancelbutton1;
    GtkWidget *button_oktaskedit;
    GtkTooltips *tooltips;
    char buf[256];
    task_t *task;

    task = NULL;
    tooltips = gtk_tooltips_new();

    dialog_taskentry = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog_taskentry), "Task Editor");
    gtk_window_set_type_hint(GTK_WINDOW(dialog_taskentry), GDK_WINDOW_TYPE_HINT_DIALOG);

    dialog_vbox1 = GTK_DIALOG(dialog_taskentry)->vbox;
    gtk_widget_show(dialog_vbox1);

    vbox2 = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox2);
    gtk_box_pack_start(GTK_BOX(dialog_vbox1), vbox2, TRUE, TRUE, 0);

    fixed5 = gtk_fixed_new();
    gtk_widget_show(fixed5);
    gtk_box_pack_start(GTK_BOX(vbox2), fixed5, TRUE, TRUE, 0);

    entry_taskname = gtk_entry_new();
    gtk_widget_show(entry_taskname);
    gtk_fixed_put(GTK_FIXED(fixed5), entry_taskname, 56, 0);
    gtk_widget_set_size_request(entry_taskname, 272, 24);

    check_weekly = gtk_check_button_new_with_mnemonic("Weekly");
    gtk_widget_show(check_weekly);
    gtk_fixed_put(GTK_FIXED(fixed5), check_weekly, 8, 72);
    gtk_widget_set_size_request(check_weekly, 100, 21);

    entry_taskdate = gtk_entry_new();
    if (tod != NULL) {
	sprintf(buf, "%d/%d/%d", tod->sday.tm_mon + 1, tod->sday.tm_mday,
		tod->sday.tm_year + 1900);
	gtk_entry_set_text((GtkEntry *) entry_taskdate, buf);
    }
    gtk_widget_show(entry_taskdate);
    gtk_fixed_put(GTK_FIXED(fixed5), entry_taskdate, 232, 128);
    gtk_widget_set_size_request(entry_taskdate, 96, 24);
    gtk_widget_set_sensitive(entry_taskdate, FALSE);

    entry_tasktime = gtk_entry_new();
    sprintf(buf, "0:00");
    gtk_entry_set_text((GtkEntry *) entry_tasktime, buf);
    gtk_widget_show(entry_tasktime);
    gtk_fixed_put(GTK_FIXED(fixed5), entry_tasktime, 232, 152);
    gtk_widget_set_size_request(entry_tasktime, 96, 24);
    gtk_widget_set_sensitive(entry_tasktime, FALSE);

    label4 = gtk_label_new("Time");
    gtk_widget_show(label4);
    gtk_fixed_put(GTK_FIXED(fixed5), label4, 184, 152);
    gtk_widget_set_size_request(label4, 48, 24);

    label3 = gtk_label_new("Date");
    gtk_widget_show(label3);
    gtk_fixed_put(GTK_FIXED(fixed5), label3, 184, 128);
    gtk_widget_set_size_request(label3, 48, 24);

    spinbutton1_adj = gtk_adjustment_new(1, 0, 100, 1, 10, 10);
    spinbutton1 = gtk_spin_button_new(GTK_ADJUSTMENT(spinbutton1_adj), 1, 0);
    gtk_widget_show(spinbutton1);
    gtk_fixed_put(GTK_FIXED(fixed5), spinbutton1, 288, 104);
    gtk_widget_set_size_request(spinbutton1, 52, 21);

    label23 = gtk_label_new("Procrastinatable Days");
    gtk_widget_show(label23);
    gtk_fixed_put(GTK_FIXED(fixed5), label23, 192, 96);
    gtk_widget_set_size_request(label23, 96, 32);
    gtk_label_set_line_wrap(GTK_LABEL(label23), TRUE);

    repeat_false = gtk_radio_button_new_with_mnemonic(NULL, "One time task");
    gtk_widget_show(repeat_false);
    gtk_fixed_put(GTK_FIXED(fixed5), repeat_false, 192, 176);
    gtk_widget_set_size_request(repeat_false, 136, 24);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(repeat_false), repeat_group);
    repeat_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(repeat_false));

    repeat_true = gtk_radio_button_new_with_mnemonic(NULL, "Repeated task");
    gtk_widget_show(repeat_true);
    gtk_fixed_put(GTK_FIXED(fixed5), repeat_true, 192, 200);
    gtk_widget_set_size_request(repeat_true, 136, 24);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(repeat_true), repeat_group);
    repeat_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(repeat_true));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(repeat_true), TRUE);

    check_daily = gtk_check_button_new_with_mnemonic("Daily");
    gtk_widget_show(check_daily);
    gtk_fixed_put(GTK_FIXED(fixed5), check_daily, 8, 48);
    gtk_widget_set_size_request(check_daily, 100, 21);

    label2 = gtk_label_new("Task needs to be done on these days");
    gtk_widget_show(label2);
    gtk_fixed_put(GTK_FIXED(fixed5), label2, 0, 24);
    gtk_widget_set_size_request(label2, 216, 24);

    label1 = gtk_label_new("Task");
    gtk_widget_show(label1);
    gtk_fixed_put(GTK_FIXED(fixed5), label1, 0, 0);
    gtk_widget_set_size_request(label1, 48, 24);
    gtk_label_set_line_wrap(GTK_LABEL(label1), TRUE);
    gtk_misc_set_padding(GTK_MISC(label1), 14, 0);

    check_noemail = gtk_check_button_new_with_mnemonic("Don't send emails");
    gtk_widget_show(check_noemail);
    gtk_fixed_put(GTK_FIXED(fixed5), check_noemail, 192, 48);
    gtk_widget_set_size_request(check_noemail, 128, 24);
    gtk_tooltips_set_tip(tooltips, check_noemail, "Don't send emails upon task completion", NULL);

    check_procrastinate = gtk_check_button_new_with_mnemonic("Can Procrastinate?");
    gtk_widget_show(check_procrastinate);
    gtk_fixed_put(GTK_FIXED(fixed5), check_procrastinate, 192, 72);
    gtk_widget_set_size_request(check_procrastinate, 136, 24);
    gtk_tooltips_set_tip(tooltips, check_procrastinate, "This task can be procrastinated?", NULL);

    entry_taskid = gtk_entry_new();
    sprintf(buf, "%d", taskid);
    gtk_entry_set_text((GtkEntry *) entry_taskid, buf);
    gtk_fixed_put(GTK_FIXED(fixed5), entry_taskid, 288, 336);
    gtk_widget_set_size_request(entry_taskid, 54, 21);
    gtk_widget_set_sensitive(entry_taskid, FALSE);
    GTK_WIDGET_UNSET_FLAGS(entry_taskid, GTK_CAN_FOCUS);
    gtk_editable_set_editable(GTK_EDITABLE(entry_taskid), FALSE);

    check_sunday = gtk_check_button_new_with_mnemonic("Sunday");
    gtk_widget_show(check_sunday);
    gtk_fixed_put(GTK_FIXED(fixed5), check_sunday, 8, 312);
    gtk_widget_set_size_request(check_sunday, 96, 24);

    check_saturday = gtk_check_button_new_with_mnemonic("Saturday");
    gtk_widget_show(check_saturday);
    gtk_fixed_put(GTK_FIXED(fixed5), check_saturday, 8, 288);
    gtk_widget_set_size_request(check_saturday, 88, 24);

    check_friday = gtk_check_button_new_with_mnemonic("Friday");
    gtk_widget_show(check_friday);
    gtk_fixed_put(GTK_FIXED(fixed5), check_friday, 8, 264);
    gtk_widget_set_size_request(check_friday, 80, 24);

    check_thursday = gtk_check_button_new_with_mnemonic("Thursday");
    gtk_widget_show(check_thursday);
    gtk_fixed_put(GTK_FIXED(fixed5), check_thursday, 8, 240);
    gtk_widget_set_size_request(check_thursday, 96, 24);

    check_wednesday = gtk_check_button_new_with_mnemonic("Wednesday");
    gtk_widget_show(check_wednesday);
    gtk_fixed_put(GTK_FIXED(fixed5), check_wednesday, 8, 216);
    gtk_widget_set_size_request(check_wednesday, 104, 24);

    check_tuesday = gtk_check_button_new_with_mnemonic("Tuesday");
    gtk_widget_show(check_tuesday);
    gtk_fixed_put(GTK_FIXED(fixed5), check_tuesday, 8, 192);
    gtk_widget_set_size_request(check_tuesday, 80, 24);

    check_monday = gtk_check_button_new_with_mnemonic("Monday");
    gtk_widget_show(check_monday);
    gtk_fixed_put(GTK_FIXED(fixed5), check_monday, 8, 168);
    gtk_widget_set_size_request(check_monday, 88, 24);

    check_bi_weekly = gtk_check_button_new_with_mnemonic("Bi-Weekly");
    gtk_widget_show(check_bi_weekly);
    gtk_fixed_put(GTK_FIXED(fixed5), check_bi_weekly, 8, 96);
    gtk_widget_set_size_request(check_bi_weekly, 100, 21);

    check_monthly = gtk_check_button_new_with_mnemonic("Monthly");
    gtk_widget_show(check_monthly);
    gtk_fixed_put(GTK_FIXED(fixed5), check_monthly, 8, 120);
    gtk_widget_set_size_request(check_monthly, 100, 21);

    check_bi_monthly = gtk_check_button_new_with_mnemonic("Bi-Monthly");
    gtk_widget_show(check_bi_monthly);
    gtk_fixed_put(GTK_FIXED(fixed5), check_bi_monthly, 8, 144);
    gtk_widget_set_size_request(check_bi_monthly, 100, 21);

    dialog_action_area1 = GTK_DIALOG(dialog_taskentry)->action_area;
    gtk_widget_show(dialog_action_area1);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area1), GTK_BUTTONBOX_SPREAD);

    applybutton1 = gtk_button_new_from_stock("gtk-apply");
    gtk_widget_show(applybutton1);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_taskentry), applybutton1, GTK_RESPONSE_APPLY);
    /* GTK_WIDGET_SET_FLAGS (applybutton1, GTK_CAN_DEFAULT); */

    cancelbutton1 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_show(cancelbutton1);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_taskentry), cancelbutton1, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(cancelbutton1, GTK_CAN_DEFAULT);

    button_oktaskedit = gtk_button_new_from_stock("gtk-ok");
    gtk_widget_show(button_oktaskedit);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_taskentry), button_oktaskedit, GTK_RESPONSE_OK);
    /* GTK_WIDGET_SET_FLAGS (button_oktaskedit, GTK_CAN_DEFAULT); */

    /* if we are editing an existing task, fill in all the boxes */
    if (taskid != -1)
	task = find_task_byid(taskid);
    if (task != NULL) {
	struct tm *tme;

	gtk_entry_set_text(GTK_ENTRY(entry_taskname), task->name);
	if (task->time != 0) {
	    tme = localtime(&task->time);
	    (void)strftime(buf, 256, "%m/%d/%Y", tme);
	    gtk_entry_set_text(GTK_ENTRY(entry_taskdate), buf);
	    (void)strftime(buf, 256, "%R", tme);
	    gtk_entry_set_text(GTK_ENTRY(entry_tasktime), buf);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(repeat_false),
					 TRUE);
	} else {
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(repeat_true), TRUE);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_daily),
					 task->dowe[0]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_weekly),
					 task->dowe[1]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_bi_weekly),
					 task->dowe[2]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_monthly),
					 task->dowe[3]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_bi_monthly),
					 task->dowe[4]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_sunday),
					 task->dow[0]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_monday),
					 task->dow[1]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_tuesday),
					 task->dow[2]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_wednesday),
					 task->dow[3]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_thursday),
					 task->dow[4]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_friday),
					 task->dow[5]);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_saturday),
					 task->dow[6]);
	}
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton1), task->pdays);
	if (task->email)
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_noemail),
					 FALSE);
	else
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_noemail),
					 TRUE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_procrastinate),
				     task->procrastinatable);
	sprintf(buf, "%d", taskid);
	gtk_entry_set_text((GtkEntry *) entry_taskid, buf);
    }

    g_signal_connect((gpointer) repeat_false, "clicked",
		     G_CALLBACK(on_repeat_false_clicked),
		     (gpointer) dialog_taskentry);
    g_signal_connect((gpointer) repeat_true, "clicked",
		     G_CALLBACK(on_repeat_true_clicked),
		     (gpointer) dialog_taskentry);
    g_signal_connect((gpointer) applybutton1, "button_press_event",
		     G_CALLBACK(on_applybutton1_button_press_event),
		     (gpointer) dialog_taskentry);
    g_signal_connect((gpointer) cancelbutton1, "button_press_event",
		     G_CALLBACK(on_cancelbutton1_button_press_event),
		     (gpointer) dialog_taskentry);
    g_signal_connect((gpointer) button_oktaskedit, "button_press_event",
		     G_CALLBACK(on_button_oktaskedit_button_press_event),
		     (gpointer) dialog_taskentry);
    g_signal_connect((gpointer) dialog_taskentry, "delete_event",
		     G_CALLBACK(dialog_taskentry_delete),
		     (gpointer) dialog_taskentry);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_taskentry, dialog_taskentry, "dialog_taskentry");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_taskentry, dialog_vbox1, "dialog_vbox1");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, vbox2, "vbox2");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, fixed5, "fixed5");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, entry_taskname, "entry_taskname");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_weekly, "check_weekly");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, entry_taskdate, "entry_taskdate");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, entry_tasktime, "entry_tasktime");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, label4, "label4");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, label3, "label3");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, spinbutton1, "spinbutton1");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, label23, "label23");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, repeat_false, "repeat_false");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, repeat_true, "repeat_true");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_daily, "check_daily");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, label2, "label2");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, label1, "label1");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_noemail, "check_noemail");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_procrastinate, "check_procrastinate");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, entry_taskid, "entry_taskid");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_sunday, "check_sunday");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_saturday, "check_saturday");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_friday, "check_friday");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_thursday, "check_thursday");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_wednesday, "check_wednesday");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_tuesday, "check_tuesday");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_monday, "check_monday");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_bi_weekly, "check_bi_weekly");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_monthly, "check_monthly");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, check_bi_monthly, "check_bi_monthly");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_taskentry, dialog_action_area1, "dialog_action_area1");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, applybutton1, "applybutton1");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, cancelbutton1, "cancelbutton1");
    GLADE_HOOKUP_OBJECT(dialog_taskentry, button_oktaskedit, "button_oktaskedit");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_taskentry, tooltips, "tooltips");

    free(tod);
    return dialog_taskentry;
}

GtkWidget *create_dialog_options(void)
{
    GtkWidget *dialog_options;
    GtkWidget *dialog_vbox2;
    GtkWidget *notebook1;
    GtkWidget *fixed4;
    GtkObject *spin_procrastinate_adj;
    GtkWidget *spin_procrastinate;
    GtkWidget *label6;
    GtkWidget *label15;
    GtkWidget *entry_taskdb;
    GtkWidget *label16;
    GtkWidget *entry_tododb;
    GtkWidget *button_findtaskdb;
    GtkWidget *button_findtododb;
    GtkObject *spin_updatetime_adj;
    GtkWidget *spin_updatetime;
    GtkWidget *label17;
    GtkWidget *label19;
    GtkWidget *entry_watch_directory;
    GtkWidget *label22;
    GtkWidget *fixed3;
    GtkWidget *label7;
    GtkWidget *entry_emailaddr;
    GtkWidget *radio_email_complete;
    GSList *radio_email_complete_group = NULL;
    GtkWidget *radio_email_time;
    GtkWidget *label8;
    GtkWidget *entry_emailtime;
    GtkWidget *radio_freq_daily;
    GSList *radio_freq_group = NULL;
    GtkWidget *radio_freq_weekly;
    GtkWidget *radio_freq_monthly;
    GtkWidget *check_reportproc;
    GtkWidget *entry_mailer_agent;
    GtkWidget *label18;
    GtkWidget *checkbutton1;
    GtkWidget *label21;
    GtkWidget *dialog_action_area2;
    GtkWidget *button_applyoptions;
    GtkWidget *button_canceloptions;
    GtkWidget *button_okoptions;
    GtkWidget *labelwu;
    GtkWidget *entry_wakeup;
    char buf[256];

    dialog_options = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog_options), "Options");
    gtk_window_set_type_hint(GTK_WINDOW(dialog_options), GDK_WINDOW_TYPE_HINT_DIALOG);

    dialog_vbox2 = GTK_DIALOG(dialog_options)->vbox;
    gtk_widget_show(dialog_vbox2);

    notebook1 = gtk_notebook_new();
    gtk_widget_show(notebook1);
    gtk_box_pack_start(GTK_BOX(dialog_vbox2), notebook1, TRUE, TRUE, 0);

    fixed4 = gtk_fixed_new();
    gtk_widget_show(fixed4);
    gtk_container_add(GTK_CONTAINER(notebook1), fixed4);

    spin_procrastinate_adj = gtk_adjustment_new(0, 0, 99, 1, 10, 10);
    spin_procrastinate = gtk_spin_button_new(GTK_ADJUSTMENT(spin_procrastinate_adj), 1, 0);
    gtk_widget_show(spin_procrastinate);
    gtk_fixed_put(GTK_FIXED(fixed4), spin_procrastinate, 272, 8);
    gtk_widget_set_size_request(spin_procrastinate, 0, 0);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin_procrastinate), TRUE);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_procrastinate),
			      options->maxproc);

    label6 = gtk_label_new("Maximum days task can be procrastinated");
    gtk_widget_show(label6);
    gtk_fixed_put(GTK_FIXED(fixed4), label6, 8, 8);
    gtk_widget_set_size_request(label6, 264, 24);
    gtk_label_set_line_wrap(GTK_LABEL(label6), TRUE);

    label15 = gtk_label_new("Task Database");
    gtk_widget_show(label15);
    gtk_fixed_put(GTK_FIXED(fixed4), label15, 8, 40);
    gtk_widget_set_size_request(label15, 88, 16);
    gtk_label_set_line_wrap(GTK_LABEL(label15), TRUE);

    entry_taskdb = gtk_entry_new();
    gtk_widget_show(entry_taskdb);
    gtk_fixed_put(GTK_FIXED(fixed4), entry_taskdb, 8, 56);
    gtk_widget_set_size_request(entry_taskdb, 158, 21);
    if (options->taskdb)
	gtk_entry_set_text(GTK_ENTRY(entry_taskdb), options->taskdb);

    label16 = gtk_label_new("Todo Database");
    gtk_widget_show(label16);
    gtk_fixed_put(GTK_FIXED(fixed4), label16, 8, 80);
    gtk_widget_set_size_request(label16, 88, 16);
    gtk_label_set_line_wrap(GTK_LABEL(label16), TRUE);

    entry_tododb = gtk_entry_new();
    gtk_widget_show(entry_tododb);
    gtk_fixed_put(GTK_FIXED(fixed4), entry_tododb, 8, 96);
    gtk_widget_set_size_request(entry_tododb, 158, 21);
    if (options->tododb)
	gtk_entry_set_text(GTK_ENTRY(entry_tododb), options->tododb);

    button_findtaskdb = gtk_button_new_from_stock("gtk-find");
    gtk_widget_show(button_findtaskdb);
    gtk_fixed_put(GTK_FIXED(fixed4), button_findtaskdb, 168, 56);
    gtk_widget_set_size_request(button_findtaskdb, 53, 23);

    button_findtododb = gtk_button_new_from_stock("gtk-find");
    gtk_widget_show(button_findtododb);
    gtk_fixed_put(GTK_FIXED(fixed4), button_findtododb, 168, 96);
    gtk_widget_set_size_request(button_findtododb, 53, 23);

    spin_updatetime_adj = gtk_adjustment_new(5, 5, 360, 1, 10, 10);
    spin_updatetime = gtk_spin_button_new(GTK_ADJUSTMENT(spin_updatetime_adj), 1, 0);
    gtk_widget_show(spin_updatetime);
    gtk_fixed_put(GTK_FIXED(fixed4), spin_updatetime, 192, 136);
    gtk_widget_set_size_request(spin_updatetime, 52, 21);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin_updatetime), TRUE);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_updatetime),
			      options->checkfrequency/60);

    label17 = gtk_label_new("Frequency in minutes to check for tasks to be done");
    gtk_widget_show(label17);
    gtk_fixed_put(GTK_FIXED(fixed4), label17, 8, 128);
    gtk_widget_set_size_request(label17, 176, 32);
    gtk_label_set_line_wrap(GTK_LABEL(label17), TRUE);

    label19 = gtk_label_new("Watch directory");
    gtk_widget_show(label19);
    gtk_fixed_put(GTK_FIXED(fixed4), label19, 8, 168);
    gtk_widget_set_size_request(label19, 144, 16);
    gtk_label_set_line_wrap(GTK_LABEL(label19), TRUE);

    entry_watch_directory = gtk_entry_new();
    gtk_widget_show(entry_watch_directory);
    gtk_fixed_put(GTK_FIXED(fixed4), entry_watch_directory, 8, 184);
    gtk_widget_set_size_request(entry_watch_directory, 232, 24);
    if (options->watchdir)
	gtk_entry_set_text(GTK_ENTRY(entry_watch_directory),options->watchdir);

    labelwu = gtk_label_new("When do you usually wake up?");
    gtk_widget_show(labelwu);
    gtk_fixed_put(GTK_FIXED(fixed4), labelwu, 8, 216);
    gtk_widget_set_size_request(labelwu, 232, 16);
    gtk_label_set_line_wrap(GTK_LABEL(labelwu), TRUE);

    entry_wakeup = gtk_entry_new();
    gtk_widget_show(entry_wakeup);
    gtk_fixed_put(GTK_FIXED(fixed4), entry_wakeup, 8, 232);
    gtk_widget_set_size_request(entry_wakeup, 64, 16);
    if (options->wakeup)
	sprintf(buf, "%d:%0.2d", options->wakeup/3600,
		(options->wakeup%3600)/60);
    else
	sprintf(buf, "6:00");
    gtk_entry_set_text(GTK_ENTRY(entry_wakeup), buf);

    label22 = gtk_label_new("General Options");
    gtk_widget_show(label22);
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook1), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook1), 0), label22);

    fixed3 = gtk_fixed_new();
    gtk_widget_show(fixed3);
    gtk_container_add(GTK_CONTAINER(notebook1), fixed3);

    label7 = gtk_label_new("Address to send updates to");
    gtk_widget_show(label7);
    gtk_fixed_put(GTK_FIXED(fixed3), label7, 8, 24);
    gtk_widget_set_size_request(label7, 160, 16);

    entry_emailaddr = gtk_entry_new();
    gtk_widget_show(entry_emailaddr);
    gtk_fixed_put(GTK_FIXED(fixed3), entry_emailaddr, 8, 40);
    gtk_widget_set_size_request(entry_emailaddr, 216, 24);
    gtk_widget_set_sensitive(entry_emailaddr, FALSE);
    if (options->emailaddr)
	gtk_entry_set_text(GTK_ENTRY(entry_emailaddr), options->emailaddr);

    radio_email_complete = gtk_radio_button_new_with_mnemonic(NULL, "Send email at task completion?");
    gtk_widget_show(radio_email_complete);
    gtk_fixed_put(GTK_FIXED(fixed3), radio_email_complete, 8, 72);
    gtk_widget_set_size_request(radio_email_complete, 208, 16);
    gtk_widget_set_sensitive(radio_email_complete, FALSE);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(radio_email_complete), radio_email_complete_group);
    radio_email_complete_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_email_complete));
    if (options->emailfreq == EMAIL_COMPLETE)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_email_complete),
				     TRUE);
    else
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_email_complete),
				     FALSE);

    radio_email_time = gtk_radio_button_new_with_mnemonic(NULL, "Send email at specific time?");
    gtk_widget_show(radio_email_time);
    gtk_fixed_put(GTK_FIXED(fixed3), radio_email_time, 8, 88);
    gtk_widget_set_size_request(radio_email_time, 184, 16);
    gtk_widget_set_sensitive(radio_email_time, FALSE);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(radio_email_time), radio_email_complete_group);
    radio_email_complete_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_email_time));
    if (options->emailtime)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_email_time),
				     TRUE);
    else
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_email_time),
				     FALSE);

    label8 = gtk_label_new("Time to send email");
    gtk_widget_show(label8);
    gtk_fixed_put(GTK_FIXED(fixed3), label8, 8, 104);
    gtk_widget_set_size_request(label8, 112, 16);
    gtk_misc_set_alignment(GTK_MISC(label8), 0, 0.5);

    entry_emailtime = gtk_entry_new();
    gtk_widget_show(entry_emailtime);
    gtk_fixed_put(GTK_FIXED(fixed3), entry_emailtime, 120, 104);
    gtk_widget_set_size_request(entry_emailtime, 64, 16);
    if (!options->emailtime)
	gtk_widget_set_sensitive(entry_emailtime, FALSE);
    if (options->emailtime)
	sprintf(buf, "%d:%0.2d", options->emailtime/3600,
		(options->emailtime%3600)/60);
    else
	sprintf(buf, "12:00");
    gtk_entry_set_text(GTK_ENTRY(entry_emailtime), buf);

    radio_freq_daily = gtk_radio_button_new_with_mnemonic(NULL, "Daily");
    gtk_widget_show(radio_freq_daily);
    gtk_fixed_put(GTK_FIXED(fixed3), radio_freq_daily, 8, 120);
    gtk_widget_set_size_request(radio_freq_daily, 94, 21);
    gtk_widget_set_sensitive(radio_freq_daily, FALSE);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(radio_freq_daily), radio_freq_group);
    radio_freq_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_freq_daily));
    if (options->emailfreq == EMAIL_DAILY)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_freq_daily),
				     TRUE);

    radio_freq_weekly = gtk_radio_button_new_with_mnemonic(NULL, "Weekly");
    gtk_widget_show(radio_freq_weekly);
    gtk_fixed_put(GTK_FIXED(fixed3), radio_freq_weekly, 8, 136);
    gtk_widget_set_size_request(radio_freq_weekly, 94, 21);
    gtk_widget_set_sensitive(radio_freq_weekly, FALSE);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(radio_freq_weekly), radio_freq_group);
    radio_freq_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_freq_weekly));
    if (options->emailfreq == EMAIL_WEEKLY)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_freq_weekly),
				     TRUE);

    radio_freq_monthly = gtk_radio_button_new_with_mnemonic(NULL, "Monthly");
    gtk_widget_show(radio_freq_monthly);
    gtk_fixed_put(GTK_FIXED(fixed3), radio_freq_monthly, 8, 152);
    gtk_widget_set_size_request(radio_freq_monthly, 94, 21);
    gtk_widget_set_sensitive(radio_freq_monthly, FALSE);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(radio_freq_monthly), radio_freq_group);
    radio_freq_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_freq_monthly));
    if (options->emailfreq == EMAIL_MONTHLY)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_freq_monthly),
				     TRUE);

    (void)gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_email_complete));
    (void)gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_email_time));
    (void)gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_freq_daily));
    (void)gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_freq_weekly));
    (void)gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_freq_monthly));


    check_reportproc = gtk_check_button_new_with_mnemonic("Report procrastination in email?");
    gtk_widget_show(check_reportproc);
    gtk_fixed_put(GTK_FIXED(fixed3), check_reportproc, 8, 176);
    gtk_widget_set_size_request(check_reportproc, 208, 16);
    gtk_widget_set_sensitive(check_reportproc, FALSE);
    if (options->reportproc)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_reportproc),TRUE);

    entry_mailer_agent = gtk_entry_new();
    gtk_widget_show(entry_mailer_agent);
    gtk_fixed_put(GTK_FIXED(fixed3), entry_mailer_agent, 8, 208);
    gtk_widget_set_size_request(entry_mailer_agent, 232, 24);
    gtk_widget_set_sensitive(entry_mailer_agent, FALSE);
    if (options->sendmail)
	gtk_entry_set_text(GTK_ENTRY(entry_mailer_agent), options->sendmail);

    label18 = gtk_label_new("Pathname of mailer agent");
    gtk_widget_show(label18);
    gtk_fixed_put(GTK_FIXED(fixed3), label18, 8, 192);
    gtk_widget_set_size_request(label18, 176, 16);
    gtk_label_set_line_wrap(GTK_LABEL(label18), TRUE);

    label21 = gtk_label_new("Email Options");
    gtk_widget_show(label21);
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook1), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook1), 1), label21);

    dialog_action_area2 = GTK_DIALOG(dialog_options)->action_area;
    gtk_widget_show(dialog_action_area2);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area2), GTK_BUTTONBOX_SPREAD);

    button_applyoptions = gtk_button_new_from_stock("gtk-apply");
    gtk_widget_show(button_applyoptions);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_options), button_applyoptions, GTK_RESPONSE_APPLY);
    GTK_WIDGET_SET_FLAGS(button_applyoptions, GTK_CAN_DEFAULT);

    button_canceloptions = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_show(button_canceloptions);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_options), button_canceloptions, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(button_canceloptions, GTK_CAN_DEFAULT);

    button_okoptions = gtk_button_new_from_stock("gtk-ok");
    gtk_widget_show(button_okoptions);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_options), button_okoptions, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(button_okoptions, GTK_CAN_DEFAULT);

    checkbutton1 = gtk_check_button_new_with_mnemonic("Send email updates?");
    gtk_widget_show(checkbutton1);
    gtk_fixed_put(GTK_FIXED(fixed3), checkbutton1, 0, 0);
    gtk_widget_set_size_request(checkbutton1, 144, 21);

    g_signal_connect((gpointer)button_findtaskdb, "button_press_event",
		     G_CALLBACK(on_button_findtaskdb_button_press_event),
		     (gpointer)dialog_options);
    g_signal_connect((gpointer)button_findtododb, "button_press_event",
		     G_CALLBACK(on_button_findtododb_button_press_event),
		     (gpointer)dialog_options);
    g_signal_connect((gpointer)checkbutton1, "clicked",
		     G_CALLBACK(on_check_sendemail_clicked),
		     (gpointer)dialog_options);
    g_signal_connect((gpointer)button_applyoptions, "button_press_event",
		     G_CALLBACK(on_button_applyoptions_button_press_event),
		     (gpointer)dialog_options);
    g_signal_connect((gpointer)button_canceloptions, "button_press_event",
		     G_CALLBACK(on_button_canceloptions_button_press_event),
		     (gpointer)dialog_options);
    g_signal_connect((gpointer)button_okoptions, "button_press_event",
		     G_CALLBACK(on_button_okoptions_button_press_event),
		     (gpointer)dialog_options);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_options, dialog_options, "dialog_options");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_options, dialog_vbox2, "dialog_vbox2");
    GLADE_HOOKUP_OBJECT(dialog_options, notebook1, "notebook1");
    GLADE_HOOKUP_OBJECT(dialog_options, fixed4, "fixed4");
    GLADE_HOOKUP_OBJECT(dialog_options, spin_procrastinate, "spin_procrastinate");
    GLADE_HOOKUP_OBJECT(dialog_options, label6, "label6");
    GLADE_HOOKUP_OBJECT(dialog_options, label15, "label15");
    GLADE_HOOKUP_OBJECT(dialog_options, entry_taskdb, "entry_taskdb");
    GLADE_HOOKUP_OBJECT(dialog_options, label16, "label16");
    GLADE_HOOKUP_OBJECT(dialog_options, entry_tododb, "entry_tododb");
    GLADE_HOOKUP_OBJECT(dialog_options, button_findtaskdb, "button_findtaskdb");
    GLADE_HOOKUP_OBJECT(dialog_options, button_findtododb, "button_findtododb");
    GLADE_HOOKUP_OBJECT(dialog_options, spin_updatetime, "spin_updatetime");
    GLADE_HOOKUP_OBJECT(dialog_options, label17, "label17");
    GLADE_HOOKUP_OBJECT(dialog_options, label19, "label19");
    GLADE_HOOKUP_OBJECT(dialog_options, entry_watch_directory, "entry_watch_directory");
    GLADE_HOOKUP_OBJECT(dialog_options, labelwu, "labelwu");
    GLADE_HOOKUP_OBJECT(dialog_options, entry_wakeup, "entry_wakeup");
    GLADE_HOOKUP_OBJECT(dialog_options, label22, "label22");
    GLADE_HOOKUP_OBJECT(dialog_options, fixed3, "fixed3");
    GLADE_HOOKUP_OBJECT(dialog_options, label7, "label7");
    GLADE_HOOKUP_OBJECT(dialog_options, entry_emailaddr, "entry_emailaddr");
    GLADE_HOOKUP_OBJECT(dialog_options, radio_email_complete, "radio_email_complete");
    GLADE_HOOKUP_OBJECT(dialog_options, radio_email_time, "radio_email_time");
    GLADE_HOOKUP_OBJECT(dialog_options, label8, "label8");
    GLADE_HOOKUP_OBJECT(dialog_options, entry_emailtime, "entry_emailtime");
    GLADE_HOOKUP_OBJECT(dialog_options, radio_freq_daily, "radio_freq_daily");
    GLADE_HOOKUP_OBJECT(dialog_options, radio_freq_weekly, "radio_freq_weekly");
    GLADE_HOOKUP_OBJECT(dialog_options, radio_freq_monthly, "radio_freq_monthly");
    GLADE_HOOKUP_OBJECT(dialog_options, check_reportproc, "check_reportproc");
    GLADE_HOOKUP_OBJECT(dialog_options, entry_mailer_agent, "entry_mailer_agent");
    GLADE_HOOKUP_OBJECT(dialog_options, label18, "label18");
    GLADE_HOOKUP_OBJECT(dialog_options, checkbutton1, "checkbutton1");
    GLADE_HOOKUP_OBJECT(dialog_options, label21, "label21");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_options, dialog_action_area2, "dialog_action_area2");
    GLADE_HOOKUP_OBJECT(dialog_options, button_applyoptions, "button_applyoptions");
    GLADE_HOOKUP_OBJECT(dialog_options, button_canceloptions, "button_canceloptions");
    GLADE_HOOKUP_OBJECT(dialog_options, button_okoptions, "button_okoptions");

    /* we do this at the bottom, so all the signals and hookups have been
       wired, as it will emit a signal */
    if (options->emailfreq != 0)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), TRUE);

    return dialog_options;
}

GtkWidget *create_dialog_reminder(char *taskname, int procable)
{
    GtkWidget *dialog_reminder;
    GtkWidget *dialog_vbox3;
    GtkWidget *label_taskname;
    GtkWidget *dialog_action_area3;
    GtkWidget *button_remindercomplete;
    GtkWidget *alignment3;
    GtkWidget *hbox6;
    GtkWidget *image2;
    GtkWidget *label12;
    GtkWidget *button_reminderprocrastinate;
    GtkWidget *alignment4;
    GtkWidget *hbox7;
    GtkWidget *image3;
    GtkWidget *label13;
    char *markup;

    dialog_reminder = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog_reminder), "Reminder");
    gtk_window_set_type_hint(GTK_WINDOW(dialog_reminder), GDK_WINDOW_TYPE_HINT_DIALOG);

    dialog_vbox3 = GTK_DIALOG(dialog_reminder)->vbox;
    gtk_widget_show(dialog_vbox3);

    label_taskname = gtk_label_new(NULL);
    markup = g_markup_printf_escaped("<span size=\"xx-large\">%s</span>",
				     taskname);
    gtk_label_set_markup(GTK_LABEL(label_taskname), markup);
    g_free(markup);
    gtk_widget_show(label_taskname);
    gtk_box_pack_start(GTK_BOX(dialog_vbox3), label_taskname, TRUE, TRUE, 0);
    gtk_label_set_justify(GTK_LABEL(label_taskname), GTK_JUSTIFY_CENTER);

    dialog_action_area3 = GTK_DIALOG(dialog_reminder)->action_area;
    gtk_widget_show(dialog_action_area3);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area3), GTK_BUTTONBOX_SPREAD);

    button_remindercomplete = gtk_button_new();
    gtk_widget_show(button_remindercomplete);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_reminder), button_remindercomplete, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(button_remindercomplete, GTK_CAN_DEFAULT);

    alignment3 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment3);
    gtk_container_add(GTK_CONTAINER(button_remindercomplete), alignment3);

    hbox6 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox6);
    gtk_container_add(GTK_CONTAINER(alignment3), hbox6);

    image2 = gtk_image_new_from_stock("gtk-apply", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image2);
    gtk_box_pack_start(GTK_BOX(hbox6), image2, FALSE, FALSE, 0);

    label12 = gtk_label_new_with_mnemonic("Complete");
    gtk_widget_show(label12);
    gtk_box_pack_start(GTK_BOX(hbox6), label12, FALSE, FALSE, 0);

    button_reminderprocrastinate = gtk_button_new();
    gtk_widget_show(button_reminderprocrastinate);
    if (!procable)
	gtk_widget_set_sensitive(button_reminderprocrastinate, FALSE);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_reminder), button_reminderprocrastinate, 0);
    GTK_WIDGET_SET_FLAGS(button_reminderprocrastinate, GTK_CAN_DEFAULT);

    alignment4 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment4);
    gtk_container_add(GTK_CONTAINER(button_reminderprocrastinate), alignment4);

    hbox7 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox7);
    gtk_container_add(GTK_CONTAINER(alignment4), hbox7);

    image3 = gtk_image_new_from_stock("gtk-add", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image3);
    gtk_box_pack_start(GTK_BOX(hbox7), image3, FALSE, FALSE, 0);

    label13 = gtk_label_new_with_mnemonic("Procrastinate");
    gtk_widget_show(label13);
    gtk_box_pack_start(GTK_BOX(hbox7), label13, FALSE, FALSE, 0);

    g_signal_connect((gpointer)button_remindercomplete, "button_press_event",
		  G_CALLBACK(on_button_remindercomplete_button_press_event),
		     (gpointer)dialog_reminder);
    g_signal_connect((gpointer)button_reminderprocrastinate, "button_press_event",
	     G_CALLBACK(on_button_reminderprocrastinate_button_press_event),
		     (gpointer)dialog_reminder);
    g_signal_connect((gpointer)dialog_reminder, "delete_event",
		     G_CALLBACK(dialog_reminder_delete),
		     (gpointer) dialog_reminder);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_reminder, dialog_reminder, "dialog_reminder");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_reminder, dialog_vbox3, "dialog_vbox3");
    GLADE_HOOKUP_OBJECT(dialog_reminder, label_taskname, "label_taskname");
    GLADE_HOOKUP_OBJECT_NO_REF(dialog_reminder, dialog_action_area3, "dialog_action_area3");
    GLADE_HOOKUP_OBJECT(dialog_reminder, button_remindercomplete, "button_remindercomplete");
    GLADE_HOOKUP_OBJECT(dialog_reminder, alignment3, "alignment3");
    GLADE_HOOKUP_OBJECT(dialog_reminder, hbox6, "hbox6");
    GLADE_HOOKUP_OBJECT(dialog_reminder, image2, "image2");
    GLADE_HOOKUP_OBJECT(dialog_reminder, label12, "label12");
    GLADE_HOOKUP_OBJECT(dialog_reminder, button_reminderprocrastinate, "button_reminderprocrastinate");
    GLADE_HOOKUP_OBJECT(dialog_reminder, alignment4, "alignment4");
    GLADE_HOOKUP_OBJECT(dialog_reminder, hbox7, "hbox7");
    GLADE_HOOKUP_OBJECT(dialog_reminder, image3, "image3");
    GLADE_HOOKUP_OBJECT(dialog_reminder, label13, "label13");

    return dialog_reminder;
}

GtkWidget *create_filechooserdialog1(void)
{
    GtkWidget *filechooserdialog1;
    GtkWidget *dialog_vbox4;
    GtkWidget *dialog_action_area4;
    GtkWidget *button2;
    GtkWidget *button3;

    filechooserdialog1 = gtk_file_chooser_dialog_new("", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
    gtk_window_set_type_hint(GTK_WINDOW(filechooserdialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

    dialog_vbox4 = GTK_DIALOG(filechooserdialog1)->vbox;
    gtk_widget_show(dialog_vbox4);

    dialog_action_area4 = GTK_DIALOG(filechooserdialog1)->action_area;
    gtk_widget_show(dialog_action_area4);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area4), GTK_BUTTONBOX_END);

    button2 = gtk_button_new_from_stock("gtk-cancel");
    gtk_widget_show(button2);
    gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog1), button2, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(button2, GTK_CAN_DEFAULT);

    button3 = gtk_button_new_from_stock("gtk-open");
    gtk_widget_show(button3);
    gtk_dialog_add_action_widget(GTK_DIALOG(filechooserdialog1), button3, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(button3, GTK_CAN_DEFAULT);
    g_signal_connect((gpointer) button3, "button_press_event",
		     G_CALLBACK(button3_button_press_event),
		     (gpointer)filechooserdialog1);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, filechooserdialog1, "filechooserdialog1");
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, dialog_vbox4, "dialog_vbox4");
    GLADE_HOOKUP_OBJECT_NO_REF(filechooserdialog1, dialog_action_area4, "dialog_action_area4");
    GLADE_HOOKUP_OBJECT(filechooserdialog1, button2, "button2");
    GLADE_HOOKUP_OBJECT(filechooserdialog1, button3, "button3");

    gtk_widget_grab_default(button3);
    return filechooserdialog1;
}

void errormsg(int sev, const char *msg, ...)
{
    va_list args;
    GtkWidget *dialog;
    char buf[2048];

    va_start(args, msg);
    vsnprintf(buf, 2048, msg, args);
    va_end(args);
    dialog = gtk_message_dialog_new(
	(GtkWindow *)calendar_window, GTK_DIALOG_DESTROY_WITH_PARENT, sev,
	GTK_BUTTONS_CLOSE, buf);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
