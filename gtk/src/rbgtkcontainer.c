/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkcontainer.c -

  $Author: mutoh $
  $Date: 2002/05/19 15:48:28 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
cont_set_bwidth(self, width)
    VALUE self, width;
{
    gtk_container_border_width(GTK_CONTAINER(get_widget(self)),
			       NUM2INT(width));
    return self;
}

static VALUE
cont_get_bwidth(self)
    VALUE self;
{
    return INT2NUM(GTK_CONTAINER(get_widget(self))->border_width);
}

static VALUE
cont_bwidth(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE width;
    rb_scan_args(argc, argv, "01", &width);

    return NIL_P(width)
	? cont_get_bwidth(self)
	: cont_set_bwidth(self, width);
}

static VALUE
cont_add(self, other)
    VALUE self, other;
{
    gtk_container_add(GTK_CONTAINER(get_widget(self)), get_widget(other));
    return self;
}

static VALUE
cont_remove(self, other)
    VALUE self, other;
{
    gtk_container_remove(GTK_CONTAINER(get_widget(self)), get_widget(other));
    return self;
}

static VALUE
cont_foreach(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE callback;

    rb_scan_args(argc, argv, "01", &callback);
    if (NIL_P(callback)) {
	callback = rb_f_lambda();
    }
    gtk_container_foreach(GTK_CONTAINER(get_widget(self)), 
			  exec_callback, (gpointer)callback);
    return self;
}

static void
yield_callback(widget)
    GtkWidget *widget;
{
    rb_yield(get_value_from_gobject(GTK_OBJECT(widget)));
}

static VALUE
cont_each(self)
    VALUE self;
{
    gtk_container_foreach(GTK_CONTAINER(get_widget(self)), 
			  yield_callback, 0);
    return self;
}

static VALUE
cont_set_focus_child(self, child)
    VALUE self, child;
{
    gtk_container_set_focus_child(GTK_CONTAINER(get_widget(self)),
                                  GTK_WIDGET(get_widget(child)));
    return self;
}

static VALUE
cont_set_focus_vadjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_container_set_focus_vadjustment(GTK_CONTAINER(get_widget(self)),
                                        GTK_ADJUSTMENT(get_gobject(adjustment)));
    return self;
}

static VALUE
cont_set_focus_hadjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_container_set_focus_hadjustment(GTK_CONTAINER(get_widget(self)),
                                        GTK_ADJUSTMENT(get_gobject(adjustment)));
    return self;
}

static void
cont_children_callback(widget, data)
    GtkWidget *widget;
    gpointer data;
{
    VALUE ary = (VALUE)data;

    rb_ary_push(ary, get_value_from_gobject(GTK_OBJECT(widget)));
}

static VALUE
cont_children(self)
    VALUE self;
{
    VALUE ary = rb_ary_new();

    gtk_container_foreach(GTK_CONTAINER(get_widget(self)),
			  cont_children_callback,
			  (gpointer)ary);
    return ary;
}

void Init_gtk_container()
{
    gContainer = rb_define_class_under(mGtk, "Container", gWidget);

    rb_define_const(gContainer, "SIGNAL_ADD", rb_str_new2("add"));
    rb_define_const(gContainer, "SIGNAL_REMOVE", rb_str_new2("remove"));
    rb_define_const(gContainer, "SIGNAL_NEED_RESIZE", rb_str_new2("need_resize"));
    rb_define_const(gContainer, "SIGNAL_FOREACH", rb_str_new2("foreach"));
    rb_define_const(gContainer, "SIGNAL_FOCUS", rb_str_new2("focus"));

    rb_define_method(gContainer, "border_width", cont_bwidth, -1);
    rb_define_method(gContainer, "border_width=", cont_set_bwidth, 1);
    rb_define_method(gContainer, "add", cont_add, 1);
    rb_define_method(gContainer, "remove", cont_remove, 1);
    rb_define_method(gContainer, "foreach", cont_foreach, -1);
    rb_define_method(gContainer, "each", cont_each, 0);
    rb_define_method(gContainer, "children", cont_children, 0);
    rb_define_method(gContainer, "set_focus_child", cont_set_focus_child, 1);
    rb_define_method(gContainer, "set_focus_vadjustment", cont_set_focus_vadjustment, 1);
    rb_define_method(gContainer, "set_focus_hadjustment", cont_set_focus_hadjustment, 1);

    /* child initialization */
    Init_gtk_bin();
    Init_gtk_box();
    Init_gtk_clist();
    Init_gtk_fixed();
    Init_gtk_layout();
    Init_gtk_list();
    Init_gtk_menu_shell();
    Init_gtk_notebook();
    Init_gtk_paned();
    Init_gtk_socket();
    Init_gtk_table();
    Init_gtk_toolbar();
    Init_gtk_tree();
}
