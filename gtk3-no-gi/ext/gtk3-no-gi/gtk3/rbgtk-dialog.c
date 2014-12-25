/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2014  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cDialog
#define _SELF(self) (RVAL2GTKDIALOG(self))

static VALUE
rg_add_button(VALUE self, VALUE button_text, VALUE response_id)
{
    VALUE buffer;
    return GOBJ2RVAL(gtk_dialog_add_button(_SELF(self),
                                           SYMBOL_P(button_text) ? RVAL2GLIBID(button_text, buffer) : RVAL2CSTR(button_text),
                                           RVAL2GTKRESPONSETYPE(response_id)));
}

static VALUE
rg_add_buttons(int argc, VALUE *argv, VALUE self)
{
    VALUE button_ary;
    rb_scan_args(argc, argv, "*", &button_ary);
    rbgtk_add_buttons(self, button_ary, rg_add_button);
    return self;
}    

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, rb_title, rb_parent, rb_flags, rb_button_ary;
    const gchar *title;
    GtkWindow *parent;
    GtkDialogFlags flags;
    GtkWidget *dialog;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "title", &rb_title,
                     "parent", &rb_parent,
                     "flags", &rb_flags,
                     "buttons", &rb_button_ary,
                     NULL);
    title = RVAL2CSTR_ACCEPT_NIL(rb_title);
    parent = NIL_P(rb_parent) ? NULL : RVAL2GTKWINDOW(rb_parent);
    flags = NIL_P(rb_flags) ? 0 : RVAL2GTKDIALOGFLAGS(rb_flags);

    dialog = gtk_dialog_new_with_buttons(title, parent, flags, NULL, NULL);
    RBGTK_INITIALIZE(self, dialog);
    if (!NIL_P(rb_button_ary))
        rb_funcall2(self, rb_intern("add_buttons"),
                    RARRAY_LEN(rb_button_ary), RARRAY_PTR(rb_button_ary));

    return Qnil;
}

static gboolean
destroy_dialog(gpointer user_data)
{
    GtkWidget *dialog = user_data;
    gtk_widget_destroy(dialog);
    return G_SOURCE_REMOVE;
}

static VALUE
rg_run(VALUE self)
{
    GtkDialog *dialog;
    GSource *interrupt_source;
    VALUE response;

    dialog = _SELF(self);
    interrupt_source = rbg_interrupt_source_new();
    g_source_set_callback(interrupt_source, destroy_dialog, dialog, NULL);
    g_source_attach(interrupt_source, NULL);
    response = INT2NUM(gtk_dialog_run(dialog));
    g_source_destroy(interrupt_source);
    g_source_unref(interrupt_source);

    rb_thread_check_ints();

    if (rb_block_given_p()) {
        return rb_yield(response);
    } else {
        return response;
    }
}

static VALUE
rg_response(VALUE self, VALUE response_id)
{
    gtk_dialog_response(_SELF(self), NUM2INT(response_id));
    return self;
}

static VALUE
rg_add_action_widget(VALUE self, VALUE child, VALUE response_id)
{
    gtk_dialog_add_action_widget(_SELF(self), RVAL2GTKWIDGET(child), 
                                 NUM2INT(response_id));
    return self;
}

static VALUE
rg_set_default_response(VALUE self, VALUE response_id)
{
    gtk_dialog_set_default_response(_SELF(self), NUM2INT(response_id));
    return self;
}

static VALUE
rg_set_response_sensitive(VALUE self, VALUE response_id, VALUE setting)
{
    gtk_dialog_set_response_sensitive(_SELF(self), NUM2INT(response_id), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_s_alternative_dialog_button_order_p(G_GNUC_UNUSED VALUE self, VALUE screen)
{
    gboolean ret = gtk_alternative_dialog_button_order(NIL_P(screen) ? NULL : 
                                                       RVAL2GDKSCREEN(screen));
    return CBOOL2RVAL(ret);
}

static VALUE
rg_set_alternative_button_order(VALUE self, VALUE rbnew_order)
{
    GtkDialog *dialog = _SELF(self);
    long n;
    gint *new_order = RVAL2GINTS(rbnew_order, n);

    gtk_dialog_set_alternative_button_order_from_array(dialog, n, new_order);

    g_free(new_order);

    return self;
}

static VALUE
rg_action_area(VALUE self)
{
    return GOBJ2RVAL(gtk_dialog_get_action_area(_SELF(self)));
}

static VALUE
rg_get_response(VALUE self, VALUE widget)
{
    return INT2NUM(gtk_dialog_get_response_for_widget(_SELF(self), RVAL2GTKWIDGET(widget)));
}

static VALUE
rg_content_area(VALUE self)
{
    return GOBJ2RVAL(gtk_dialog_get_content_area(_SELF(self)));
}

static VALUE
rg_get_widget_for_response(VALUE self, VALUE response_id)
{
    return GOBJ2RVAL(gtk_dialog_get_widget_for_response(_SELF(self), NUM2INT(response_id)));
}

void
Init_gtk_dialog(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_DIALOG, "Dialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(run, 0);
    RG_DEF_METHOD(response, 1);
    RG_DEF_METHOD(add_button, 2);
    RG_DEF_METHOD(add_buttons, -1);
    RG_DEF_METHOD(add_action_widget, 2);
    RG_DEF_METHOD(set_default_response, 1);
    RG_DEF_SMETHOD_P(alternative_dialog_button_order, 1);
    RG_DEF_METHOD(set_alternative_button_order, 1);
    RG_DEF_METHOD(set_response_sensitive, 2);
    RG_DEF_METHOD(action_area, 0);
    RG_DEF_METHOD(get_response, 1);
    RG_DEF_ALIAS("get_response_for_widget", "get_response");
    RG_DEF_METHOD(content_area, 0);
    RG_DEF_METHOD(get_widget_for_response, 1);

    G_DEF_CLASS(GTK_TYPE_DIALOG_FLAGS, "Flags", RG_TARGET_NAMESPACE);
}
