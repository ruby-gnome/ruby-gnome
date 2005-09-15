/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkdialog.c -

  $Author: ggc $
  $Date: 2005/09/15 18:23:00 $

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_DIALOG(RVAL2GOBJ(self)))

static ID id_to_a;

static VALUE
dialog_add_button(self, button_text, response_id)
    VALUE self, button_text, response_id;
{
    gchar* name;
    if (SYMBOL_P(button_text)) {
        name = rb_id2name(SYM2ID(button_text));
    } else {
        name = RVAL2CSTR(button_text);
    }
    return GOBJ2RVAL(gtk_dialog_add_button(_SELF(self), name, 
                                           RVAL2GENUM(response_id, GTK_TYPE_RESPONSE_TYPE)));
}

VALUE
rbgtk_dialog_add_buttons_internal(self, button_ary)
    VALUE self, button_ary;
{
    int i;
    GObject* obj = RVAL2GOBJ(self);
    g_object_freeze_notify(obj);
    for (i = 0; i < RARRAY(button_ary)->len; i++) {
        Check_Type(RARRAY(RARRAY(button_ary)->ptr[i]), T_ARRAY);
        dialog_add_button(self, RARRAY(RARRAY(button_ary)->ptr[i])->ptr[0],
                          RARRAY(RARRAY(button_ary)->ptr[i])->ptr[1]);
    }
    g_object_thaw_notify(obj);
    return self;
}    

static VALUE
dialog_add_buttons(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE button_ary;
    rb_scan_args(argc, argv, "*", &button_ary);
    rbgtk_dialog_add_buttons_internal(self, button_ary);
    return self;
}    

static VALUE
dialog_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE title, parent, flags, button_ary;
    rb_scan_args(argc, argv, "03*", &title, &parent, &flags, &button_ary);

    if (argc == 0){
        RBGTK_INITIALIZE(self, gtk_dialog_new());
    } else if (argc > 0){
        GtkDialog* dialog = GTK_DIALOG(g_object_new(GTK_TYPE_DIALOG, NULL));
        GtkDialogFlags gflags = NIL_P(flags) ? 0 : RVAL2GFLAGS(flags, GTK_TYPE_DIALOG_FLAGS);
        if (! NIL_P(title))
            gtk_window_set_title(GTK_WINDOW(dialog), RVAL2CSTR(title));
        if (! NIL_P(parent))
            gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(RVAL2GOBJ(parent)));
        
        if (gflags & GTK_DIALOG_MODAL)
            gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
        if (gflags & GTK_DIALOG_DESTROY_WITH_PARENT)
            gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), TRUE);
        if (gflags & GTK_DIALOG_NO_SEPARATOR)
            gtk_dialog_set_has_separator(dialog, FALSE);

        RBGTK_INITIALIZE(self, dialog);
        if (! NIL_P(button_ary))
            rbgtk_dialog_add_buttons_internal(self, button_ary);
    } else {
        rb_raise(rb_eArgError, "invalid argument number");
    }
    return Qnil;
}

static VALUE
dialog_run(self)
    VALUE self;
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
dialog_response(self, response_id)
    VALUE self, response_id;
{
    gtk_dialog_response(_SELF(self), NUM2INT(response_id));
    return self;
}

static VALUE
dialog_add_action_widget(self, child, response_id)
    VALUE self, child, response_id;
{
    gtk_dialog_add_action_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)), 
                                 NUM2INT(response_id));
    return self;
}

static VALUE
dialog_set_default_response(self, response_id)
    VALUE self, response_id;
{
    gtk_dialog_set_default_response(_SELF(self), NUM2INT(response_id));
    return self;
}

static VALUE
dialog_set_response_sensitive(self, response_id, setting)
    VALUE self, response_id, setting;
{
    gtk_dialog_set_response_sensitive(_SELF(self), NUM2INT(response_id), RTEST(setting));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
dialog_s_alternative_dialog_button_order(self, screen)
    VALUE self, screen;
{
    gboolean ret = gtk_alternative_dialog_button_order(NIL_P(screen) ? NULL : 
                                                       GDK_SCREEN(RVAL2GOBJ(screen)));
    return CBOOL2RVAL(ret);
}

/* Use gtk_dialog_set_alternative_button_order_from_array() instead.
void        gtk_dialog_set_alternative_button_order
                                            (GtkDialog *dialog,
                                             gint first_response_id,
                                             ...);
*/

static VALUE
dialog_set_alternative_button_order(self, new_order)
    VALUE self, new_order;
{
    gint i;
    gint len = RARRAY(new_order)->len;
    gint* gnew_order = g_new(gint, len);

    for (i = 0; i < len; i++){
        gnew_order[i] = NUM2INT(RARRAY(new_order)->ptr[i]);
    }

    gtk_dialog_set_alternative_button_order_from_array(_SELF(self), len, gnew_order);
    g_free(gnew_order);
    return self;
}

#endif

static VALUE
dialog_vbox(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->vbox);
}

static VALUE
dialog_action_area(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->action_area);
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
dialog_get_response_for_widget(self, widget)
    VALUE self, widget;
{
    return INT2NUM(gtk_dialog_get_response_for_widget(_SELF(self), RVAL2GOBJ(widget)));
}
#endif

void 
Init_gtk_dialog()
{
    VALUE gDialog = G_DEF_CLASS(GTK_TYPE_DIALOG, "Dialog", mGtk);

    id_to_a = rb_intern("to_a");

    rb_define_method(gDialog, "initialize", dialog_initialize, -1);
    rb_define_method(gDialog, "run", dialog_run, 0);
    rb_define_method(gDialog, "response", dialog_response, 1);
    rb_define_method(gDialog, "add_button", dialog_add_button, 2);
    rb_define_method(gDialog, "add_buttons", dialog_add_buttons, -1);
    rb_define_method(gDialog, "add_action_widget", dialog_add_action_widget, 2);
    rb_define_method(gDialog, "set_default_response", dialog_set_default_response, 1);

#if GTK_CHECK_VERSION(2,6,0)
    rb_define_singleton_method(gDialog, "alternative_dialog_button_order?", 
                               dialog_s_alternative_dialog_button_order, 1);
    rb_define_method(gDialog, "set_alternative_button_order", dialog_set_alternative_button_order, 1);
#endif

    rb_define_method(gDialog, "set_response_sensitive", dialog_set_response_sensitive, 2);
    rb_define_method(gDialog, "vbox", dialog_vbox, 0);
    rb_define_method(gDialog, "action_area", dialog_action_area, 0);

#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gDialog, "get_response_for_widget", dialog_get_response_for_widget, 1);
#endif
   
    G_DEF_SETTERS(gDialog);
   
    /* GtkDialogFlags */
    G_DEF_CLASS(GTK_TYPE_DIALOG_FLAGS, "Flags", gDialog);
    G_DEF_CONSTANTS(gDialog, GTK_TYPE_DIALOG_FLAGS, "GTK_DIALOG_");

    /* GtkResponseType */
    G_DEF_CLASS(GTK_TYPE_RESPONSE_TYPE, "ResponseType", gDialog);
    G_DEF_CONSTANTS(gDialog, GTK_TYPE_RESPONSE_TYPE, "GTK_");
}
