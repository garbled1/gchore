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
   \file tasks.c
   \brief Task and todo list loading/saving/basic handling
   \author Tim Rightnour
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
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
#include "interface.h"
#include "support.h"

TAILQ_HEAD(, _task_t) tasktable = TAILQ_HEAD_INITIALIZER(tasktable);
TAILQ_HEAD(, _todo_t) todotable = TAILQ_HEAD_INITIALIZER(todotable);
TAILQ_HEAD(, _log_t) logtable = TAILQ_HEAD_INITIALIZER(logtable);
time_t today;

extern options_t *options;
extern char *tzname[2];
extern guint timeout;
extern GtkWidget *calendar_window;

/**
   \brief generate a monotonically increasing taskid number
   \return a taskid number guaranteed to be new
*/
int gen_new_taskid(void)
{
    int i;
    task_t *task;

    i = 1;
    TAILQ_FOREACH(task, &tasktable, next) {
	if (task->taskid >= i)
	    i = task->taskid + 1;
    }
    return(i);
}

/**
   \brief find a task by it's name
   \param name name of task to find
   \return a pointer to the task, NULL if none found
*/

task_t *find_task_byname(char *name)
{
    task_t *task;

    TAILQ_FOREACH(task, &tasktable, next) {
	if (!strcmp(task->name, name)) {
	    return(task);
	}
    }
    return(NULL);
}

/**
   \brief find a task by it's id
   \param id taskid of task to find
   \return a pointer to the task, NULL if none found
*/

task_t *find_task_byid(int id)
{
    task_t *task;

    TAILQ_FOREACH(task, &tasktable, next) {
	if (task->taskid == id) {
	    return(task);
	}
    }
    return(NULL);
}

/**
   \brief find a todo by it's id
   \param id taskid of todo to find
   \return a pointer to the todo, NULL if none found
*/

todo_t *find_todo_byid(int id)
{
    todo_t *todo;

    TAILQ_FOREACH(todo, &todotable, next) {
	if (todo->taskid == id) {
	    return(todo);
	}
    }
    return(NULL);
}

/**
   \brief remove a task from the list
   \param id taskid to remove
*/

void delete_task(int id)
{
    task_t *task;

    task = find_task_byid(id);
    if (task == NULL)
	return;
    TAILQ_REMOVE(&tasktable, task, next);
}

/**
   \brief remove a todo from the list
   \param id taskid to remove
*/

void delete_todo(int id)
{
    todo_t *todo;

    todo = find_todo_byid(id);
    if (todo == NULL)
	return;
    TAILQ_REMOVE(&todotable, todo, next);
}

/**
   \brief parse a task
   \param doc pointer to xml doc file
   \param cur expected to be at the level of the task tag on entry
   \param task pointer to task structure to be filled out
*/

void parse_task(xmlDocPtr doc, xmlNodePtr cur, task_t *task)
{
    xmlNodePtr child;

    child = cur->xmlChildrenNode;
    while (child != NULL) {
	if (!strcmp(child->name, "name"))
	    parsestring(doc, child, &task->name);
	if (!strcmp(child->name, "procrastinate"))
	    task->procrastinatable = 1;
	if (!strcmp(child->name, "sunday"))
	    task->dow[0] = 1;
	if (!strcmp(child->name, "monday"))
	    task->dow[1] = 1;
	if (!strcmp(child->name, "tuesday"))
	    task->dow[2] = 1;
	if (!strcmp(child->name, "wednesday"))
	    task->dow[3] = 1;
	if (!strcmp(child->name, "thursday"))
	    task->dow[4] = 1;
	if (!strcmp(child->name, "friday"))
	    task->dow[5] = 1;
	if (!strcmp(child->name, "saturday"))
	    task->dow[6] = 1;
	if (!strcmp(child->name, "daily"))
	    task->dowe[0] = 1;
	if (!strcmp(child->name, "weekly"))
	    task->dowe[1] = 1;
	if (!strcmp(child->name, "bi_weekly"))
	    task->dowe[2] = 1;
	if (!strcmp(child->name, "monthly"))
	    task->dowe[3] = 1;
	if (!strcmp(child->name, "bi_monthly"))
	    task->dowe[4] = 1;
	if (!strcmp(child->name, "email"))
	    task->email = 1;
	if (!strcmp(child->name, "pdays"))
	    parseint(doc, child, &task->pdays);
	if (!strcmp(child->name, "time"))
	    parselong(doc, child, &task->time);
	if (!strcmp(child->name, "taskid"))
	    parseint(doc, child, &task->taskid);
	child = child->next;
    }
}

