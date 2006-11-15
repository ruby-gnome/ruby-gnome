/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprintoperation.c -

  $Author: mutoh $
  $Date: 2006/11/15 23:46:13 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)
#define _SELF(s) (GTK_PRINT_OPERATION(RVAL2GOBJ(s)))

static VALUE gPrintOperation;

static VALUE
po_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_print_operation_new());
    return Qnil;
}

static VALUE
po_run(int argc, VALUE *argv, VALUE self)
{
    VALUE action, parent, rb_result;
    GtkPrintOperationResult result;
    GError *error = NULL;

    rb_scan_args(argc, argv, "11", &action, &parent);

    result = gtk_print_operation_run(_SELF(self), 
                                     RVAL2GENUM(action, GTK_TYPE_PRINT_OPERATION_ACTION),
                                     RVAL2GOBJ(parent), &error);

    rb_result = GENUM2RVAL(result, GTK_TYPE_PRINT_OPERATION_RESULT);
    if (rb_block_given_p()) {
        return rb_yield(rb_result);
    } else {
        if (result == GTK_PRINT_OPERATION_RESULT_ERROR)
            RAISE_GERROR(error);
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
    return self;
}

struct callback_arg
{
    VALUE callback;
    VALUE page_setup;
};

static VALUE
invoke_callback(VALUE arg_)
{
    struct callback_arg *arg = (struct callback_arg *)arg_;
    return rb_funcall(arg->callback, id_call, 1, arg->page_setup);
}

static void
page_setup_done_cb(GtkPageSetup *page_setup, gpointer data)
{
    VALUE callback = (VALUE)data;
    struct callback_arg arg;

    G_CHILD_UNSET(gPrintOperation, rb_intern("setup_done_cb"));

    arg.callback = callback;
    arg.page_setup = GOBJ2RVAL(page_setup);

    G_PROTECT_CALLBACK(invoke_callback, &arg);
}

static VALUE
po_run_page_setup_dialog(int argc, VALUE *argv, VALUE self)
{
    VALUE parent, page_setup, settings;
    rb_scan_args(argc, argv, "03", &parent, &page_setup, &settings);

    if (rb_block_given_p()) {
        volatile VALUE func = G_BLOCK_PROC();
        G_CHILD_SET(gPrintOperation, rb_intern("setup_done_cb"), func);
        gtk_print_run_page_setup_dialog_async(RVAL2GOBJ(parent),
                                              RVAL2GOBJ(page_setup),
                                              RVAL2GOBJ(settings),
                                              page_setup_done_cb,
                                              (gpointer)func);
        return Qnil;
    } else {
        return GOBJ2RVALU(gtk_print_run_page_setup_dialog(RVAL2GOBJ(parent),
                                                          RVAL2GOBJ(page_setup),
                                                          RVAL2GOBJ(settings)));
    }
}
#endif

void
Init_gtk_print_operation()
{
#if GTK_CHECK_VERSION(2,10,0)
    gPrintOperation = G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION, "PrintOperation", mGtk);

    G_DEF_ERROR(GTK_PRINT_ERROR, "PrintError", mGtk, rb_eRuntimeError,
                GTK_TYPE_PRINT_ERROR);

    rb_define_method(gPrintOperation, "initialize", po_initialize, 0);
    rb_define_method(gPrintOperation, "run", po_run, -1);
    rb_define_method(gPrintOperation, "error", po_get_error, 0);
    rb_define_method(gPrintOperation, "finished?", po_is_finished, 0);
    rb_define_method(gPrintOperation, "cancel", po_cancel, 0);

    rb_define_singleton_method(gPrintOperation, "run_page_setup_dialog",
                               po_run_page_setup_dialog, -1);

    /* GtkPrintStatus */
    G_DEF_CLASS(GTK_TYPE_PRINT_STATUS, "Status", gPrintOperation);
    G_DEF_CONSTANTS(gPrintOperation, GTK_TYPE_PRINT_STATUS, "GTK_PRINT_");

    /* GtkPrintOperationAction */
    G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION_ACTION,"Action", gPrintOperation);
    G_DEF_CONSTANTS(gPrintOperation, GTK_TYPE_PRINT_OPERATION_ACTION, "GTK_PRINT_OPERATION_");

    /* GtkPrintOperationResult */
    G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION_RESULT, "Result", gPrintOperation);
    G_DEF_CONSTANTS(gPrintOperation, GTK_TYPE_PRINT_OPERATION_RESULT, "GTK_PRINT_OPERATION_");

    G_DEF_SETTERS(gPrintOperation);
#endif
}
