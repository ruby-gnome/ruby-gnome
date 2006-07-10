/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprinter.c -

  $Author: ktou $
  $Date: 2006/07/10 13:17:56 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#ifdef HAVE_GTK_GTKPRINTER_H
#include <gtk/gtkprinter.h>

#ifndef GTK_TYPE_PRINT_BACKEND
#define GTK_TYPE_PRINT_BACKEND  (gtk_print_backend_get_type ())
#define GTK_PRINT_BACKEND(obj)  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRINT_BACKEND, GtkPrintBackend))
GType gtk_print_backend_get_type (void) G_GNUC_CONST;
#endif

#define _SELF(s) (GTK_PRINTER(RVAL2GOBJ(s)))

#define RVAL2PB(o) (GTK_PRINT_BACKEND(RVAL2GOBJ(o)))

static VALUE gPrinter;

static VALUE
p_initialize(VALUE self, VALUE name, VALUE backend, VALUE rb_virtual)
{
    GtkPrinter *printer;

    printer = gtk_printer_new(RVAL2CSTR(name), RVAL2PB(backend),
                              RVAL2CBOOL(rb_virtual));

    G_INITIALIZE(self, printer);
    return Qnil;
}

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
    if (RTEST(rb_equal(rb_obj_class(self), rb_obj_class(other)))) {
        return INT2NUM(gtk_printer_compare(_SELF(self), _SELF(other)));
    } else {
        return INT2NUM(G_MAXINT);
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
    return NIL_P(ruby_errinfo) ? TRUE : RVAL2CBOOL(result);
}

static void
remove_callback_reference(gpointer data)
{
    VALUE callback = (VALUE)data;
    G_CHILD_REMOVE(gPrinter, callback);
}

static VALUE
p_s_enumerate_printers(int argc, VALUE *argv, VALUE self)
{
    VALUE wait, block;
    rb_scan_args(argc, argv, "01", &wait);

    block = G_BLOCK_PROC();
    G_CHILD_ADD(gPrinter, block);
    gtk_enumerate_printers(each_printer, (gpointer)block,
                           remove_callback_reference, RVAL2CBOOL(wait));

    return Qnil;
}
#endif

void
Init_gtk_printer()
{
#ifdef HAVE_GTK_GTKPRINTER_H
    gPrinter = G_DEF_CLASS(GTK_TYPE_PRINTER, "Printer", mGtk);
    G_DEF_CLASS(GTK_TYPE_PRINT_BACKEND, "PrintBackend", mGtk);

    rb_define_singleton_method(gPrinter, "each", p_s_enumerate_printers, -1);

    rb_define_method(gPrinter, "initialize", p_initialize, 3);
    rb_define_method(gPrinter, "description", p_get_description, 0);
    rb_define_method(gPrinter, "active?", p_is_active, 0);
    rb_define_method(gPrinter, "default?", p_is_default, 0);
    rb_define_method(gPrinter, "<=>", p_compare, 1);

    G_DEF_SETTERS(gPrinter);
#endif
}
