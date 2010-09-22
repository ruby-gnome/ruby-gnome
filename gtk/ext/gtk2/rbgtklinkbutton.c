/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtklinkbutton.c -

  $Author: sakai $
  $Date: 2007/07/08 03:00:49 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(self) (GTK_LINK_BUTTON(RVAL2GOBJ(self)))

static VALUE
lb_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE uri, label;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "11", &uri, &label);
    if (NIL_P(label)){
        widget = gtk_link_button_new(RVAL2CSTR(uri));
    } else {
        widget = gtk_link_button_new_with_label(RVAL2CSTR(uri), RVAL2CSTR(label));
    }
    RBGTK_INITIALIZE(self, widget); 
    return Qnil;
}

static void
link_func(button, link, func)
    GtkLinkButton* button;
    const gchar* link;
    gpointer func;
{
    rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(button), CSTR2RVAL(link));
}

static VALUE
lb_set_uri_hook(self)
    VALUE self;
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_link_button_set_uri_hook((GtkLinkButtonUriFunc)link_func, (gpointer)func, (GDestroyNotify)NULL);
    return self;
}
#endif 

void 
Init_gtk_link_button()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE button = G_DEF_CLASS(GTK_TYPE_LINK_BUTTON, "LinkButton", mGtk);

    rb_define_method(button, "initialize", lb_initialize, -1);
    rb_define_singleton_method(button, "set_uri_hook", lb_set_uri_hook, 0);

    G_DEF_SETTERS(button);
#endif
}
