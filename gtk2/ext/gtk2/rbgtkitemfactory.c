/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Hiroshi Igarashi,
 *                          dellin,
 *                          MUTOH Masao
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
#include <gdk-pixbuf/gdk-pixdata.h>

#define RG_TARGET_NAMESPACE cItemFactory
#define _SELF(self) (GTK_ITEM_FACTORY(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))
#define RVAL2ACCEL(a)  (GTK_ACCEL_GROUP(RVAL2GOBJ(a)))

static VALUE action_table;
static guint action_id = 0;

static VALUE
rg_initialize(VALUE self, VALUE type, VALUE path, VALUE accel)
{
    VALUE obj = rb_eval_string("eval('self', binding)");
    G_RELATIVE(obj, self);
    RBGTK_INITIALIZE(self, gtk_item_factory_new(NUM2ULONG(type), RVAL2CSTR(path),
                                                RVAL2ACCEL(accel)));

    return Qnil;
}

static VALUE
rg_construct(int argc, VALUE *argv, VALUE self)
{
    VALUE type, path, accel;
    GtkItemFactory* ifact = _SELF(self);
    ifact->accel_group = NULL;

    rb_scan_args(argc, argv, "21", &type, &path, &accel);
    gtk_item_factory_construct(ifact, FIX2INT(type), 
                               RVAL2CSTR(path), RVAL2ACCEL(accel));
    return self;
}

static VALUE
rg_s_from_widget(VALUE self, VALUE widget)
{
    VALUE obj = GOBJ2RVAL(gtk_item_factory_from_widget(RVAL2WIDGET(widget)));
    G_RELATIVE(obj, self);
    return obj;
}

static VALUE
rg_s_path_from_widget(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    return CSTR2RVAL(gtk_item_factory_path_from_widget(RVAL2WIDGET(widget)));
}

static VALUE
rg_get_item(VALUE self, VALUE path)
{
    VALUE obj = Qnil;
    GtkWidget* widget = gtk_item_factory_get_item(_SELF(self), RVAL2CSTR(path));
    if (widget){
        obj = GOBJ2RVAL(widget);
        G_RELATIVE(obj, self);
    }
    return obj;
}

