/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkitemfactory.c -

  $Author: mutoh $
  $Date: 2002/10/30 13:34:36 $

  Copyright (C) 1998-2000 Hiroshi Igarashi,
                          dellin,
                          MUTOH Masao

************************************************/

#include "global.h"

#define _SELF(self) (GTK_ITEM_FACTORY(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))
#define RVAL2ACCEL(a)  (GTK_ACCEL_GROUP(RVAL2GOBJ(a)))

static VALUE
ifact_initialize(self, type, path, accel)
    VALUE self;
    VALUE type, path, accel;
{
    VALUE obj = rb_eval_string("eval('self', binding)");
    G_RELATIVE(obj, self);
    RBGTK_INITIALIZE(self, gtk_item_factory_new(FIX2INT(type), RVAL2CSTR(path),
                                                RVAL2ACCEL(accel)));
   
    return Qnil;
}

static VALUE
ifact_construct(self, type, path, accel)
    VALUE self, type, path, accel;
{
    gtk_item_factory_construct(_SELF(self), FIX2INT(type), 
                               RVAL2CSTR(path), RVAL2ACCEL(accel));
    return self;
}
 
static VALUE
ifact_s_add_foreign(self, accel_widget, full_path, accel_group, keyval, modifiers)
    VALUE self, accel_widget, full_path, accel_group, keyval, modifiers;
{
    gtk_item_factory_add_foreign(RVAL2WIDGET(accel_widget), RVAL2CSTR(full_path),
                                 RVAL2ACCEL(accel_group), NUM2UINT(keyval),
                                 FIX2INT(modifiers));
    return self;
}

static VALUE
ifact_s_from_widget(self, widget)
    VALUE self, widget;
{
    VALUE obj = GOBJ2RVAL(gtk_item_factory_from_widget(RVAL2WIDGET(widget)));
    G_RELATIVE(obj, self);
    return obj;
}

static VALUE
ifact_s_path_from_widget(self, widget)
    VALUE self, widget;
{
    return CSTR2RVAL(gtk_item_factory_path_from_widget(RVAL2WIDGET(widget)));
}

static VALUE
ifact_get_item(self, path)
    VALUE self, path;
{
    VALUE obj = GOBJ2RVAL(gtk_item_factory_get_item(_SELF(self), RVAL2CSTR(path)));
    G_RELATIVE(obj, self);
    return obj;
}

static VALUE
ifact_get_widget(self, path)
    VALUE self, path;
{
    VALUE obj = GOBJ2RVAL(gtk_item_factory_get_widget(_SELF(self), RVAL2CSTR(path)));
    G_RELATIVE(obj, self);
    return obj;
}

/* We can't implement them because actions are always 1.
GtkWidget*  gtk_item_factory_get_widget_by_action
                                            (GtkItemFactory *ifactory,
                                             guint action);
GtkWidget*  gtk_item_factory_get_item_by_action
                                            (GtkItemFactory *ifactory,
                                             guint action);
*/

static int
menuitem_type_check(item_type)
    char *item_type;
{
    if (item_type == NULL || strcmp(item_type, "<Branch>") == 0 
        || (strcmp(item_type, "<LastBranch>") == 0)
        || (strcmp(item_type, "<Separator>") == 0))
        return 0;
    else 
        return -1;
}