/**
   \brief parse a todo
   \param doc pointer to xml doc file
   \param cur expected to be at the level of the todo tag on entry
   \param todo pointer to todo structure to be filled out
*/

void parse_todo(xmlDocPtr doc, xmlNodePtr cur, todo_t *todo)
{
    xmlNodePtr child;

    child = cur->xmlChildrenNode;
    while (child != NULL) {
	if (!strcmp(child->name, "procdays"))
	    parseint(doc, child, &todo->procdays);
	if (!strcmp(child->name, "completed"))
	    parselong(doc, child, &todo->completed);
	if (!strcmp(child->name, "lastalert"))
	    parselong(doc, child, &todo->lastalert);
	if (!strcmp(child->name, "taskid"))
	    parseint(doc, child, &todo->taskid);
	child = child->next;
    }
}

/**
   \brief write a task
   \param node an xmlNodePtr
   \param elm element name
   \param task task to write out
*/

void write_task(xmlNodePtr node, char *elm, task_t *task)
{
    xmlNodePtr child;

    child = xmlNewChild(node, NULL, elm, NULL);
    write_string(child, "name", task->name);
    write_boolean(child, "procrastinate", task->procrastinatable);
    write_boolean(child, "sunday", task->dow[0]);
    write_boolean(child, "monday", task->dow[1]);
    write_boolean(child, "tuesday", task->dow[2]);
    write_boolean(child, "wednesday", task->dow[3]);
    write_boolean(child, "thursday", task->dow[4]);
    write_boolean(child, "friday", task->dow[5]);
    write_boolean(child, "saturday", task->dow[6]);
    write_boolean(child, "daily", task->dowe[0]);
    write_boolean(child, "weekly", task->dowe[1]);
    write_boolean(child, "bi_weekly", task->dowe[2]);
    write_boolean(child, "monthly", task->dowe[3]);
    write_boolean(child, "bi_monthly", task->dowe[4]);
    write_boolean(child, "email", task->email);
    write_int(child, "pdays", task->pdays);
    write_long_if(child, "time", task->time);
    write_int(child, "taskid", task->taskid);
}

/**
   \brief write a todo
   \param node an xmlNodePtr
   \param elm element name
   \param todo todo to write out
*/

void write_todo(xmlNodePtr node, char *elm, todo_t *todo)
{
    xmlNodePtr child;

    child = xmlNewChild(node, NULL, elm, NULL);
    write_int(child, "procdays", todo->procdays);
    write_long(child, "completed", todo->completed);
    write_long(child, "lastalert", todo->lastalert);
    write_int(child, "taskid", todo->taskid);
}

/**
   \brief allocate a new task
   \return a new task
*/

task_t *new_task(void)
{
    task_t *task;

    task = smalloc(task_t);
    return(task);
}

/**
   \brief allocate a new todo
   \return a new todo
*/

todo_t *new_todo(void)
{
    todo_t *todo;

    todo = smalloc(todo_t);
    return(todo);
}

/**
   \brief parse the tasks file
   \param file filename to parse
*/

void parse_taskfile(char *file)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    task_t *task;

    doc = xmlParseFile(file);
    if (doc == NULL)
	return;

    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
	fprintf(stderr, "%s: empty document", file);
	return;
    }

    if (strcmp(cur->name, "Tasks")) {
	fprintf(stderr, "%s: root node != Tasks", file);
	return;
    }

    if (TAILQ_EMPTY(&tasktable))
	TAILQ_INIT(&tasktable);


    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if (!strcmp(cur->name, "todofile"))
	    parsestring(doc, cur, &options->tododb);
	if (!strcmp(cur->name, "logfile"))
	    parsestring(doc, cur, &options->logdb);
	if (!strcmp(cur->name, "task")) {
	    task = new_task();
	    parse_task(doc, cur, task);
	    TAILQ_INSERT_TAIL(&tasktable, task, next);
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    options->taskdb = strdup(file);
}

