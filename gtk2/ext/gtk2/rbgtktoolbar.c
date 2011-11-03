/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE cToolbar
#define _SELF(self) (GTK_TOOLBAR(RVAL2GOBJ(self)))
#define N_RVAL2CSTR(text) (NIL_P(text) ? NULL : RVAL2CSTR(text))
#define N_RVAL2WIDGET(w)  (NIL_P(w) ? NULL : GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_toolbar_new());
    return Qnil;
}

static VALUE
rg_append(int argc, VALUE *argv, VALUE self)
{
    GtkWidget* ret = NULL;
    VALUE type = Qnil;
    VALUE widget, stock_id, element_type, text, ttext, ptext, icon, func;

    if (argc > 0) type = argv[0];

    if (NIL_P(type) || TYPE(type) == T_STRING){
        rb_scan_args(argc, argv, "05", &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_append_item(_SELF(self), N_RVAL2CSTR(text),
                                      N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                      N_RVAL2WIDGET(icon),
                                      GTK_SIGNAL_FUNC(exec_callback),
                                      (gpointer)func);
    } else if (TYPE(type) == T_FIXNUM) {
        rb_scan_args(argc, argv, "07", &element_type, &widget, &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_append_element(_SELF(self), 
                                         RVAL2GENUM(element_type, GTK_TYPE_TOOLBAR_CHILD_TYPE), 
                                         N_RVAL2WIDGET(widget), N_RVAL2CSTR(text),
                                         N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                         N_RVAL2WIDGET(icon),
                                         GTK_SIGNAL_FUNC(exec_callback),
                                         (gpointer)func);
        if (RVAL2GENUM(element_type, GTK_TYPE_TOOLBAR_CHILD_TYPE) == GTK_TOOLBAR_CHILD_SPACE) 
            ret = NULL;
    } else if (TYPE(type) == T_SYMBOL) {
        rb_scan_args(argc, argv, "13", &stock_id, &ttext, &ptext, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_insert_stock(_SELF(self), rb_id2name(SYM2ID(stock_id)),
                                       N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                       GTK_SIGNAL_FUNC(exec_callback),
                                       (gpointer)func, -1);
    } else {
        rb_scan_args(argc, argv, "12", &widget, &ttext, &ptext);
        gtk_toolbar_append_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                                  N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext));
        return widget;
    }

    return ret ? GOBJ2RVAL(ret) : Qnil;
}

static VALUE
rg_prepend(int argc, VALUE *argv, VALUE self)
{
    GtkWidget* ret = NULL;
    VALUE type = Qnil;
    VALUE widget, stock_id, element_type, text, ttext, ptext, icon, func;

    if (argc > 0) type = argv[0];

    if (NIL_P(type) || TYPE(type) == T_STRING){
        rb_scan_args(argc, argv, "05", &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_prepend_item(_SELF(self), N_RVAL2CSTR(text),
                                      N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                      N_RVAL2WIDGET(icon),
                                      GTK_SIGNAL_FUNC(exec_callback),
                                      (gpointer)func);
    } else if (TYPE(type) == T_FIXNUM) {
        rb_scan_args(argc, argv, "07", &element_type, &widget, &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_prepend_element(_SELF(self), RVAL2GENUM(element_type, GTK_TYPE_TOOLBAR_CHILD_TYPE),
                                         N_RVAL2WIDGET(widget), N_RVAL2CSTR(text),
                                         N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                         N_RVAL2WIDGET(icon),
                                         GTK_SIGNAL_FUNC(exec_callback),
                                         (gpointer)func);
        if (RVAL2GENUM(element_type, GTK_TYPE_TOOLBAR_CHILD_TYPE) == GTK_TOOLBAR_CHILD_SPACE) 
            ret = NULL;
    } else if (TYPE(type) == T_SYMBOL) {
        rb_scan_args(argc, argv, "13", &stock_id, &ttext, &ptext, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_insert_stock(_SELF(self), rb_id2name(SYM2ID(stock_id)),
                                       N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                       GTK_SIGNAL_FUNC(exec_callback),
                                       (gpointer)func, 0);
    } else {
        rb_scan_args(argc, argv, "12", &widget, &ttext, &ptext);
        gtk_toolbar_prepend_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                                  N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext));
        return widget;
    }

    return ret ? GOBJ2RVAL(ret) : Qnil;
}

static VALUE
rg_insert(int argc, VALUE *argv, VALUE self)
{
    GtkWidget* ret = NULL;
    VALUE type = Qnil;
    VALUE pos, widget, stock_id, element_type, text, ttext, ptext, icon, func;

    if (argc > 1) type = argv[1];

    if (NIL_P(type) || TYPE(type) == T_STRING){
        rb_scan_args(argc, argv, "15", &pos, &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);
        ret = gtk_toolbar_insert_item(_SELF(self),N_RVAL2CSTR(text),
                                      N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                      N_RVAL2WIDGET(icon),
                                      GTK_SIGNAL_FUNC(exec_callback),
                                      (gpointer)func,
                                      NUM2INT(pos));
    } else if (TYPE(type) == T_FIXNUM) {
        rb_scan_args(argc, argv, "17", &pos, &element_type, &widget, &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_insert_element(_SELF(self),
                                         RVAL2GENUM(element_type, GTK_TYPE_TOOLBAR_CHILD_TYPE),
                                         N_RVAL2WIDGET(widget), 
                                         N_RVAL2CSTR(text), N_RVAL2CSTR(ttext),
                                         N_RVAL2CSTR(ptext), N_RVAL2WIDGET(icon),
                                         GTK_SIGNAL_FUNC(exec_callback),
                                         (gpointer)func,
                                         NUM2INT(pos));
        if (RVAL2GENUM(element_type, GTK_TYPE_TOOLBAR_CHILD_TYPE) == GTK_TOOLBAR_CHILD_SPACE) 
            ret = NULL;
    } else if (TYPE(type) == T_SYMBOL) {
        rb_scan_args(argc, argv, "14", &pos, &stock_id, &ttext, &ptext, &func);
        if (NIL_P(func)) func = rb_block_proc();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_insert_stock(_SELF(self), rb_id2name(SYM2ID(stock_id)),
                                       N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                       GTK_SIGNAL_FUNC(exec_callback),
                                       (gpointer)func, NUM2INT(pos));
#if GTK_CHECK_VERSION(2,4,0)
    } else if (rb_obj_is_kind_of(type, GTYPE2CLASS(GTK_TYPE_TOOL_ITEM))){
        rb_scan_args(argc, argv, "20", &pos, &widget);
        gtk_toolbar_insert(_SELF(self), GTK_TOOL_ITEM(RVAL2GOBJ(widget)),
                           NUM2INT(pos));
        return widget;
#endif
    } else {
        rb_scan_args(argc, argv, "22", &pos, &widget, &ttext, &ptext);
        gtk_toolbar_insert_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                                  N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                  NUM2INT(pos));
        return widget;
    }

    return ret ? GOBJ2RVAL(ret) : Qnil;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_item_index(VALUE self, VALUE item)
{
    return INT2NUM(gtk_toolbar_get_item_index(_SELF(self), GTK_TOOL_ITEM(RVAL2GOBJ(item))));
}

static VALUE
rg_n_items(VALUE self)
{
    return INT2NUM(gtk_toolbar_get_n_items(_SELF(self)));
}

static VALUE
rg_nth_item(VALUE self, VALUE n)
{
    return GOBJ2RVAL(gtk_toolbar_get_nth_item(_SELF(self), NUM2INT(n)));
}

static VALUE
rg_drop_index(VALUE self, VALUE x, VALUE y)
{
    return INT2NUM(gtk_toolbar_get_drop_index(_SELF(self), NUM2INT(x), NUM2INT(y)));
}

static VALUE
rg_set_drop_highlight_item(VALUE self, VALUE item, VALUE index)
{
    gtk_toolbar_set_drop_highlight_item(_SELF(self), 
                                        GTK_TOOL_ITEM(RVAL2GOBJ(item)), 
                                        NUM2INT(index));
    return self;
}

static VALUE
rg_relief_style(VALUE self)
{
    return GENUM2RVAL(gtk_toolbar_get_relief_style(_SELF(self)), GTK_TYPE_RELIEF_STYLE);
}
#endif

static VALUE
rg_append_space(VALUE self)
{
    gtk_toolbar_append_space(_SELF(self));
    return self;
}

static VALUE
rg_prepend_space(VALUE self)
{
    gtk_toolbar_prepend_space(_SELF(self));
    return self;
}

static VALUE
rg_insert_space(VALUE self, VALUE pos)
{
    gtk_toolbar_insert_space(_SELF(self), NUM2INT(pos));
    return self;
}

static VALUE
rg_remove_space(VALUE self, VALUE position)
{
    gtk_toolbar_remove_space(_SELF(self), NUM2INT(position));
    return self;
}

static VALUE
rg_unset_icon_size(VALUE self)
{
    gtk_toolbar_unset_icon_size(_SELF(self));
    return self;
}

static VALUE
rg_unset_style(VALUE self)
{
    gtk_toolbar_unset_style(_SELF(self));
    return self;
}

void 
Init_gtk_toolbar(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOOLBAR, "Toolbar", mGtk);

    RG_DEF_METHOD(initialize, 0);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(item_index, 1);
    RG_DEF_METHOD(n_items, 0);
    RG_DEF_METHOD(nth_item, 1);
    RG_DEF_METHOD(drop_index, 2);
    RG_DEF_METHOD(set_drop_highlight_item, 2);
    RG_DEF_METHOD(relief_style, 0);
#endif
    RG_DEF_METHOD(append, -1);
    RG_DEF_METHOD(prepend, -1);
    RG_DEF_METHOD(insert, -1);
    RG_DEF_METHOD(append_space, 0);
    RG_DEF_METHOD(prepend_space, 0);
    RG_DEF_METHOD(insert_space, 1);
    RG_DEF_METHOD(remove_space, 1);
    RG_DEF_METHOD(unset_icon_size, 0);
    RG_DEF_METHOD(unset_style, 0);

    /* GtkToolbarChildType */
    G_DEF_CLASS(GTK_TYPE_TOOLBAR_CHILD_TYPE, "ChildType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_TOOLBAR_CHILD_TYPE, "GTK_TOOLBAR_");

    /* GtkToolbarSpaceStyle */
    G_DEF_CLASS(GTK_TYPE_TOOLBAR_SPACE_STYLE, "SpaceStyle", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_TOOLBAR_SPACE_STYLE, "GTK_TOOLBAR_");

    /* GtkToolbarStyle(from Standard Constants) */
    G_DEF_CLASS(GTK_TYPE_TOOLBAR_STYLE, "Style", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_TOOLBAR_STYLE, "GTK_TOOLBAR_");
}