static void
item_exec_callback_wrap(ifact, iter, p_item)
    VALUE ifact, iter;
    GtkWidget *p_item;
{
    if (!NIL_P(iter)) {
        rb_funcall(iter, id_call, 1, GOBJ2RVAL(p_item));
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

    rb_scan_args(argc, argv, "12", &path, &item_type, &accel);

    entry = ALLOC(GtkItemFactoryEntry);

    entry->path = NIL_P(path) ? NULL : RVAL2CSTR(path);
    entry->item_type = NIL_P(item_type) ? "<Branch>" : RVAL2CSTR(item_type);
    entry->accelerator = NIL_P(accel) ? NULL : RVAL2CSTR(accel);
    entry->callback = item_exec_callback_wrap;
    if (menuitem_type_check(entry->item_type) != 0) {
        action = rb_rescue((VALUE(*)())rb_f_lambda, 0, NULL, 0);
        G_RELATIVE(self, action);
        entry->callback_action = action;
    }
  
    if (! NIL_P(item_type) && menuitem_type_check(entry->item_type) == 0) {
        entry->callback = NULL;
        entry->callback_action = 0;
    }
    gtk_item_factory_create_item(_SELF(self), entry, (gpointer)self, 1);
    g_free(entry);
    return self;
}

static void
items_exec_callback_wrap(callback_data, action, widget)
    VALUE callback_data, action;
    GtkWidget widget;
{
    VALUE iter, data;
    iter = RARRAY(action)->ptr[0];
    data = RARRAY(action)->ptr[1];
    if (!NIL_P(iter)) {
        rb_funcall(iter, id_call, 1, data);
    }
}

static VALUE
ifact_create_items(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE ary, cb_data;
    VALUE rb_entry, rb_path, rb_accel, rb_type, rb_func, rb_data;
    VALUE action;
    GtkItemFactoryEntry *entries;
    guint i, len, n_menu_entries;

    rb_scan_args(argc, argv, "11", &ary, &cb_data);

    n_menu_entries = RARRAY(ary)->len;

    entries = ALLOC_N(GtkItemFactoryEntry, n_menu_entries);

    for (i = 0; i < n_menu_entries; i++) {
        rb_entry = RARRAY(ary)->ptr[i];
        len = RARRAY(rb_entry)->len;
        Check_Type(rb_entry, T_ARRAY);
        rb_path =  RARRAY(rb_entry)->ptr[0];
        rb_type =  ((len > 1) ? RARRAY(rb_entry)->ptr[1] : Qnil);
        rb_accel = ((len > 2) ? RARRAY(rb_entry)->ptr[2] : Qnil);
        rb_func =  ((len > 3) ? RARRAY(rb_entry)->ptr[3] : Qnil);
        rb_data =  ((len > 4) ? RARRAY(rb_entry)->ptr[4] : Qnil);
        entries[i].path = NIL_P(rb_path) ? NULL : RVAL2CSTR(rb_path);
        entries[i].item_type = NIL_P(rb_type) ? "<Branch>" : RVAL2CSTR(rb_type);
        entries[i].accelerator = NIL_P(rb_accel) ? NULL : RVAL2CSTR(rb_accel);

        if (menuitem_type_check(entries[i].item_type) == 0) {
            entries[i].callback = NULL;
        } else {
            if (NIL_P(rb_func)) {
                entries[i].callback = NULL;
            } else {
                entries[i].callback = items_exec_callback_wrap;
            }
        }
        action = rb_ary_new3(4, rb_func, rb_data, self, rb_path);

        G_RELATIVE(self, action);
        entries[i].callback_action = action;
    }

    gtk_item_factory_create_items (_SELF(self), n_menu_entries, entries, NULL);
    g_free(entries);

    return self;
}


static VALUE
ifact_delete_item(self, path)
    VALUE self, path;
{
    gtk_item_factory_delete_item(_SELF(self), RVAL2CSTR(path));
    return self;
}

/*
void        gtk_item_factory_delete_entry   (GtkItemFactory *ifactory,
                                             GtkItemFactoryEntry *entry);
void        gtk_item_factory_delete_entries (GtkItemFactory *ifactory,
                                             guint n_entries,
                                             GtkItemFactoryEntry *entries);
void        gtk_item_factory_popup          (GtkItemFactory *ifactory,
                                             guint x,
                                             guint y,
                                             guint mouse_button,
                                             guint32 time);
void        gtk_item_factory_popup_with_data
                                            (GtkItemFactory *ifactory,
                                             gpointer popup_data,
                                             GtkDestroyNotify destroy,
                                             guint x,
                                             guint y,
                                             guint mouse_button,
                                             guint32 time);

gpointer    gtk_item_factory_popup_data     (GtkItemFactory *ifactory);
gpointer    gtk_item_factory_popup_data_from_widget
                                            (GtkWidget *widget);
void        gtk_item_factory_set_translate_func
                                            (GtkItemFactory *ifactory,
                                             GtkTranslateFunc func,
                                             gpointer data,
                                             GtkDestroyNotify notify);
*/


void 
Init_gtk_itemfactory()
{
    VALUE gItemFactory = G_DEF_CLASS(GTK_TYPE_ITEM_FACTORY, "ItemFactory", mGtk);

    rb_define_singleton_method(gItemFactory, "add_foreign", ifact_s_add_foreign, 4);
    rb_define_singleton_method(gItemFactory, "from_widget", ifact_s_from_widget, 1);
    rb_define_singleton_method(gItemFactory, "path_from_widget", ifact_s_path_from_widget, 1);

    rb_define_method(gItemFactory, "initialize", ifact_initialize, 3);
    rb_define_method(gItemFactory, "construct", ifact_construct, 3);
    rb_define_method(gItemFactory, "get_item", ifact_get_item, 1);
    rb_define_method(gItemFactory, "get_widget", ifact_get_widget, 1);
    rb_define_method(gItemFactory, "create_item", ifact_create_item, -1);
    rb_define_method(gItemFactory, "create_items", ifact_create_items, -1);
    rb_define_method(gItemFactory, "delete_item", ifact_delete_item, 1);

    /* Ruby/GTK Original constants */
    rb_define_const(gItemFactory, "TYPE_MENU_BAR", INT2FIX(GTK_TYPE_MENU_BAR));
    rb_define_const(gItemFactory, "TYPE_MENU", INT2FIX(GTK_TYPE_MENU));
    rb_define_const(gItemFactory, "TYPE_OPTION_MENU", INT2FIX(GTK_TYPE_OPTION_MENU));

    rb_define_const(gItemFactory, "TITLE", rb_str_new2("<Title>"));
    rb_define_const(gItemFactory, "ITEM", rb_str_new2("<Item>"));
    rb_define_const(gItemFactory, "CHECK_ITEM", rb_str_new2("<CheckItem>"));
    rb_define_const(gItemFactory, "TOGGLE_ITEM", rb_str_new2("<ToggleItem>"));
    rb_define_const(gItemFactory, "RADIO_ITEM", rb_str_new2("<RadioItem>"));
    rb_define_const(gItemFactory, "SEPARATOR", rb_str_new2("<Separator>"));
    rb_define_const(gItemFactory, "BRANCH", rb_str_new2("<Branch>"));
    rb_define_const(gItemFactory, "LAST_BRANCH", rb_str_new2("<LastBranch>"));
    rb_define_const(gItemFactory, "TEAROFF", rb_str_new2("<Tearoff>"));
}