/**
   \brief parse the todos file
   \param file filename to parse
*/

void parse_todofile(char *file)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    todo_t *todo;

    doc = xmlParseFile(file);
    if (doc == NULL)
	return;

    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
	errormsg(GTK_MESSAGE_ERROR, "%s: empty document", file);
	return;
    }

    if (strcmp(cur->name, "Todos")) {
	errormsg(GTK_MESSAGE_ERROR, "%s: root node != Todos", file);
	return;
    }

    if (TAILQ_EMPTY(&todotable))
	TAILQ_INIT(&todotable);


    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if (!strcmp(cur->name, "taskfile"))
	    parsestring(doc, cur, &options->taskdb);
	if (!strcmp(cur->name, "logfile"))
	    parsestring(doc, cur, &options->logdb);
	if (!strcmp(cur->name, "todo")) {
	    todo = new_todo();
	    parse_todo(doc, cur, todo);
	    TAILQ_INSERT_TAIL(&todotable, todo, next);
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    options->tododb = strdup(file);
}

/**
   \brief write out a taskfile
   \param file full path to file
*/

void write_taskfile(char *file)
{
    xmlDocPtr doc;
    xmlNodePtr tree;
    task_t *task;

    doc = xmlNewDoc("1.0");
    doc->children = xmlNewDocNode(doc, NULL, "Tasks", NULL);
    tree = doc->children;
    write_string_if(tree, "todofile", options->tododb);
    write_string_if(tree, "logfile", options->logdb);
    TAILQ_FOREACH(task, &tasktable, next) {
	write_task(tree, "task", task);
    }
    xmlSaveFormatFile(file, doc, 1);
    xmlFreeDoc(doc);
}

/**
   \brief write out a todofile
   \param file full path to file
*/

void write_todofile(char *file)
{
    xmlDocPtr doc;
    xmlNodePtr tree;
    todo_t *todo;

    doc = xmlNewDoc("1.0");
    doc->children = xmlNewDocNode(doc, NULL, "Todos", NULL);
    tree = doc->children;
    write_string_if(tree, "taskfile", options->taskdb);
    write_string_if(tree, "logfile", options->logdb);
    TAILQ_FOREACH(todo, &todotable, next) {
	write_todo(tree, "todo", todo);
    }
    xmlSaveFormatFile(file, doc, 1);
    xmlFreeDoc(doc);
}

/**
   \brief save out some files if possible
*/

void save_some_files(void)
{
    /* if we have a taskdb, save it */
    if (options->taskdb != NULL) {
	write_taskfile(options->taskdb);
    }
    /* if we have a tododb, save it */
    if (options->tododb != NULL) {
	write_todofile(options->tododb);
    }
    /* and now the logfile */
    if (options->logdb != NULL) {
	write_logfile(options->logdb);
    }
}

/**
   \brief calculate midnight of a given time
   \param time time to calc midnight for
   \return midnight today in time_t
   This takes into account options->wakeup and modifies it by that much.
*/

time_t calc_midnight(time_t time)
{
    struct tm now_tm;
    time_t mid;

    (void)localtime_r(&time, &now_tm);
    /* make it midnight */
    now_tm.tm_sec = 0;
    now_tm.tm_min = 0;
    now_tm.tm_hour = 0;
    if (options->wakeup) {
	now_tm.tm_hour += options->wakeup/3600;
	now_tm.tm_min += (options->wakeup%3600)/60;
    }

    mid = mktime(&now_tm);
    return(mid);
}

/* LOG STUFF */

/**
   \brief allocate a new log
   \return a new log
*/

log_t *new_log(void)
{
    log_t *log;

    log = smalloc(log_t);
    return(log);
}

/**
   \brief parse a log
   \param doc pointer to xml doc file
   \param cur expected to be at the level of the log tag on entry
   \param log pointer to log structure to be filled out
*/

