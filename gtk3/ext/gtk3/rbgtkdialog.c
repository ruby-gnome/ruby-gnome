/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

static ID id_to_a;

static VALUE
rg_add_button(VALUE self, VALUE button_text, VALUE response_id)
{
    const gchar *name;
    if (SYMBOL_P(button_text)) {
        name = rb_id2name(SYM2ID(button_text));
    } else {
        name = RVAL2CSTR(button_text);
    }
    return GOBJ2RVAL(gtk_dialog_add_button(_SELF(self), name, 
                                           RVAL2GTKRESPONSETYPE(response_id)));
}

struct rbgtk_dialog_add_buttons_internal_args {
    VALUE self;
    VALUE buttons;
};

static VALUE
rbgtk_dialog_add_buttons_internal_body(VALUE value)
{
    struct rbgtk_dialog_add_buttons_internal_args *args = (struct rbgtk_dialog_add_buttons_internal_args *)value;
    long i;
    long n = RARRAY_LEN(args->buttons);

    for (i = 0; i < n; i++) {
        VALUE button = rb_ary_to_ary(RARRAY_PTR(args->buttons)[i]);

        rg_add_button(args->self, RARRAY_PTR(button)[0], RARRAY_PTR(button)[1]);
    }

    return args->self;
}

static VALUE
rbgtk_dialog_add_buttons_internal_ensure(VALUE value)
{
    g_object_thaw_notify(RVAL2GOBJ(((struct rbgtk_dialog_add_buttons_internal_args *)value)->self));

    return Qnil;
}

VALUE
rbgtk_dialog_add_buttons_internal(VALUE self, VALUE buttons)
{
    struct rbgtk_dialog_add_buttons_internal_args args = { self, buttons };

    if (NIL_P(RARRAY_PTR(buttons)[0]))
        return self;

    g_object_freeze_notify(RVAL2GOBJ(self));

    return rb_ensure(rbgtk_dialog_add_buttons_internal_body, (VALUE)&args,
                     rbgtk_dialog_add_buttons_internal_ensure, (VALUE)&args);
}    

static VALUE
rg_add_buttons(int argc, VALUE *argv, VALUE self)
{
    VALUE button_ary;
    rb_scan_args(argc, argv, "*", &button_ary);
    rbgtk_dialog_add_buttons_internal(self, button_ary);
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
        rbgtk_dialog_add_buttons_internal(self, rb_button_ary);

    return Qnil;
}

static VALUE
rg_run(VALUE self)
{
    if (rb_block_given_p()){
        VALUE ret = INT2NUM(gtk_dialog_run(_SELF(self)));
        rb_yield(ret);
        return ret;
    } else {
        return INT2NUM(gtk_dialog_run(_SELF(self)));
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

    id_to_a = rb_intern("to_a");

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(run, 0);
    RG_DEF_METHOD(response, 1);
    RG_DEF_METHOD(add_button, 2);
    RG_DEF_METHOD(add_buttons, -1);
    RG_DEF_METHOD(add_action_widget, 2);
    RG_DEF_METHOD(set_default_response, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "default_response");
    RG_DEF_SMETHOD_P(alternative_dialog_button_order, 1);
    RG_DEF_METHOD(set_alternative_button_order, 1);
    RG_DEF_METHOD(set_response_sensitive, 2);
    RG_DEF_METHOD(action_area, 0);
    RG_DEF_METHOD(get_response, 1);
    RG_DEF_ALIAS("get_response_for_widget", "get_response");
    RG_DEF_METHOD(content_area, 0);
    RG_DEF_METHOD(get_widget_for_response, 1);

    G_DEF_CLASS(GTK_TYPE_DIALOG_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GTK_TYPE_RESPONSE_TYPE, "ResponseType", RG_TARGET_NAMESPACE);
}
