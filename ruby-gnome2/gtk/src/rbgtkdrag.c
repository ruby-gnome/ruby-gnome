/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkdrag.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:33 $

  Copyright (C) 2002 MUTOH Masao
************************************************/


#include "global.h"

VALUE mGtkDrag;

static VALUE
gtkdrag_finish(self, context, success, del, time)
    VALUE self, context, success, del, time;
{
    gtk_drag_finish(get_gdkdragcontext(context), RTEST(success),
						RTEST(del), NUM2INT(time));
    return Qnil;
}

static VALUE
gtkdrag_get_source_widget(self, context)
    VALUE self, context;
{
    gtk_drag_get_source_widget(get_gdkdragcontext(context));
    return Qnil;
}

static VALUE
gtkdrag_begin(self, widget, targets, actions, button, event)
    VALUE self, widget, targets, actions, button, event;
{
/*	return make_gdkdragcontext(gtk_drag_begin(get_widget(self), ); */
    rb_notimplement();
    return Qnil;
}

static VALUE
gtkdrag_set_icon_widget(self, context, widget, hot_x, hot_y)
    VALUE self, context, widget, hot_x, hot_y;
{
    gtk_drag_set_icon_widget(get_gdkdragcontext(context), 
                 get_widget(widget), NUM2INT(hot_x), NUM2INT(hot_y));
    return Qnil;
}

static VALUE
gtkdrag_set_icon_pixmap(self, context, colormap, pixmap, mask, hot_x, hot_y)
    VALUE self, context, colormap, pixmap, mask, hot_x, hot_y;
{
    gtk_drag_set_icon_pixmap(get_gdkdragcontext(context), 
                 get_gdkcmap(colormap), get_gdkpixmap(pixmap),
                 get_gdkbitmap(mask), NUM2INT(hot_x), NUM2INT(hot_y));
    return Qnil;
}

static VALUE
gtkdrag_set_default_icon(self, context)
    VALUE self, context;
{
    gtk_drag_set_icon_default(get_gdkdragcontext(context));
    return Qnil;
}

void
Init_gtk_drag()
{
  mGtkDrag = rb_define_module_under(mGtk, "Drag");

  rb_define_module_function(mGtkDrag, "finish", gtkdrag_finish, 4);
  rb_define_module_function(mGtkDrag, "get_source_widget", gtkdrag_get_source_widget, 1);
  rb_define_module_function(mGtkDrag, "begin", gtkdrag_begin, 5);
  rb_define_module_function(mGtkDrag, "set_icon_widget", gtkdrag_set_icon_widget, 4);
  rb_define_module_function(mGtkDrag, "set_icon_pixmap", gtkdrag_set_icon_pixmap, 6);
  rb_define_module_function(mGtkDrag, "set_default_icon", gtkdrag_set_default_icon, 1);
}
