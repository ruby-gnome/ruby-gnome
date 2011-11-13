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

#define RG_TARGET_NAMESPACE cAssistant
#define _SELF(s) (GTK_ASSISTANT(RVAL2GOBJ(s)))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_assistant_new());
    return Qnil;
}

static VALUE
rg_current_page(VALUE self)
{
    return INT2NUM(gtk_assistant_get_current_page(_SELF(self)));
}

static VALUE
rg_set_current_page(VALUE self, VALUE page_num)
{
    gtk_assistant_set_current_page(_SELF(self), NUM2INT(page_num));
    return self;
}

static VALUE
rg_n_pages(VALUE self)
{
    return INT2NUM(gtk_assistant_get_n_pages(_SELF(self)));
}

static VALUE
rg_get_nth_page(VALUE self, VALUE page_num)
{
    return GOBJ2RVAL(gtk_assistant_get_nth_page(_SELF(self), NUM2INT(page_num)));
}

static VALUE
rg_prepend_page(VALUE self, VALUE page)
{
    return INT2NUM(gtk_assistant_prepend_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
rg_append_page(VALUE self, VALUE page)
{
    return INT2NUM(gtk_assistant_append_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
rg_insert_page(VALUE self, VALUE page, VALUE position)
{
    return INT2NUM(gtk_assistant_insert_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), NUM2INT(position)));
}

static gint
ass_page_func(gint current_page, gpointer func)
{
    return NUM2INT(rb_funcall((VALUE)func, id_call, 1, INT2NUM(current_page)));
}

static VALUE
rg_set_forward_page_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_assistant_set_forward_page_func(_SELF(self), (GtkAssistantPageFunc)ass_page_func, (gpointer)func, NULL);
    return self;
}

static VALUE
rg_set_page_type(VALUE self, VALUE page, VALUE type)
{
    gtk_assistant_set_page_type(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2GENUM(type, GTK_TYPE_ASSISTANT_PAGE_TYPE));
    return self;
}

static VALUE
rg_get_page_type(VALUE self, VALUE page)
{
    return GENUM2RVAL(gtk_assistant_get_page_type(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))), GTK_TYPE_ASSISTANT_PAGE_TYPE);
}

static VALUE
rg_set_page_title(VALUE self, VALUE page, VALUE title)
{
    gtk_assistant_set_page_title(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2CSTR(title));
    return self;
}

static VALUE
rg_get_page_title(VALUE self, VALUE page)
{
    return CSTR2RVAL(gtk_assistant_get_page_title(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
rg_set_page_header_image(VALUE self, VALUE page, VALUE header_image)
{
    gtk_assistant_set_page_header_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), GDK_PIXBUF(RVAL2GOBJ(header_image)));
    return self;
}

static VALUE
rg_get_page_header_image(VALUE self, VALUE page)
{
    return GOBJ2RVAL(gtk_assistant_get_page_header_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
rg_set_page_side_image(VALUE self, VALUE page, VALUE side_image)
{
    gtk_assistant_set_page_side_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), GDK_PIXBUF(RVAL2GOBJ(side_image)));
    return self;
}

static VALUE
rg_get_page_side_image(VALUE self, VALUE page)
{
    return GOBJ2RVAL(gtk_assistant_get_page_side_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
rg_set_page_complete(VALUE self, VALUE page, VALUE complete)
{
    gtk_assistant_set_page_complete(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2CBOOL(complete));
    return self;
}

static VALUE
rg_get_page_complete(VALUE self, VALUE page)
{
    return CBOOL2RVAL(gtk_assistant_get_page_complete(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
rg_add_action_widget(VALUE self, VALUE child)
{
    gtk_assistant_add_action_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
rg_remove_action_widget(VALUE self, VALUE child)
{
    gtk_assistant_remove_action_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
rg_update_buttons_state(VALUE self)
{
    gtk_assistant_update_buttons_state(_SELF(self));
    return self;
}

static VALUE
rg_commit(VALUE self)
{
    gtk_assistant_commit(_SELF(self));

    return self;
}

#endif

void
Init_gtk_assistant(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ASSISTANT, "Assistant", mGtk);
    RG_DEF_METHOD(initialize, 0);

    RG_DEF_METHOD(current_page, 0);
    RG_DEF_METHOD(set_current_page, 1);
    RG_DEF_METHOD(n_pages, 0);
    RG_DEF_METHOD(get_nth_page, 1);
    RG_DEF_METHOD(prepend_page, 1);
    RG_DEF_METHOD(append_page, 1);
    RG_DEF_METHOD(insert_page, 2);
    RG_DEF_METHOD(set_forward_page_func, 0);
    RG_DEF_METHOD(set_page_type, 2);
    RG_DEF_METHOD(get_page_type, 1);
    RG_DEF_METHOD(set_page_title, 2);
    RG_DEF_METHOD(get_page_title, 1);
    RG_DEF_METHOD(set_page_header_image, 2);
    RG_DEF_METHOD(get_page_header_image, 1);
    RG_DEF_METHOD(set_page_side_image, 2);
    RG_DEF_METHOD(get_page_side_image, 1);
    RG_DEF_METHOD(set_page_complete, 2);
    RG_DEF_METHOD(get_page_complete, 1);
    RG_DEF_METHOD(add_action_widget, 1);
    RG_DEF_METHOD(remove_action_widget, 1);
    RG_DEF_METHOD(update_buttons_state, 0);
    RG_DEF_METHOD(commit, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkAssistantPageType */
    G_DEF_CLASS(GTK_TYPE_ASSISTANT_PAGE_TYPE, "PageType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_ASSISTANT_PAGE_TYPE, "GTK_ASSISTANT_");
#endif
}