void parse_log(xmlDocPtr doc, xmlNodePtr cur, log_t *log)
{
    xmlNodePtr child;

    child = cur->xmlChildrenNode;
    while (child != NULL) {
	if (!strcmp(child->name, "procdays"))
	    parseint(doc, child, &log->procdays);
	if (!strcmp(child->name, "completed"))
	    parselong(doc, child, &log->completed);
	if (!strcmp(child->name, "taskid"))
	    parseint(doc, child, &log->taskid);
	child = child->next;
    }
}

/**
   \brief write a log
   \param node an xmlNodePtr
   \param elm element name
   \param log log to write out
*/

void write_log(xmlNodePtr node, char *elm, log_t *log)
{
    xmlNodePtr child;

    child = xmlNewChild(node, NULL, elm, NULL);
    write_int(child, "procdays", log->procdays);
    write_long(child, "completed", log->completed);
    write_int(child, "taskid", log->taskid);
}

/**
   \brief parse the logs file
   \param file filename to parse
*/

void parse_logfile(char *file)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    log_t *log;

    doc = xmlParseFile(file);
    if (doc == NULL)
	return;

    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
	errormsg(GTK_MESSAGE_ERROR, "%s: empty document", file);
	return;
    }

    if (strcmp(cur->name, "Logs")) {
	errormsg(GTK_MESSAGE_ERROR, "%s: root node != Logs", file);
	return;
    }

    if (TAILQ_EMPTY(&logtable))
	TAILQ_INIT(&logtable);


    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if (!strcmp(cur->name, "taskfile"))
	    parsestring(doc, cur, &options->taskdb);
	if (!strcmp(cur->name, "todofile"))
	    parsestring(doc, cur, &options->tododb);
	if (!strcmp(cur->name, "log")) {
	    log = new_log();
	    parse_log(doc, cur, log);
	    TAILQ_INSERT_TAIL(&logtable, log, next);
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);
    options->logdb = strdup(file);
}

/**
   \brief write out a logfile
   \param file full path to file
*/

void write_logfile(char *file)
{
    xmlDocPtr doc;
    xmlNodePtr tree;
    log_t *log;

    doc = xmlNewDoc("1.0");
    doc->children = xmlNewDocNode(doc, NULL, "Logs", NULL);
    tree = doc->children;
    write_string_if(tree, "taskfile", options->taskdb);
    write_string_if(tree, "todofile", options->tododb);
    TAILQ_FOREACH(log, &logtable, next) {
	write_log(tree, "log", log);
    }
    xmlSaveFormatFile(file, doc, 1);
    xmlFreeDoc(doc);
}

/**
   \brief send an email for a specific task having been completed
   \param log log entry for task
*/

void send_per_task_email(log_t *log)
{
    task_t *task;
    char sfn[20] = "/tmp/gchore.XXXXXX";
    char buf[2048];
    int fd = -1;
    FILE *sfp;

    if (options->emailaddr == NULL || options->sendmail == NULL)
	return;

    task = find_task_byid(log->taskid);
    if (task == NULL)
	return;

    if ((fd = mkstemp(sfn)) == -1 || (sfp = fdopen(fd, "w+")) == NULL) {
	if (fd != -1) {
	    unlink(sfn);
	    close(fd);
	}
	errormsg(GTK_MESSAGE_ERROR, "%s: %s\n", sfn, strerror(errno));
	return;
    }

    /* we have a task and a file handle.  yay */

    fprintf(sfp, "Subject: [GCHORE] Task %s completed\n\n", task->name);
    fprintf(sfp, "The task %s has been completed on %s", task->name,
	    ctime(&log->completed));
    if (options->reportproc && log->procdays)
	fprintf(sfp, "The task was procrastinated for %d days.\n",
		log->procdays);
    fclose(sfp);
    close(fd);

    snprintf(buf, 2048, "%s %s < %s", options->sendmail, options->emailaddr,
	     sfn);
    if (system(buf) == -1)
	errormsg(GTK_MESSAGE_ERROR, "Could not launch sendmail with command:\n"
		 "%s : %s\n", buf, strerror(errno));
    unlink(sfn);
}

