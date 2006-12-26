/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprintjob.c -

  $Author: mutoh $
  $Date: 2006/12/26 15:47:52 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)
#ifdef GTK_PRINT_JOB_GET_TYPE

#include <gtk/gtkprintjob.h>
#ifndef GTK_TYPE_PRINT_CAPABILITIES
#  define GTK_TYPE_PRINT_CAPABILITIES (gtk_print_capabilities_get_type())
#endif

#ifdef HAVE_RB_CAIRO_H
#  include <rb_cairo.h>
#endif

#define _SELF(s) (GTK_PRINT_JOB(RVAL2GOBJ(s)))

static VALUE gPrintJob;

static VALUE
pj_initialize(VALUE self, VALUE title, VALUE printer,
              VALUE settings, VALUE page_setup)
{
    GtkPrintJob *job;

    job = gtk_print_job_new(RVAL2CSTR(title), 
                            GTK_PRINTER(RVAL2GOBJ(printer)),
                            GTK_PRINT_SETTINGS(RVAL2GOBJ(settings)),
                            GTK_PAGE_SETUP(RVAL2GOBJ(page_setup)));

    G_INITIALIZE(self, job);
    return Qnil;
}

/* Defined as properties.
GtkPrintSettings* gtk_print_job_get_settings
                                            (GtkPrintJob *job);
GtkPrinter* gtk_print_job_get_printer       (GtkPrintJob *job);
const gchar* gtk_print_job_get_title        (GtkPrintJob *job);
void        gtk_print_job_set_track_print_status
                                            (GtkPrintJob *job,
                                             gboolean track_status);
gboolean    gtk_print_job_get_track_print_status
                                            (GtkPrintJob *job);
*/

static VALUE
pj_get_status(VALUE self)
{
    return GENUM2RVAL(gtk_print_job_get_status(_SELF(self)), GTK_TYPE_PRINT_STATUS);
}

static VALUE
pj_set_source_file(VALUE self, VALUE filename)
{
    GError *error = NULL;
    gboolean result;

    result = gtk_print_job_set_source_file(_SELF(self), RVAL2CSTR(filename),
                                           &error);

    if (!result)
        RAISE_GERROR(error);

    return Qnil;
}

#ifdef HAVE_RB_CAIRO_H
static VALUE
pj_get_surface(VALUE self)
{
    cairo_surface_t *surface;
    GError *error = NULL;

    surface = gtk_print_job_get_surface(_SELF(self), &error);
    if (error)
        RAISE_GERROR(error);
    return CRSURFACE2RVAL(surface);
}
#endif

struct callback_arg
{
    VALUE callback;
    VALUE job;
    GError* error;
};

static VALUE
invoke_callback(VALUE data)
{
    struct callback_arg *arg = (struct callback_arg *)data;

    if (arg->error) RAISE_GERROR(arg->error);
    rb_funcall(arg->callback, id_call, 1, arg->job);
    return Qnil;
}
static void
complete_func(GtkPrintJob *job, gpointer data, GError *error)
{
    struct callback_arg arg;

    arg.callback = (VALUE)data;
    arg.job = GOBJ2RVAL(job);
    arg.error = error;
    G_PROTECT_CALLBACK(invoke_callback, &arg);
}

static void
remove_callback_reference(gpointer data)
{
    G_CHILD_REMOVE(gPrintJob, (VALUE)data);
}

static VALUE
pj_send(VALUE self)
{
    VALUE block = G_BLOCK_PROC();
    G_CHILD_ADD(gPrintJob, block);
    gtk_print_job_send(_SELF(self), complete_func, (gpointer)block,
                       remove_callback_reference);
    return self;
}
#endif
#endif

void
Init_gtk_print_job()
{
#if GTK_CHECK_VERSION(2,10,0)
#ifdef GTK_PRINT_JOB_GET_TYPE
    gPrintJob = G_DEF_CLASS(GTK_TYPE_PRINT_JOB, "PrintJob", mGtk);
    G_DEF_CLASS(GTK_TYPE_PRINT_CAPABILITIES, "PrintCapabilities", mGtk);

    rb_define_method(gPrintJob, "initialize", pj_initialize, 4);
    rb_define_method(gPrintJob, "status", pj_get_status, 0);
    rb_define_method(gPrintJob, "set_source_file", pj_set_source_file, 1);
#ifdef HAVE_RB_CAIRO_H
    rb_define_method(gPrintJob, "surface", pj_get_surface, 0);
#endif
    rb_define_method(gPrintJob, "send", pj_send, 0);

    G_DEF_SETTERS(gPrintJob);
#endif
#endif
}
