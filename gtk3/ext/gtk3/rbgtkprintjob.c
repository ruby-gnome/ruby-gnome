/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#ifdef HAVE_GTK_UNIX_PRINT
#include <gtk/gtkprintjob.h>
#ifndef GTK_TYPE_PRINT_CAPABILITIES
#  define GTK_TYPE_PRINT_CAPABILITIES (gtk_print_capabilities_get_type())
#endif

#ifdef HAVE_RB_CAIRO_H
#  include <rb_cairo.h>
#endif

#define RG_TARGET_NAMESPACE cPrintJob
#define _SELF(s) (GTK_PRINT_JOB(RVAL2GOBJ(s)))

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_initialize(VALUE self, VALUE title, VALUE printer,
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

static VALUE
rg_status(VALUE self)
{
    return GENUM2RVAL(gtk_print_job_get_status(_SELF(self)), GTK_TYPE_PRINT_STATUS);
}

static VALUE
rg_set_source_file(VALUE self, VALUE filename)
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
rg_surface(VALUE self)
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
    G_CHILD_REMOVE(RG_TARGET_NAMESPACE, (VALUE)data);
}

static VALUE
rg_send(VALUE self)
{
    VALUE block = rb_block_proc();
    G_CHILD_ADD(RG_TARGET_NAMESPACE, block);
    gtk_print_job_send(_SELF(self), complete_func, (gpointer)block,
                       remove_callback_reference);
    return self;
}
#endif

void
Init_gtk_print_job(VALUE mGtk)
{
#ifdef HAVE_GTK_UNIX_PRINT
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PRINT_JOB, "PrintJob", mGtk);
    G_DEF_CLASS(GTK_TYPE_PRINT_CAPABILITIES, "PrintCapabilities", mGtk);

    RG_DEF_METHOD(initialize, 4);
    RG_DEF_METHOD(status, 0);
    RG_DEF_METHOD(set_source_file, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "source_file");
#ifdef HAVE_RB_CAIRO_H
    RG_DEF_METHOD(surface, 0);
#endif
    RG_DEF_METHOD(send, 0);
#endif
}
