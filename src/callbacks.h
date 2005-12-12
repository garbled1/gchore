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
