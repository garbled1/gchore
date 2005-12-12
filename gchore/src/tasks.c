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

TAILQ_HEAD(, _task_t) tasktable = TAILQ_HEAD_INITIALIZER(tasktable);
TAILQ_HEAD(, _todo_t) todotable = TAILQ_HEAD_INITIALIZER(todotable);
time_t today;

extern options_t *options;
extern char *tzname[2];
extern guint timeout;

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
}

/**
   \brief calculate midnight of a given time
   \param time time to calc midnight for
   \return midnight today in time_t
*/

time_t calc_midnight(time_t time)
{
    struct tm *now_tm;
    time_t mid;

    now_tm = localtime(&time);
    /* make it midnight */
    now_tm->tm_sec = 0;
    now_tm->tm_min = 0;
    now_tm->tm_hour = 0;
    mid = mktime(now_tm);
    return(mid);
}

/**
   \brief scan tasks and todos to see if any new tasks are due
**/

gboolean scan_tasktable(gpointer data)
{
    task_t *task;
    todo_t *todo;
    time_t now, midnight;
    int addtask;
    struct tm *sday;

    midnight = calc_midnight(time(NULL));
    sday = localtime(&midnight);

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
	    if (task->dowe[0] || task->dow[sday->tm_wday]) {
		addtask++;
	    } else if ((task->dowe[1] && sday->tm_wday == 1) ||
		       (task->dowe[2] && sday->tm_wday == 1 &&
			((sday->tm_yday/7)%2 == 0)) ||
		       (task->dowe[3] && sday->tm_mday == 1) ||
		       (task->dowe[4] && sday->tm_mday == 1 &&
			sday->tm_mon%2 == 0)) {
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
	    now = calc_midnight(todo->completed);
	    if (difftime(time(NULL), todo->lastalert) > SECONDS_PER_DAY &&
		difftime(time(NULL), now) > SECONDS_PER_DAY) {
		todo->window = create_dialog_reminder(task->name, canproc);
		todo->lastalert = time(NULL);
		gtk_widget_show(todo->window);
	    }
	}
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
