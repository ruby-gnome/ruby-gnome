/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprinter.c -

  $Author: sakai $
  $Date: 2007/07/08 03:00:49 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#ifdef HAVE_GTK_UNIX_PRINT
#include <gtk/gtkprinter.h>

#ifndef GTK_TYPE_PRINT_BACKEND
#define GTK_TYPE_PRINT_BACKEND  (gtk_print_backend_get_type ())
#define GTK_PRINT_BACKEND(obj)  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRINT_BACKEND, GtkPrintBackend))
GType gtk_print_backend_get_type (void) G_GNUC_CONST;
#endif

#define _SELF(s) (GTK_PRINTER(RVAL2GOBJ(s)))

static VALUE
p_initialize(VALUE self, VALUE name, VALUE backend, VALUE rb_virtual)
{
    GtkPrinter *printer;

    printer = gtk_printer_new(RVAL2CSTR(name),
                              GTK_PRINT_BACKEND(RVAL2GOBJ(backend)),
                              RVAL2CBOOL(rb_virtual));

    G_INITIALIZE(self, printer);
    return Qnil;
}

/* Defined as properties
gboolean    gtk_printer_accepts_ps          (GtkPrinter *printer);
gboolean    gtk_printer_accepts_pdf         (GtkPrinter *printer);
GtkPrintBackend* gtk_printer_get_backend    (GtkPrinter *printer);
const gchar* gtk_printer_get_icon_name      (GtkPrinter *printer);
gboolean    gtk_printer_is_virtual          (GtkPrinter *printer);
const gchar* gtk_printer_get_name           (GtkPrinter *printer);
const gchar* gtk_printer_get_state_message  (GtkPrinter *printer);
const gchar* gtk_printer_get_location       (GtkPrinter *printer);
gint        gtk_printer_get_job_count       (GtkPrinter *printer);
*/

static VALUE
p_get_description(VALUE self)
{
    return CSTR2RVAL(gtk_printer_get_description(_SELF(self)));
}

static VALUE
p_is_active(VALUE self)
{
    return CBOOL2RVAL(gtk_printer_is_default(_SELF(self)));
}

static VALUE
p_is_default(VALUE self)
{
    return CBOOL2RVAL(gtk_printer_is_default(_SELF(self)));
}

static VALUE
p_compare(VALUE self, VALUE other)
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
p_s_enumerate_printers(int argc, VALUE *argv, VALUE self)
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
Init_gtk_printer()
{
#ifdef HAVE_GTK_UNIX_PRINT
    VALUE gPrinter = G_DEF_CLASS(GTK_TYPE_PRINTER, "Printer", mGtk);
    rb_include_module(gPrinter, rb_mComparable);

    G_DEF_CLASS(GTK_TYPE_PRINT_BACKEND, "PrintBackend", mGtk);

    rb_define_singleton_method(gPrinter, "each", p_s_enumerate_printers, -1);

    rb_define_method(gPrinter, "initialize", p_initialize, 3);
    rb_define_method(gPrinter, "description", p_get_description, 0);
    rb_define_method(gPrinter, "active?", p_is_active, 0);
    rb_define_method(gPrinter, "default?", p_is_default, 0);
    rb_define_method(gPrinter, "<=>", p_compare, 1);

    G_DEF_CLASS3("GtkPrintBackendCups", "PrintBackendCups", mGtk);
    G_DEF_CLASS3("GtkPrintBackendFile", "PrintBackendFile", mGtk);
    G_DEF_CLASS3("GtkPrintBackendLpr", "PrintBackendLpr", mGtk);

    G_DEF_SETTERS(gPrinter);
#endif
}
