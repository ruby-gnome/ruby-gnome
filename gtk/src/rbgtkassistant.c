/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkassistant.c -

  $Author: mutoh $
  $Date: 2007/01/29 16:31:27 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(s) (GTK_ASSISTANT(RVAL2GOBJ(s)))

static VALUE
ass_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_assistant_new());
    return Qnil;
}

static VALUE
ass_get_current_page(self)
    VALUE self;
{
    return INT2NUM(gtk_assistant_get_current_page(_SELF(self)));
}

static VALUE
ass_set_current_page(self, page_num)
    VALUE self, page_num;
{
    gtk_assistant_set_current_page(_SELF(self), NUM2INT(page_num));
    return self;
}

static VALUE
ass_get_n_pages(self)
    VALUE self;
{
    return INT2NUM(gtk_assistant_get_n_pages(_SELF(self)));
}

static VALUE
ass_get_nth_page(self, page_num)
    VALUE self, page_num;
{
    return GOBJ2RVAL(gtk_assistant_get_nth_page(_SELF(self), NUM2INT(page_num)));
}

static VALUE
ass_prepend_page(self, page)
    VALUE self, page;
{
    return INT2NUM(gtk_assistant_prepend_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_append_page(self, page)
    VALUE self, page;
{
    return INT2NUM(gtk_assistant_append_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_insert_page(self, page, position)
    VALUE self, page, position;
{
    return INT2NUM(gtk_assistant_insert_page(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), NUM2INT(position)));
}

static gint
ass_page_func(current_page, func)
    gint current_page;
    gpointer func;
{
    return NUM2INT(rb_funcall((VALUE)func, id_call, 1, INT2NUM(current_page)));
}

static VALUE
ass_set_forward_page_func(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_assistant_set_forward_page_func(_SELF(self), (GtkAssistantPageFunc)ass_page_func, (gpointer)func, NULL);
    return self;
}

static VALUE
ass_set_page_type(self, page, type)
    VALUE self, page, type;
{
    gtk_assistant_set_page_type(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2GENUM(type, GTK_TYPE_ASSISTANT_PAGE_TYPE));
    return self;
}

static VALUE
ass_get_page_type(self, page)
    VALUE self, page;
{
    return GENUM2RVAL(gtk_assistant_get_page_type(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))), GTK_TYPE_ASSISTANT_PAGE_TYPE);
}

static VALUE
ass_set_page_title(self, page, title)
    VALUE self, page, title;
{
    gtk_assistant_set_page_title(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2CSTR(title));
    return self;
}

static VALUE
ass_get_page_title(self, page)
    VALUE self, page;
{
    return CSTR2RVAL(gtk_assistant_get_page_title(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_set_page_header_image(self, page, header_image)
    VALUE self, page, header_image;
{
    gtk_assistant_set_page_header_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), GDK_PIXBUF(RVAL2GOBJ(header_image)));
    return self;
}

static VALUE
ass_get_page_header_image(self, page)
    VALUE self, page;
{
    return GOBJ2RVAL(gtk_assistant_get_page_header_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_set_page_side_image(self, page, side_image)
    VALUE self, page, side_image;
{
    gtk_assistant_set_page_side_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), GDK_PIXBUF(RVAL2GOBJ(side_image)));
    return self;
}

static VALUE
ass_get_page_side_image(self, page)
    VALUE self, page;
{
    return GOBJ2RVAL(gtk_assistant_get_page_side_image(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_set_page_complete(self, page, complete)
    VALUE self, page, complete;
{
    gtk_assistant_set_page_complete(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page)), RVAL2CBOOL(complete));
    return self;
}

static VALUE
ass_get_page_complete(self, page)
    VALUE self, page;
{
    return CBOOL2RVAL(gtk_assistant_get_page_complete(_SELF(self), GTK_WIDGET(RVAL2GOBJ(page))));
}

static VALUE
ass_add_action_widget(self, child)
    VALUE self, child;
{
    gtk_assistant_add_action_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
ass_remove_action_widget(self, child)
    VALUE self, child;
{
    gtk_assistant_remove_action_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
ass_update_buttons_state(self)
    VALUE self;
{
    gtk_assistant_update_buttons_state(_SELF(self));
    return self;
}

#endif

void 
Init_gtk_assistant()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE ass = G_DEF_CLASS(GTK_TYPE_ASSISTANT, "Assistant", mGtk);
    rb_define_method(ass, "initialize", ass_initialize, 0);

    rb_define_method(ass, "current_page", ass_get_current_page, 0);
    rb_define_method(ass, "set_current_page", ass_set_current_page, 1);
    rb_define_method(ass, "n_pages", ass_get_n_pages, 0);
    rb_define_method(ass, "get_nth_page", ass_get_nth_page, 1);
    rb_define_method(ass, "prepend_page", ass_prepend_page, 1);
    rb_define_method(ass, "append_page", ass_append_page, 1);
    rb_define_method(ass, "insert_page", ass_insert_page, 2);
    rb_define_method(ass, "set_forward_page_func", ass_set_forward_page_func, 0);
    rb_define_method(ass, "set_page_type", ass_set_page_type, 2);
    rb_define_method(ass, "get_page_type", ass_get_page_type, 1);
    rb_define_method(ass, "set_page_title", ass_set_page_title, 2);
    rb_define_method(ass, "get_page_title", ass_get_page_title, 1);
    rb_define_method(ass, "set_page_header_image", ass_set_page_header_image, 2);
    rb_define_method(ass, "get_page_header_image", ass_get_page_header_image, 1);
    rb_define_method(ass, "set_page_side_image", ass_set_page_side_image, 2);
    rb_define_method(ass, "get_page_side_image", ass_get_page_side_image, 1);
    rb_define_method(ass, "set_page_complete", ass_set_page_complete, 2);
    rb_define_method(ass, "get_page_complete", ass_get_page_complete, 1);
    rb_define_method(ass, "add_action_widget", ass_add_action_widget, 1);
    rb_define_method(ass, "remove_action_widget", ass_remove_action_widget, 1);
    rb_define_method(ass, "update_buttons_state", ass_update_buttons_state, 0);

    G_DEF_SETTERS(ass);

    /* GtkAssistantPageType */
    G_DEF_CLASS(GTK_TYPE_ASSISTANT_PAGE_TYPE, "PageType", ass);
    G_DEF_CONSTANTS(ass, GTK_TYPE_ASSISTANT_PAGE_TYPE, "GTK_ASSISTANT_");
#endif
}