/**
   \brief send out a scheduled email of tasks completed
*/

void send_task_email(void)
{
    task_t *task;
    log_t *log;
    char sfn[20] = "/tmp/gchore.XXXXXX";
    char buf[2048];
    int fd = -1;
    FILE *sfp;

    if (options->emailaddr == NULL || options->sendmail == NULL)
	return;

    if (TAILQ_EMPTY(&logtable))
	return;

    if ((fd = mkstemp(sfn)) == -1 || (sfp = fdopen(fd, "w+")) == NULL) {
	if (fd != -1) {
	    unlink(sfn);
	    close(fd);
	}
	errormsg(GTK_MESSAGE_ERROR, "%s: %s\n", sfn, strerror(errno));
	return;
    }

    fprintf(sfp, "Subject: [GCHORE] Task completion notice\n\n");

    while (TAILQ_FIRST(&logtable) != NULL) {
	log = TAILQ_FIRST(&logtable);
	task = find_task_byid(log->taskid);
	if (task != NULL) {
	    fprintf(sfp, "The task %s has been completed on %s", task->name,
		    ctime(&log->completed));
	    if (options->reportproc && log->procdays)
		fprintf(sfp, "The task was procrastinated for %d days.\n\n",
			log->procdays);
	    else
		fprintf(sfp, "\n");
	}
	TAILQ_REMOVE(&logtable, log, next);
	free(log);
    }

    fclose(sfp);
    close(fd);

    snprintf(buf, 2048, "%s %s < %s", options->sendmail, options->emailaddr,
	     sfn);
    if (system(buf) == -1)
	errormsg(GTK_MESSAGE_ERROR, "Could not launch sendmail with command:\n"
		 "%s : %s\n", buf, strerror(errno));
    unlink(sfn);

    if (options->logdb != NULL)
	write_logfile(options->logdb);
}

/**
   \brief log a task completion
   \param taskid taskid of task completed
   \param completed time task was completed
   \param procdays number of days task was procrastinated
*/

void log_task(int taskid, time_t completed, int procdays)
{
    log_t *logentry;

    logentry = new_log();
    logentry->taskid = taskid;
    logentry->completed = completed;
    logentry->procdays = procdays;

    if (options->emailfreq == EMAIL_COMPLETE) {
	send_per_task_email(logentry);
	free(logentry);
	return;
    }

    if (TAILQ_EMPTY(&logtable))
	TAILQ_INIT(&logtable);

    TAILQ_INSERT_TAIL(&logtable, logentry, next);
}

/**
   \brief scan tasks and todos to see if any new tasks are due
**/

