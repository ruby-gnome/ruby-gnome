/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkitemfactory.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Hiroshi Igarashi,
                          dellin,
                          MUTOH Masao

************************************************/

#include "global.h"

static const guint cb_type = 2;

static VALUE
ifact_initialize(self, type, path, accel)
    VALUE self;
    VALUE type, path, accel;
{
    RBGTK_INITIALIZE(self, gtk_item_factory_new(FIX2INT(type),
												STR2CSTR(path),
												get_gtkaccelgrp(accel)));
    return Qnil;
}

static VALUE
distinguish_item_type(p_item)
	GtkWidget *p_item;
{
    if (GTK_IS_RADIO_MENU_ITEM(p_item))
        return rb_obj_alloc(gRMenuItem);
    else if (GTK_IS_CHECK_MENU_ITEM(p_item))
        return rb_obj_alloc(gCMenuItem);
    else if (GTK_IS_TEAROFF_MENU_ITEM(p_item))
        return rb_obj_alloc(gTMenuItem);
    else if (GTK_IS_MENU_ITEM(p_item))
        return rb_obj_alloc(gMenuItem);
    else if (GTK_IS_LIST_ITEM(p_item))
        return rb_obj_alloc(gListItem);
#ifdef GTK_ENABLE_BROKEN
    else if (GTK_IS_TREE_ITEM(p_item))
        return rb_obj_alloc(gTreeItem);
#endif
    else
        return rb_obj_alloc(gItem);
}

static int
menuitem_type_check(item_type)
    char *item_type;
{
    if (item_type == NULL) {
		return -1;
    }
    if ((strcmp(item_type, "<Branch>") == 0) 
		|| (strcmp(item_type, "<LastBranch>") == 0)
		|| (strcmp(item_type, "<Separator>") == 0))
		return 0;
    else 
		return -1;
}

static void
item_exec_callback_wrap(p_item, ifact, iter)
    GtkWidget *p_item;
    VALUE ifact;
    VALUE iter;
{
    VALUE item;
    item = distinguish_item_type(p_item);
    RBGTK_INITIALIZE(item, GTK_WIDGET(p_item));

    if (!NIL_P(iter)) {
        rb_funcall(iter, id_call, 1, item);
    }
}

static void
items_exec_callback_wrap(p_item, func_data_ary)
    GtkWidget *p_item; 
    VALUE func_data_ary;
{
    VALUE iter, data;

    iter = rb_ary_entry(func_data_ary, 0);
    data = rb_ary_entry(func_data_ary, 1);

    if (!NIL_P(iter)) {
        rb_funcall(iter, id_call, 1, data);
    }
}

static VALUE
ifact_create_item(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE path, accel, item_type, action;
    GtkItemFactoryEntry *entry;

    rb_scan_args(argc, argv, "12",
                 &path, &accel, &item_type);

    entry = ALLOC(GtkItemFactoryEntry);

    entry->path = NIL_P(path)?NULL:STR2CSTR(path);
    entry->accelerator = NIL_P(accel)?NULL:STR2CSTR(accel);
    entry->item_type = NIL_P(item_type)?NULL:STR2CSTR(item_type);
    entry->callback = item_exec_callback_wrap;
    if (menuitem_type_check(entry->item_type) != 0) {
        action = rb_rescue((VALUE(*)())rb_f_lambda, 0, NULL, 0);
        add_relative(self, action);
        entry->callback_action = action;
    }
  
    if (NIL_P(item_type)) {
        gtk_item_factory_create_item(GTK_ITEM_FACTORY(RVAL2GOBJ(self)),
                                     entry, (gpointer)self, cb_type);
    }
    else if (menuitem_type_check(entry->item_type) == 0) {
        entry->callback = NULL;
        entry->callback_action = 0;
        gtk_item_factory_create_item(GTK_ITEM_FACTORY(RVAL2GOBJ(self)),
                                     entry, (gpointer)self, cb_type);
    }
    else {
        gtk_item_factory_create_item(GTK_ITEM_FACTORY(RVAL2GOBJ(self)),
                                     entry, (gpointer)self, cb_type);
    }

    g_free(entry);

    return Qnil;
}

