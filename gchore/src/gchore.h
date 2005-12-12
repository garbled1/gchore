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
   \file gchore.h
   \brief Basic structures for Gchore
   \author Tim Rightnour
*/

#ifndef __gchore_h__
#define __gchore_h__

#include <gtk/gtk.h>
#if defined(__linux__)
#include "queue.h"
#else
#include <sys/queue.h>
#endif
#include <time.h>

/** \brief task structure */
typedef struct _task_t task_t;
/** \brief todo structure */
typedef struct _todo_t todo_t;

/** \brief task structure */
struct _task_t {
    TAILQ_ENTRY(_task_t) next;  /**< \brief doubly linked list */
    char *name;                 /**< \brief task name */
    int procrastinatable;
    int dow[7];			/**< \brief sun-sat */
    int dowe[5];		/**< \brief daily,week,bi-week, mon, bi-mon */
    int pdays;			/**< \brief days we can procrastinate */
    int email;			/**< \brief send email at completion? */
    time_t time;
    int taskid;
};

/** \brief todo list strucuture */
struct _todo_t {
    TAILQ_ENTRY(_todo_t) next;
    int taskid;
    int procdays;
    time_t completed;
    time_t lastalert;
    GtkWidget *window;
};

/** \brief calendar time structure */
typedef struct _caltime_t {
    struct tm sday;
    time_t midnight;
} caltime_t;

#define EMAIL_COMPLETE 1
#define EMAIL_DAILY 2
#define EMAIL_WEEKLY 3
#define EMAIL_MONTHLY 4

/** \brief options structure */
typedef struct _options_t {
    int maxproc;       /**< \brief maximum days to allow procrastination */
    char *emailaddr;   /**< \brief addr to email reports to */
    int emailfreq;     /**< \brief frequency to email reports, EMAIL_* */
    int emailtime;     /**< \brief seconds past midnight to mail report */
    int reportproc;    /**< \brief report procrastinations in report */
    char *taskdb;      /**< \brief default taskdb to open at start */
    char *tododb;      /**< \brief default tododb to open at start */
    char *watchdir;    /**< \brief directory to watch for updates */
    char *sendmail;    /**< \brief path to sendmail binary */
    int checkfrequency;/**< \brief seconds to wait between task checks */
} options_t;

#define SECONDS_PER_DAY 86400
#define MAX_PATH 2048
#define MAX_ITEMS_PER_DAY 30

/* declarations, because this program is too small to make lots of little
   files */

/* options.c */
void parse_options(xmlDocPtr doc, xmlNodePtr cur, options_t *options);
void write_options(xmlNodePtr node, options_t *options);
int parse_optionsfile(void);
void write_optionsfile(void);

/* tasks.c */
int gen_new_taskid(void);
task_t *find_task_byname(char *name);
task_t *find_task_byid(int id);
todo_t *find_todo_byid(int id);
void delete_task(int id);
void delete_todo(int id);
void parse_task(xmlDocPtr doc, xmlNodePtr cur, task_t *task);
void parse_todo(xmlDocPtr doc, xmlNodePtr cur, todo_t *todo);
void write_task(xmlNodePtr node, char *elm, task_t *task);
void write_todo(xmlNodePtr node, char *elm, todo_t *todo);
task_t *new_task(void);
todo_t *new_todo(void);
void parse_taskfile(char *file);
void parse_todofile(char *file);
void write_taskfile(char *file);
void write_todofile(char *file);
void save_some_files(void);
time_t calc_midnight(time_t time);
gboolean scan_tasktable(gpointer data);
void clean_all_tasks(void);
void clean_all_todos(void);

#endif /* __gchore_h__ */
