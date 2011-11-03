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
#include <gtk/gtkprinter.h>

#ifndef GTK_TYPE_PRINT_BACKEND
#define GTK_TYPE_PRINT_BACKEND  (gtk_print_backend_get_type ())
#define GTK_PRINT_BACKEND(obj)  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRINT_BACKEND, GtkPrintBackend))
GType gtk_print_backend_get_type (void) G_GNUC_CONST;
#endif

#define RG_TARGET_NAMESPACE cPrinter
#define _SELF(s) (GTK_PRINTER(RVAL2GOBJ(s)))

static VALUE
rg_initialize(VALUE self, VALUE name, VALUE backend, VALUE rb_virtual)
{
    GtkPrinter *printer;

    printer = gtk_printer_new(RVAL2CSTR(name),
                              GTK_PRINT_BACKEND(RVAL2GOBJ(backend)),
                              RVAL2CBOOL(rb_virtual));

    G_INITIALIZE(self, printer);
    return Qnil;
}

static VALUE
rg_description(VALUE self)
{
    return CSTR2RVAL(gtk_printer_get_description(_SELF(self)));
}

static VALUE
rg_active_p(VALUE self)
{
    return CBOOL2RVAL(gtk_printer_is_default(_SELF(self)));
}

static VALUE
rg_default_p(VALUE self)
{
    return CBOOL2RVAL(gtk_printer_is_default(_SELF(self)));
}

static VALUE
rg_operator_p_compare(VALUE self, VALUE other)
{
    if (rb_obj_is_kind_of(other, GTYPE2CLASS(GTK_TYPE_PRINTER))){
        return INT2NUM(gtk_printer_compare(_SELF(self), _SELF(other)));
    } else {
        rb_raise(rb_eTypeError, "%s isn't a kind of Gtk::Printer", rb_class2name(other));
    }
}

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

static gboolean
each_printer(GtkPrinter *printer, gpointer data)
{
    VALUE result;
    VALUE argv[1];
    struct callback_arg arg;

    argv[0] = GOBJ2RVAL(printer);

    arg.callback = (VALUE)data;
    arg.argc = 1;
    arg.argv = argv;

    result = G_PROTECT_CALLBACK(invoke_callback, &arg);
    return NIL_P(rb_errinfo()) ? TRUE : RVAL2CBOOL(result);
}

static void
remove_callback_reference(gpointer data)
{
    VALUE callback = (VALUE)data;
    G_CHILD_REMOVE(mGtk, callback);
}

static VALUE
rg_s_each(int argc, VALUE *argv, VALUE self)
{
    VALUE wait, block;
    rb_scan_args(argc, argv, "01", &wait);

    block = rb_block_proc();
    G_CHILD_ADD(mGtk, block);
    gtk_enumerate_printers(each_printer, (gpointer)block,
                           remove_callback_reference, RVAL2CBOOL(wait));

    return self;
}
#endif

void
Init_gtk_printer(VALUE mGtk)
{
#ifdef HAVE_GTK_UNIX_PRINT
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PRINTER, "Printer", mGtk);
    rb_include_module(RG_TARGET_NAMESPACE, rb_mComparable);

    G_DEF_CLASS(GTK_TYPE_PRINT_BACKEND, "PrintBackend", mGtk);

    RG_DEF_SMETHOD(each, -1);

    RG_DEF_METHOD(initialize, 3);
    RG_DEF_METHOD(description, 0);
    RG_DEF_METHOD_P(active, 0);
    RG_DEF_METHOD_P(default, 0);
    RG_DEF_METHOD_OPERATOR("<=>", p_compare, 1);

    G_DEF_CLASS3("GtkPrintBackendCups", "PrintBackendCups", mGtk);
    G_DEF_CLASS3("GtkPrintBackendFile", "PrintBackendFile", mGtk);
    G_DEF_CLASS3("GtkPrintBackendLpr", "PrintBackendLpr", mGtk);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