static VALUE
ifact_create_items(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE rb_entries, cb_data;
    VALUE rb_entry, rb_path, rb_accel, rb_type, rb_func, rb_data;
    VALUE action;
    GtkItemFactoryEntry *entries;
    GtkAccelGroup *accel_group;
    guint i, n_menu_entries;

    accel_group = gtk_accel_group_new();

    rb_scan_args(argc, argv, "11", &rb_entries, &cb_data);

    n_menu_entries = FIX2INT(rb_funcall(rb_entries, rb_intern("length"), 0));

    entries = ALLOC_N(GtkItemFactoryEntry, n_menu_entries);

    for (i = 0; i < n_menu_entries; i++) {
        rb_entry = rb_ary_entry(rb_entries, i);
        Check_Type(rb_entry, T_ARRAY);
        rb_path = rb_ary_entry(rb_entry, 0);
        rb_accel = rb_ary_entry(rb_entry, 1);
        rb_type = rb_ary_entry(rb_entry, 2);
        rb_func = rb_ary_entry(rb_entry, 3);
        rb_data = rb_ary_entry(rb_entry, 4);
        entries[i].path = NIL_P(rb_path)?NULL:STR2CSTR(rb_path);
        entries[i].accelerator = NIL_P(rb_accel)?NULL:STR2CSTR(rb_accel);
        entries[i].item_type = NIL_P(rb_type)?NULL:STR2CSTR(rb_type);
        if (menuitem_type_check(entries[i].item_type) != 0) {
            if (!NIL_P(rb_func)) {
                entries[i].callback = items_exec_callback_wrap;
            }
            action = rb_ary_new3(4, rb_func, rb_data, self, rb_path);
            add_relative(self, action);
            entries[i].callback_action = action;
        }
    }

    gtk_item_factory_create_items (GTK_ITEM_FACTORY(RVAL2GOBJ(self)),
                                   n_menu_entries, entries, NULL);
    g_free(entries);

    return Qnil;
}

static VALUE
ifact_get_gobject(self, path)
    VALUE self, path;
{
    GtkWidget *p_menu;
    VALUE menuobj;

    p_menu = gtk_item_factory_get_widget(GTK_ITEM_FACTORY(RVAL2GOBJ(self)),
										 STR2CSTR(path));
    if (GTK_IS_OPTION_MENU(p_menu))
        menuobj = rb_obj_alloc(gOptionMenu);
    else if (GTK_IS_MENU(p_menu))
        menuobj = rb_obj_alloc(gMenu);
    else
        menuobj = rb_obj_alloc(gMenuBar);

    RBGTK_INITIALIZE(menuobj, p_menu);
  
    return menuobj;
}

static VALUE
ifact_get_item(self, path)
    VALUE self, path;
{
    GtkWidget *p_item;
    VALUE item;

    p_item = gtk_item_factory_get_item(GTK_ITEM_FACTORY(RVAL2GOBJ(self)),
                                       STR2CSTR(path));
    item = distinguish_item_type(p_item);
    RBGTK_INITIALIZE(item, p_item);

    return item;
}

static VALUE
ifact_delete_item(self, path)
    VALUE self, path;
{
    gtk_item_factory_delete_item(GTK_ITEM_FACTORY(RVAL2GOBJ(self)),
                                 STR2CSTR(path));
    return Qnil;
}

static VALUE
ifact_s_path_from_widget(self, widget)
    VALUE self, widget;
{
    return rb_str_new2(gtk_item_factory_path_from_widget(GTK_WIDGET(RVAL2GOBJ(widget))));
}

void Init_gtk_itemfactory()
{
    static rbgtk_class_info cinfo;

    gItemFactory = rb_define_class_under(mGtk, "ItemFactory", gObject);
    gIFConst = rb_define_module_under(gItemFactory, "Constants");

    cinfo.klass = gItemFactory;
    cinfo.gtype = GTK_TYPE_ITEM_FACTORY;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gIFConst, "TYPE_MENU_BAR", INT2FIX(GTK_TYPE_MENU_BAR));
    rb_define_const(gIFConst, "TYPE_MENU", INT2FIX(GTK_TYPE_MENU));
    rb_define_const(gIFConst, "TYPE_OPTION_MENU",
                    INT2FIX(GTK_TYPE_OPTION_MENU));
  
    rb_define_const(gIFConst, "TITLE", rb_str_new2("<Title>"));
    rb_define_const(gIFConst, "ITEM", rb_str_new2("<Item>"));
    rb_define_const(gIFConst, "CHECK_ITEM", rb_str_new2("<CheckItem>"));
    rb_define_const(gIFConst, "TOGGLE_ITEM", rb_str_new2("<ToggleItem>"));
    rb_define_const(gIFConst, "RADIO_ITEM", rb_str_new2("<RadioItem>"));
    rb_define_const(gIFConst, "SEPARATOR", rb_str_new2("<Separator>"));
    rb_define_const(gIFConst, "BRANCH", rb_str_new2("<Branch>"));
    rb_define_const(gIFConst, "LAST_BRANCH", rb_str_new2("<LastBranch>"));
    rb_define_const(gIFConst, "TEAROFF", rb_str_new2("<Tearoff>"));
  
    rb_include_module(gItemFactory, gIFConst);

    rb_define_method(gItemFactory, "initialize", ifact_initialize, 3);
    rb_define_method(gItemFactory, "create_item", ifact_create_item, -1);
    rb_define_method(gItemFactory, "create_items", ifact_create_items, -1);
    rb_define_method(gItemFactory, "delete_item", ifact_delete_item, 1);
    rb_define_method(gItemFactory, "get_widget", ifact_get_gobject, 1);
    rb_define_method(gItemFactory, "get_item", ifact_get_item, 1);
 
    rb_define_singleton_method(gItemFactory, "path_from_widget",
                               ifact_s_path_from_widget, 1);
}
