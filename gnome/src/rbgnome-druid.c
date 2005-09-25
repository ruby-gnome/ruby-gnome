/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-druid.c,v 1.4 2005/09/25 17:53:07 mutoh Exp $ */

/* Gnome::Druid widget for Ruby/GNOME2
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

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

/* Defined as a property.
void        gnome_druid_set_show_finish     (GnomeDruid *druid,
                                             gboolean show_finish);
void        gnome_druid_set_show_help       (GnomeDruid *druid,
                                             gboolean show_help);
*/

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

/* Don't need this.
void        gnome_druid_construct_with_window
                                            (GnomeDruid *druid,
                                             const char *title,
                                             GtkWindow *parent,
                                             gboolean close_on_cancel,
                                             GtkWidget **window);
*/

void
Init_gnome_druid(mGnome)
    VALUE mGnome;
{
    VALUE gnoDruid = G_DEF_CLASS(GNOME_TYPE_DRUID, "Druid", mGnome);

    rb_define_method(gnoDruid, "initialize", druid_initialize, 0);
    rb_define_method(gnoDruid, "set_buttons_sensitive", druid_set_buttons_sensitive, 4);
    rb_define_method(gnoDruid, "prepend_page", druid_prepend_page, 1);
    rb_define_method(gnoDruid, "insert_page", druid_insert_page, 2);
    rb_define_method(gnoDruid, "append_page", druid_append_page, 1);
    rb_define_method(gnoDruid, "set_page", druid_set_page, 1);
    
    G_DEF_SETTERS(gnoDruid);
    rb_define_singleton_method(gnoDruid, "new_with_window", druid_s_new_with_window, 3);
}