static VALUE
rg_get_widget(VALUE self, VALUE path)
{
    VALUE obj = Qnil;
    GtkWidget* widget = gtk_item_factory_get_widget(_SELF(self), RVAL2CSTR(path));
    if (widget){
        obj = GOBJ2RVAL(widget);
        G_RELATIVE(obj, self);
    }
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
menuitem_type_check(char *item_type)
{
    if (item_type == NULL || strcmp(item_type, "<Branch>") == 0 
        || (strcmp(item_type, "<LastBranch>") == 0)
        || (strcmp(item_type, "<Separator>") == 0))
        return 0;
    else 
        return -1;
}

static void
items_exec_callback_wrap(G_GNUC_UNUSED VALUE callback_data, guint action_id, GtkWidget *widget)
{
    VALUE iter, data;
    VALUE action = rb_hash_aref(action_table, UINT2NUM(action_id));

    iter = RARRAY_PTR(action)[0];
    data = RARRAY_PTR(action)[1];
    if (!NIL_P(iter)) {
        rb_funcall(iter, id_call, 2, data, GOBJ2RVAL(widget));
    }
}

static void
create_factory_entry(GtkItemFactoryEntry *entry, VALUE self, VALUE path, VALUE item_type, VALUE accel, VALUE extdata, VALUE func, VALUE data)
{
    VALUE action;

    entry->path = (gchar *)(NIL_P(path) ? NULL : RVAL2CSTR(path));
    entry->item_type = (gchar *)(NIL_P(item_type) ? "<Branch>" : RVAL2CSTR(item_type));
    entry->accelerator = (gchar *)(NIL_P(accel) ? NULL : RVAL2CSTR(accel));

    if (menuitem_type_check(entry->item_type) == 0) {
        entry->callback = NULL;
    } else {
        if (NIL_P(func)) {
            entry->callback = NULL;
        } else {
            entry->callback = items_exec_callback_wrap;
        }
    }
    action = rb_ary_new3(2, func, data);
    G_RELATIVE(self, action);
    rb_hash_aset(action_table, UINT2NUM(action_id), action);
    entry->callback_action = action_id;
    action_id++;

    if (NIL_P(extdata)){
        entry->extra_data = NULL;
    } else if (TYPE(extdata) == T_STRING){
        entry->extra_data = RVAL2CSTR(extdata);
    } else if (TYPE(extdata) == T_SYMBOL){
        entry->extra_data = rb_id2name(SYM2ID(extdata));
    } else if (RVAL2GTYPE(extdata) == GDK_TYPE_PIXBUF){
        GdkPixdata pixdata;
        guint stream_length_p;
        gdk_pixdata_from_pixbuf(&pixdata, GDK_PIXBUF(RVAL2GOBJ(extdata)), TRUE);
        entry->extra_data = gdk_pixdata_serialize(&pixdata, &stream_length_p);
    } else {
        entry->extra_data = NULL;
    }
}  

static VALUE
rg_create_item(int argc, VALUE *argv, VALUE self)
{
    VALUE path, type, accel, extdata, data, func;
    GtkItemFactoryEntry *entry;

    rb_scan_args(argc, argv, "14", &path, &type, &accel, &extdata, &data);

    entry = ALLOC(GtkItemFactoryEntry);
    func = rb_rescue((VALUE(*)())rb_block_proc, 0, NULL, 0);

    create_factory_entry(entry, self, path, type, accel, extdata, func, data);

    gtk_item_factory_create_item(_SELF(self), entry, (gpointer)self, 1);
    g_free(entry);
    return self;
}

static VALUE
rg_create_items(VALUE self, VALUE ary)
{
    VALUE entry, path, accel, type, func, data, extdata;
    GtkItemFactoryEntry *entries;
    guint i, len, n_menu_entries;

    n_menu_entries = RARRAY_LEN(ary);
    entries = ALLOC_N(GtkItemFactoryEntry, n_menu_entries);

    for (i = 0; i < n_menu_entries; i++) {
        entry = RARRAY_PTR(ary)[i];
        len = RARRAY_LEN(entry);
        Check_Type(entry, T_ARRAY);
        path =  RARRAY_PTR(entry)[0];
        type =  ((len > 1) ? RARRAY_PTR(entry)[1] : Qnil);
        accel = ((len > 2) ? RARRAY_PTR(entry)[2] : Qnil);
        extdata = ((len > 3) ? RARRAY_PTR(entry)[3] : Qnil);
        func =  ((len > 4) ? RARRAY_PTR(entry)[4] : Qnil);
        data =  ((len > 5) ? RARRAY_PTR(entry)[5] : Qnil);

        create_factory_entry(&entries[i], self, path, type, accel, extdata, func, data);
    }

    gtk_item_factory_create_items (_SELF(self), n_menu_entries, entries, NULL);
    g_free(entries);

    return self;
}

static VALUE
rg_delete_item(VALUE self, VALUE path)
{
    gtk_item_factory_delete_item(_SELF(self), RVAL2CSTR(path));
    return self;
}

/*
Don't implement. Use Gtk::ItemFactory#delete_item instead.
void        gtk_item_factory_delete_entry   (GtkItemFactory *ifactory,
                                             GtkItemFactoryEntry *entry);
void        gtk_item_factory_delete_entries (GtkItemFactory *ifactory,
                                             guint n_entries,
                                             GtkItemFactoryEntry *entries);
*/

static VALUE
rg_popup(VALUE self, VALUE x, VALUE y, VALUE mouse_button, VALUE time)
{
    gtk_item_factory_popup(_SELF(self), NUM2UINT(x), NUM2UINT(y), NUM2UINT(mouse_button),
                           NUM2UINT(time));
    return self;
}
/*
Do we need these methods?
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
Init_gtk_itemfactory(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ITEM_FACTORY, "ItemFactory", mGtk);

    RG_DEF_SMETHOD(from_widget, 1);
    RG_DEF_SMETHOD(path_from_widget, 1);

    RG_DEF_METHOD(initialize, 3);
    RG_DEF_METHOD(construct, -1);
    RG_DEF_METHOD(get_item, 1);
    RG_DEF_METHOD(get_widget, 1);
    RG_DEF_METHOD(create_item, -1);
    RG_DEF_METHOD(create_items, 1);
    RG_DEF_METHOD(delete_item, 1);
    RG_DEF_METHOD(popup, 4);

    /* Ruby/GTK Original constants */
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_MENU_BAR", INT2FIX(GTK_TYPE_MENU_BAR));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_MENU", INT2FIX(GTK_TYPE_MENU));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_OPTION_MENU", INT2FIX(GTK_TYPE_OPTION_MENU));

    rb_define_const(RG_TARGET_NAMESPACE, "TITLE", CSTR2RVAL("<Title>"));
    rb_define_const(RG_TARGET_NAMESPACE, "ITEM", CSTR2RVAL("<Item>"));
    rb_define_const(RG_TARGET_NAMESPACE, "CHECK_ITEM", CSTR2RVAL("<CheckItem>"));
    rb_define_const(RG_TARGET_NAMESPACE, "TOGGLE_ITEM", CSTR2RVAL("<ToggleItem>"));
    rb_define_const(RG_TARGET_NAMESPACE, "RADIO_ITEM", CSTR2RVAL("<RadioItem>"));
    rb_define_const(RG_TARGET_NAMESPACE, "IMAGE_ITEM", CSTR2RVAL("<ImageItem>"));
    rb_define_const(RG_TARGET_NAMESPACE, "STOCK_ITEM", CSTR2RVAL("<StockItem>"));
    rb_define_const(RG_TARGET_NAMESPACE, "SEPARATOR", CSTR2RVAL("<Separator>"));
    rb_define_const(RG_TARGET_NAMESPACE, "BRANCH", CSTR2RVAL("<Branch>"));
    rb_define_const(RG_TARGET_NAMESPACE, "LAST_BRANCH", CSTR2RVAL("<LastBranch>"));
    rb_define_const(RG_TARGET_NAMESPACE, "TEAROFF", CSTR2RVAL("<Tearoff>"));

    action_table = rb_hash_new();
    rb_global_variable(&action_table);
}
