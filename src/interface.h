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
   \file interface.h
   \brief dialog and window creation headers
   \author Tim Rightnour
*/

GtkWidget *create_window1(void);
GtkWidget *create_dialog_taskentry(caltime_t *tod, int taskid);
GtkWidget *create_dialog_options(void);
GtkWidget *create_dialog_reminder(char *taskname, int procable);
GtkWidget *create_filechooserdialog1(void);
void errormsg(int sev, const char *msg, ...);
