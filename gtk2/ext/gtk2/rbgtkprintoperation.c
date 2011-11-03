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

#if GTK_CHECK_VERSION(2,10,0)

#define RG_TARGET_NAMESPACE cPrintOperation
#define _SELF(s) (GTK_PRINT_OPERATION(RVAL2GOBJ(s)))

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_print_operation_new());
    return Qnil;
}

static VALUE
rg_run(int argc, VALUE *argv, VALUE self)
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
rg_error(VALUE self)
{
    GError *error = NULL;
    gtk_print_operation_get_error(_SELF(self), &error);
    return error ? rbgerr_gerror2exception(error) : Qnil;
}

static VALUE
rg_finished_p(VALUE self)
{
    return CBOOL2RVAL(gtk_print_operation_is_finished(_SELF(self)));
}

static VALUE
rg_cancel(VALUE self)
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

    G_CHILD_UNSET(RG_TARGET_NAMESPACE, rb_intern("setup_done_cb"));

    arg.callback = callback;
    arg.page_setup = GOBJ2RVAL(page_setup);

    G_PROTECT_CALLBACK(invoke_callback, &arg);
}

static VALUE
rg_s_run_page_setup_dialog(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE parent, page_setup, settings;
    rb_scan_args(argc, argv, "03", &parent, &page_setup, &settings);

    if (rb_block_given_p()) {
        volatile VALUE func = rb_block_proc();
        G_CHILD_SET(RG_TARGET_NAMESPACE, rb_intern("setup_done_cb"), func);
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
Init_gtk_print_operation(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,10,0)
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION, "PrintOperation", mGtk);

    G_DEF_ERROR(GTK_PRINT_ERROR, "PrintError", mGtk, rb_eRuntimeError,
                GTK_TYPE_PRINT_ERROR);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(run, -1);
    RG_DEF_METHOD(error, 0);
    RG_DEF_METHOD_P(finished, 0);
    RG_DEF_METHOD(cancel, 0);

    RG_DEF_SMETHOD(run_page_setup_dialog, -1);

    /* GtkPrintStatus */
    G_DEF_CLASS(GTK_TYPE_PRINT_STATUS, "Status", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PRINT_STATUS, "GTK_PRINT_");

    /* GtkPrintOperationAction */
    G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION_ACTION,"Action", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PRINT_OPERATION_ACTION, "GTK_PRINT_OPERATION_");

    /* GtkPrintOperationResult */
    G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION_RESULT, "Result", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PRINT_OPERATION_RESULT, "GTK_PRINT_OPERATION_");
#endif
}
