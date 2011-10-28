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
ass_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_assistant_new());
    return Qnil;
}

static VALUE
ass_get_current_page(VALUE self)
{
    return INT2NUM(gtk_assistant_get_current_page(_SELF(self)));
}

static VALUE
ass_set_current_page(VALUE self, VALUE page_num)
{
    gtk_assistant_set_current_page(_SELF(self), NUM2INT(page_num));
    return self;
}

static VALUE
ass_get_n_pages(VALUE self)
{
    return INT2NUM(gtk_assistant_get_n_pages(_SELF(self)));
}

static VALUE
ass_get_nth_page(VALUE self, VALUE page_num)
{
    return GOBJ2RVAL(gtk_assistant_get_nth_page(_SELF(self), NUM2INT(page_num)));
}

static VALUE
ass_prepend_page(VALUE self, VALUE page)
{
    return INT2NUM(gtk_assistant_prepend_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_append_page(VALUE self, VALUE page)
{
    return INT2NUM(gtk_assistant_append_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_insert_page(VALUE self, VALUE page, VALUE position)
{
    return INT2NUM(gtk_assistant_insert_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), NUM2INT(position)));
}

static gint
ass_page_func(gint current_page, gpointer func)
{
    return NUM2INT(rb_funcall((VALUE)func, id_call, 1, INT2NUM(current_page)));
}

static VALUE
ass_set_forward_page_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_assistant_set_forward_page_func(_SELF(self), (GtkAssistantPageFunc)ass_page_func, (gpointer)func, NULL);
    return self;
}

static VALUE
ass_set_page_type(VALUE self, VALUE page, VALUE type)
{
    gtk_assistant_set_page_type(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2GENUM(type, GTK_TYPE_ASSISTANT_PAGE_TYPE));
    return self;
}

static VALUE
ass_get_page_type(VALUE self, VALUE page)
{
    return GENUM2RVAL(gtk_assistant_get_page_type(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))), GTK_TYPE_ASSISTANT_PAGE_TYPE);
}

static VALUE
ass_set_page_title(VALUE self, VALUE page, VALUE title)
{
    gtk_assistant_set_page_title(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2CSTR(title));
    return self;
}

static VALUE
ass_get_page_title(VALUE self, VALUE page)
{
    return CSTR2RVAL(gtk_assistant_get_page_title(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_set_page_header_image(VALUE self, VALUE page, VALUE header_image)
{
    gtk_assistant_set_page_header_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), GDK_PIXBUF(RVAL2GOBJ(header_image)));
    return self;
}

static VALUE
ass_get_page_header_image(VALUE self, VALUE page)
{
    return GOBJ2RVAL(gtk_assistant_get_page_header_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_set_page_side_image(VALUE self, VALUE page, VALUE side_image)
{
    gtk_assistant_set_page_side_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), GDK_PIXBUF(RVAL2GOBJ(side_image)));
    return self;
}

static VALUE
ass_get_page_side_image(VALUE self, VALUE page)
{
    return GOBJ2RVAL(gtk_assistant_get_page_side_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_set_page_complete(VALUE self, VALUE page, VALUE complete)
{
    gtk_assistant_set_page_complete(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2CBOOL(complete));
    return self;
}

static VALUE
ass_get_page_complete(VALUE self, VALUE page)
{
    return CBOOL2RVAL(gtk_assistant_get_page_complete(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_add_action_widget(VALUE self, VALUE child)
{
    gtk_assistant_add_action_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
ass_remove_action_widget(VALUE self, VALUE child)
{
    gtk_assistant_remove_action_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
ass_update_buttons_state(VALUE self)
{
    gtk_assistant_update_buttons_state(_SELF(self));
    return self;
}

static VALUE
ass_commit(VALUE self)
{
    gtk_assistant_commit(_SELF(self));

    return self;
}

#endif

void
Init_gtk_assistant(void)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ASSISTANT, "Assistant", mGtk);
    rb_define_method(RG_TARGET_NAMESPACE, "initialize", ass_initialize, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "current_page", ass_get_current_page, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_current_page", ass_set_current_page, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "n_pages", ass_get_n_pages, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "get_nth_page", ass_get_nth_page, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "prepend_page", ass_prepend_page, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "append_page", ass_append_page, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "insert_page", ass_insert_page, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_forward_page_func", ass_set_forward_page_func, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_page_type", ass_set_page_type, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_page_type", ass_get_page_type, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_page_title", ass_set_page_title, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_page_title", ass_get_page_title, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_page_header_image", ass_set_page_header_image, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_page_header_image", ass_get_page_header_image, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_page_side_image", ass_set_page_side_image, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_page_side_image", ass_get_page_side_image, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_page_complete", ass_set_page_complete, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_page_complete", ass_get_page_complete, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "add_action_widget", ass_add_action_widget, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "remove_action_widget", ass_remove_action_widget, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "update_buttons_state", ass_update_buttons_state, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "commit", ass_commit, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkAssistantPageType */
    G_DEF_CLASS(GTK_TYPE_ASSISTANT_PAGE_TYPE, "PageType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_ASSISTANT_PAGE_TYPE, "GTK_ASSISTANT_");
#endif
}
