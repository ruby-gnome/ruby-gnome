/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-druid.c,v 1.1 2002/10/20 07:33:51 tkubo Exp $ */

#include "rbgnome.h"

#define _SELF(self) GNOME_DRUID(RVAL2GOBJ(self))

static VALUE
druid_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gnome_druid_new());
    return Qnil;
}

static VALUE
druid_set_buttons_sensitive(self, back_sensitive, next_sensitive, cancel_sensitive, help_sensitive)
    VALUE self, back_sensitive, next_sensitive, cancel_sensitive, help_sensitive;
{
    gnome_druid_set_buttons_sensitive(_SELF(self),
                                      RTEST(back_sensitive),
                                      RTEST(next_sensitive),
                                      RTEST(cancel_sensitive),
                                      RTEST(help_sensitive));
    return self;
}

static VALUE
druid_set_show_finish(self, show_finish)
    VALUE self, show_finish;
{
    gnome_druid_set_show_finish(_SELF(self), RTEST(show_finish));
    return self;
}

static VALUE
druid_set_show_help(self, show_help)
    VALUE self, show_help;
{
    gnome_druid_set_show_help(_SELF(self), RTEST(show_help));
    return self;
}

static VALUE
druid_prepend_page(self, page)
    VALUE self, page;
{
    gnome_druid_prepend_page(_SELF(self),
                             GNOME_DRUID_PAGE(RVAL2GOBJ(page)));
    return self;
}

static VALUE
druid_insert_page(self, back_page, page)
    VALUE self, back_page, page;
{
    gnome_druid_insert_page(_SELF(self),
                            GNOME_DRUID_PAGE(RVAL2GOBJ(back_page)),
                            GNOME_DRUID_PAGE(RVAL2GOBJ(page)));
    return self;
}

static VALUE
druid_append_page(self, page)
    VALUE self, page;
{
    gnome_druid_append_page(_SELF(self),
                            GNOME_DRUID_PAGE(RVAL2GOBJ(page)));
    return self;
}

static VALUE
druid_set_page(self, page)
    VALUE self, page;
{
    gnome_druid_set_page(_SELF(self),
                         GNOME_DRUID_PAGE(RVAL2GOBJ(page)));
    return self;
}

static VALUE
druid_s_new_with_window(klass, title, parent, close_on_cancel)
    VALUE klass, title, parent, close_on_cancel;
{
    GtkWidget *druid, *window;

    druid = gnome_druid_new_with_window(NIL_P(title) ? NULL : RVAL2CSTR(title),
                                        NIL_P(parent) ? NULL : GTK_WINDOW(RVAL2GOBJ(parent)),
                                        RTEST(close_on_cancel),
                                        &window);
    if (druid == NULL)
        return Qnil;
    druid = gtk_widget_ref(druid);
    gtk_object_sink(GTK_OBJECT(druid));
    window = gtk_widget_ref(window);
    gtk_object_sink(GTK_OBJECT(window));
    return rb_ary_new3(2, GOBJ2RVAL(druid), GOBJ2RVAL(window));
}

void
Init_gnome_druid(mGnome)
    VALUE mGnome;
{
    VALUE gnoDruid = G_DEF_CLASS(GNOME_TYPE_DRUID, "Druid", mGnome);

    rb_define_method(gnoDruid, "initialize", druid_initialize, 0);
    rb_define_method(gnoDruid, "set_buttons_sensitive", druid_set_buttons_sensitive, 4);
    rb_define_method(gnoDruid, "set_show_finish", druid_set_show_finish, 1);
    rb_define_method(gnoDruid, "set_show_help", druid_set_show_help, 1);
    rb_define_method(gnoDruid, "prepend_page", druid_prepend_page, 1);
    rb_define_method(gnoDruid, "insert_page", druid_insert_page, 2);
    rb_define_method(gnoDruid, "append_page", druid_append_page, 1);
    rb_define_method(gnoDruid, "set_page", druid_set_page, 1);

    rb_define_singleton_method(gnoDruid, "new_with_window", druid_s_new_with_window, 3);
}
