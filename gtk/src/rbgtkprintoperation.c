/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprintoperation.c -

  $Author: ktou $
  $Date: 2006/07/09 06:12:41 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)
#define _SELF(s) (GTK_PRINT_OPERATION(RVAL2GOBJ(s)))
#define RVAL2POA(o) (RVAL2GENUM(o, GTK_TYPE_PRINT_OPERATION_ACTION))
#define POR2RVAL(o) (GENUM2RVAL(o, GTK_TYPE_PRINT_OPERATION_RESULT))

static VALUE gPrint;

static VALUE
po_initialize(VALUE self)
{
    GtkPrintOperation *op;

    op = gtk_print_operation_new();

    G_INITIALIZE(self, op);
    return Qnil;
}

static VALUE
po_run(int argc, VALUE *argv, VALUE self)
{
    VALUE action, parent, rb_result;
    GtkPrintOperationResult result;
    GError *error = NULL;

    rb_scan_args(argc, argv, "11", &action, &parent);

    result = gtk_print_operation_run(_SELF(self), RVAL2POA(action),
                                     RVAL2GOBJ(parent), &error);
    if (result == GTK_PRINT_OPERATION_RESULT_ERROR)
        RAISE_GERROR(error);

    rb_result = POR2RVAL(result);
    if (rb_block_given_p()) {
        return rb_yield(rb_result);
    } else {
        return rb_result;
    }
}

static VALUE
po_get_error(VALUE self)
{
    GError *error = NULL;
    gtk_print_operation_get_error(_SELF(self), &error);
    return error ? rbgerr_gerror2exception(error) : Qnil;
}

static VALUE
po_is_finished(VALUE self)
{
    return CBOOL2RVAL(gtk_print_operation_is_finished(_SELF(self)));
}

static VALUE
po_cancel(VALUE self)
{
    gtk_print_operation_cancel(_SELF(self));
    return Qnil;
}

struct callback_arg
{
    VALUE callback;
    int argc;
    VALUE *argv;
};

static VALUE
invoke_callback(VALUE arg_)
{
    struct callback_arg *arg = (struct callback_arg *)arg_;
    return rb_funcall2(arg->callback, id_call, arg->argc, arg->argv);
}

static void
page_setup_done_cb(GtkPageSetup *page_setup, gpointer data)
{
    VALUE callback = (VALUE)data;
    VALUE argv[1];
    struct callback_arg arg;

    G_CHILD_REMOVE(gPrint, callback);

    argv[0] = GOBJ2RVAL(page_setup);

    arg.callback = callback;
    arg.argc = sizeof(VALUE) / sizeof(argv);
    arg.argv = argv;
    G_PROTECT_CALLBACK(invoke_callback, &arg);
}

static VALUE
p_run_page_setup_dialog(int argc, VALUE *argv, VALUE self)
{
    VALUE parent, page_setup, settings;
    rb_scan_args(argc, argv, "12", &settings, &page_setup, &parent);

    if (rb_block_given_p()) {
        volatile VALUE func = G_BLOCK_PROC();
        G_CHILD_ADD(gPrint, func);
        gtk_print_run_page_setup_dialog_async(RVAL2GOBJ(parent),
                                              RVAL2GOBJ(settings),
                                              RVAL2GOBJ(page_setup),
                                              page_setup_done_cb,
                                              (gpointer)func);
        return Qnil;
    } else {
        VALUE rb_result_setup;
        GtkPageSetup *result_setup;
        result_setup = gtk_print_run_page_setup_dialog(RVAL2GOBJ(parent),
                                                       RVAL2GOBJ(settings),
                                                       RVAL2GOBJ(page_setup));
        rb_result_setup = GOBJ2RVAL(result_setup);
        g_object_unref(result_setup);
        return rb_result_setup;
    }
}
#endif

void
Init_gtk_print_operation()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE gPrintOperation = G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION,
                                        "PrintOperation", mGtk);
    gPrint = rb_define_module_under(mGtk, "Print");
    G_DEF_CLASS(GTK_TYPE_PRINT_STATUS, "PrintStatus", mGtk);
    G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION_ACTION,"PrintOperationAction", mGtk);
    G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION_RESULT, "PrintOperationResult", mGtk);

    rb_define_method(gPrintOperation, "initialize", po_initialize, 0);
    rb_define_method(gPrintOperation, "run", po_run, -1);
    rb_define_method(gPrintOperation, "error", po_get_error, 0);
    rb_define_method(gPrintOperation, "finished?", po_is_finished, 0);
    rb_define_method(gPrintOperation, "cancel", po_cancel, 0);

    G_DEF_SETTERS(gPrintOperation);

    rb_define_module_function(gPrint, "run_page_setup_dialog",
                              p_run_page_setup_dialog, -1);
#endif
}
