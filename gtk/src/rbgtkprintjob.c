/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprinter.c -

  $Author: ktou $
  $Date: 2006/07/10 14:55:27 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#ifdef HAVE_GTK_GTKPRINTJOB_H

#include <gtk/gtkprintjob.h>
#ifndef GTK_TYPE_PRINT_CAPABILITIES
#  define GTK_TYPE_PRINT_CAPABILITIES (gtk_print_capabilities_get_type())
#endif

#ifdef HAVE_RB_CAIRO_H
#  include <rb_cairo.h>
#endif

#define _SELF(s) (GTK_PRINT_JOB(RVAL2GOBJ(s)))

#define RVAL2PRINTER(o) (GTK_PRINTER(RVAL2GOBJ(o)))
#define RVAL2SETTINGS(o) (GTK_PRINT_SETTINGS(RVAL2GOBJ(o)))
#define RVAL2SETUP(o) (GTK_PAGE_SETUP(RVAL2GOBJ(o)))

#define RVAL2PC(o) (RVAL2GENUM(o, GTK_TYPE_PRINT_CAPABILITIES))

#define STATUS2RVAL(o) (GENUM2RVAL(o, GTK_TYPE_PRINT_STATUS))

static VALUE gPrintJob;

static VALUE
pj_initialize(VALUE self, VALUE title, VALUE printer,
              VALUE settings, VALUE page_setup)
{
    GtkPrintJob *job;

    job = gtk_print_job_new(RVAL2CSTR(title), RVAL2PRINTER(printer),
                            RVAL2SETTINGS(settings),
                            RVAL2SETUP(page_setup));

    G_INITIALIZE(self, job);
    return Qnil;
}

static VALUE
pj_get_status(VALUE self)
{
    return STATUS2RVAL(gtk_print_job_get_status(_SELF(self)));
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
    int argc;
    VALUE *argv;
};

static VALUE
invoke_callback(VALUE data)
{
    struct callback_arg *arg = (struct callback_arg *)data;

    return rb_funcall2(arg->callback, id_call, arg->argc, arg->argv);
}
static void
complete_func(GtkPrintJob *job, gpointer data, GError *error)
{
    VALUE argv[2];
    struct callback_arg arg;

    argv[0] = GOBJ2RVAL(job);
    argv[1] = rbgerr_gerror2exception(error);

    arg.callback = (VALUE)data;
    arg.argc = 2;
    arg.argv = argv;

    G_PROTECT_CALLBACK(invoke_callback, &arg);
}

static void
remove_callback_reference(gpointer data)
{
    VALUE callback = (VALUE)data;
    G_CHILD_REMOVE(gPrintJob, callback);
}

static VALUE
pj_send(VALUE self)
{
    VALUE block;
    block = G_BLOCK_PROC();
    G_CHILD_ADD(gPrintJob, block);
    gtk_print_job_send(_SELF(self), complete_func, (gpointer)block,
                       remove_callback_reference);
    return Qnil;
}
#endif

void
Init_gtk_print_job()
{
#ifdef HAVE_GTK_GTKPRINTJOB_H
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
}