gboolean scan_tasktable(gpointer data)
{
    task_t *task;
    todo_t *todo;
    time_t now, midnight, etime;
    int addtask, adder;
    struct tm sday;
    GtkWidget *cal;

    now = time(NULL);
    midnight = calc_midnight(now);
    (void)localtime_r(&midnight, &sday);

    /* update the calendar */
    cal = lookup_widget(calendar_window, "calendar1");
    if (cal != NULL) {
	gtk_calendar_select_month(GTK_CALENDAR(cal), sday.tm_mon,
				  sday.tm_year+1900);
	gtk_calendar_select_day(GTK_CALENDAR(cal), sday.tm_mday);
	gtk_calendar_mark_day(GTK_CALENDAR(cal), sday.tm_mday);
    }

    TAILQ_FOREACH(task, &tasktable, next) {
	/* first, scan the tasktable, and make sure every task has a todo */
	if (find_todo_byid(task->taskid) == NULL) {
	    todo = new_todo();
	    todo->taskid = task->taskid;
	    TAILQ_INSERT_TAIL(&todotable, todo, next);
	}
    }
    /* now we scan the todo list, and look for stuff to do */
    TAILQ_FOREACH(todo, &todotable, next) {
	addtask = 0;
	/* do we need to do this task now? */
	task = find_task_byid(todo->taskid);
	if (task == NULL)
	    continue;
	if (task->time) {
	    if (calc_midnight(task->time) == midnight)
		addtask++;
	} else {
	    if (task->dowe[0] || task->dow[sday.tm_wday]) {
		addtask++;
	    } else if ((task->dowe[1] && sday.tm_wday == 1) ||
		       (task->dowe[2] && sday.tm_wday == 1 &&
			((sday.tm_yday/7)%2 == 0)) ||
		       (task->dowe[3] && sday.tm_mday == 1) ||
		       (task->dowe[4] && sday.tm_mday == 1 &&
			sday.tm_mon%2 == 0)) {
		  addtask++;
	    } else if (task->dowe[1] && sday.tm_wday > 1) {
		/* weekly special check */
		if (difftime(now, calc_midnight(todo->completed))
		    > SECONDS_PER_DAY*(sday.tm_wday - 1))
		    addtask++;
	    } else if (task->dowe[2]) {
		adder = 0;
		if ((sday.tm_yday/7)%2 != 0)
		    adder = 7;
		if (difftime(now, calc_midnight(todo->completed))
		    > SECONDS_PER_DAY*(adder + sday.tm_wday -1))
		    addtask++;
	    } else if (task->dowe[3]) {
		if (difftime(now, calc_midnight(todo->completed))
		    > SECONDS_PER_DAY*sday.tm_mday)
		    addtask++;
	    } else if (task->dowe[4]) {
		adder = 0;
		if (sday.tm_mon%2 != 0)
		    adder = 30;
		if (difftime(now, calc_midnight(todo->completed))
		    > SECONDS_PER_DAY*(sday.tm_mday+adder))
		    addtask++;
	    }
	}
	if (addtask && todo->window != NULL) {
	    gtk_window_present((GtkWindow *)todo->window);
	} else if (addtask) {
	    int canproc;

	    canproc = task->procrastinatable;
	    if (canproc && todo->procdays >= task->pdays)
		canproc = 0;

	    /* theoretically this task needs to be done */
	    if (difftime(now,
			 calc_midnight(todo->lastalert)) > SECONDS_PER_DAY &&
		difftime(now,
			 calc_midnight(todo->completed)) > SECONDS_PER_DAY) {
		todo->window = create_dialog_reminder(task->name, canproc);
		todo->lastalert = now;
		gtk_widget_show(todo->window);
	    }
	}
    }

    /* now see if we owe anyone an email */
    midnight = calc_midnight(now);
    midnight -= options->wakeup;
    etime = midnight + options->emailtime;
    (void)localtime_r(&midnight, &sday);

    if (options->emailfreq == EMAIL_DAILY ||
	(options->emailfreq == EMAIL_WEEKLY && sday.tm_wday == 0) ||
	(options->emailfreq == EMAIL_MONTHLY && sday.tm_mday == 1)) {
	if (difftime(now, etime) >= 0 &&
	    difftime(now, etime) <= options->checkfrequency)
	    send_task_email();
    }

    /* rescedule the timeout */
    timeout = g_timeout_add(options->checkfrequency*1000,
			    scan_tasktable, NULL);
    save_some_files();
    return(FALSE);
}

/**
   \brief clean out all tasks
*/

void clean_all_tasks(void)
{
    /* if we have a taskdb, save it */
    if (options->taskdb != NULL) {
	write_taskfile(options->taskdb);
	options->taskdb = NULL;
    }

    while (TAILQ_FIRST(&tasktable) != NULL)
	TAILQ_REMOVE(&tasktable, TAILQ_FIRST(&tasktable), next);
}

/**
   \brief clean out all todos
*/

void clean_all_todos(void)
{
    /* if we have a tododb, save it */
    if (options->tododb != NULL) {
	write_todofile(options->tododb);
	options->tododb = NULL;
    }

    while (TAILQ_FIRST(&todotable) != NULL)
	TAILQ_REMOVE(&todotable, TAILQ_FIRST(&todotable), next);
}

/**
   \brief deal with a quit

   This keeps any popped reminders from getting delayed for a day
*/

void quit_program(void)
{
    todo_t *todo;

    TAILQ_FOREACH(todo, &todotable, next) {
	if (todo->window)
	    todo->lastalert = 0;
    }
    if (options->tododb)
	write_todofile(options->tododb);
    gtk_main_quit();
}
