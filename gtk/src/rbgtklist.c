/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtklist.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef GTK_DISABLE_DEPRECATED

#include "global.h"

static VALUE
list_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_list_new());
    return Qnil;
}

static VALUE
list_set_sel_mode(self, mode)
    VALUE self, mode;
{
    gtk_list_set_selection_mode(GTK_LIST(get_widget(self)),
				(GtkSelectionMode)NUM2INT(mode));
    return self;
}

static VALUE
list_sel_mode(self)
    VALUE self;
{
    return INT2FIX(GTK_LIST(get_widget(self))->selection_mode);
}

static VALUE
list_selection(self)
    VALUE self;
{
    return glist2ary(GTK_LIST(get_widget(self))->selection);
}

static VALUE
list_insert_items(self, items, pos)
    VALUE self, items, pos;
{
    GList *glist;

    glist = ary2glist(items);

    gtk_list_insert_items(GTK_LIST(get_widget(self)), glist, NUM2INT(pos));

    return self;
}

static VALUE
list_append_items(self, items)
    VALUE self, items;
{
    GList *glist;

    glist = ary2glist(items);

    gtk_list_append_items(GTK_LIST(get_widget(self)), glist);

    return self;
}

static VALUE
list_prepend_items(self, items)
    VALUE self, items;
{
    GList *glist;

    glist = ary2glist(items);
    gtk_list_prepend_items(GTK_LIST(get_widget(self)), glist);

    return self;
}

static VALUE
list_remove_items(self, items)
    VALUE self, items;
{
    GList *glist;

    glist = ary2glist(items);
    gtk_list_remove_items(GTK_LIST(get_widget(self)), glist);

    return self;
}

static VALUE
list_clear_items(self, start, end)
    VALUE self, start, end;
{
    gtk_list_clear_items(GTK_LIST(get_widget(self)),
			 NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
list_select_item(self, pos)
    VALUE self, pos;
{
    gtk_list_select_item(GTK_LIST(get_widget(self)), NUM2INT(pos));
    return self;
}

static VALUE
list_unselect_item(self, pos)
    VALUE self, pos;
{
    gtk_list_unselect_item(GTK_LIST(get_widget(self)), NUM2INT(pos));
    return self;
}

static VALUE
list_select_child(self, child)
    VALUE self, child;
{
    gtk_list_select_child(GTK_LIST(get_widget(self)), get_widget(child));
    return self;
}

static VALUE
list_unselect_child(self, child)
    VALUE self, child;
{
    gtk_list_unselect_child(GTK_LIST(get_widget(self)), get_widget(child));
    return self;
}

static VALUE
list_child_position(self, child)
    VALUE self, child;
{
    gint pos;

    pos = gtk_list_child_position(GTK_LIST(get_widget(self)),
				  get_widget(child));
    return INT2FIX(pos);
}

void Init_gtk_list()
{
    static rbgtk_class_info cinfo;

    gList = rb_define_class_under(mGtk, "List", gContainer);
    cinfo.klass = gList;
    cinfo.gtype = GTK_TYPE_LIST;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gList, "SIGNAL_SELECTION_CHANGED", rb_str_new2("selection_changed"));
    rb_define_const(gList, "SIGNAL_SELECT_CHILD", rb_str_new2("select_child"));
    rb_define_const(gList, "SIGNAL_UNSELECT_CHILD", rb_str_new2("unselect_child"));

    rb_define_method(gList, "initialize", list_initialize, 0);
    rb_define_method(gList, "set_selection_mode", list_set_sel_mode, 1);
    rb_define_method(gList, "selection_mode", list_sel_mode, 0);
    rb_define_method(gList, "selection", list_selection, 0);
    rb_define_method(gList, "insert_items", list_insert_items, 2);
    rb_define_method(gList, "append_items", list_append_items, 1);
    rb_define_method(gList, "prepend_items", list_prepend_items, 1);
    rb_define_method(gList, "remove_items", list_remove_items, 1);
    rb_define_method(gList, "clear_items", list_clear_items, 2);
    rb_define_method(gList, "select_item", list_select_item, 1);
    rb_define_method(gList, "unselect_item", list_unselect_item, 1);
    rb_define_method(gList, "select_child", list_select_child, 1);
    rb_define_method(gList, "unselect_child", list_unselect_child, 1);
    rb_define_method(gList, "child_position", list_child_position, 1);
}
#endif
