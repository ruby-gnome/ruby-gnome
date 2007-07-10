/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktoolbar.c -

  $Author: ggc $
  $Date: 2007/07/10 13:22:40 $

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_TOOLBAR(RVAL2GOBJ(self)))
#define N_RVAL2CSTR(text) (NIL_P(text) ? NULL : RVAL2CSTR(text))
#define N_RVAL2WIDGET(w)  (NIL_P(w) ? NULL : GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
tbar_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_toolbar_new());
    return Qnil;
}

static VALUE
tbar_append(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
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
tbar_prepend(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
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
tbar_insert(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
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
tbar_get_item_index(self, item)
    VALUE self, item;
{
    return INT2NUM(gtk_toolbar_get_item_index(_SELF(self), GTK_TOOL_ITEM(RVAL2GOBJ(item))));
}

static VALUE
tbar_get_n_items(self)
    VALUE self;
{
    return INT2NUM(gtk_toolbar_get_n_items(_SELF(self)));
}

static VALUE
tbar_get_nth_item(self, n)
    VALUE self, n;
{
    return GOBJ2RVAL(gtk_toolbar_get_nth_item(_SELF(self), NUM2INT(n)));
}

static VALUE
tbar_get_drop_index(self, x, y)
    VALUE self, x, y;
{
    return INT2NUM(gtk_toolbar_get_drop_index(_SELF(self), NUM2INT(x), NUM2INT(y)));
}

static VALUE
tbar_set_drop_highlight_item(self, item, index)
    VALUE self, item, index;
{
    gtk_toolbar_set_drop_highlight_item(_SELF(self), 
                                        GTK_TOOL_ITEM(RVAL2GOBJ(item)), 
                                        NUM2INT(index));
    return self;
}
/* Defined as Properties
void        gtk_toolbar_set_show_arrow      (GtkToolbar *toolbar,
                                             gboolean show_arrow);
gboolean    gtk_toolbar_get_show_arrow      (GtkToolbar *toolbar);
*/

static VALUE
tbar_get_relief_style(self)
    VALUE self;
{
    return GENUM2RVAL(gtk_toolbar_get_relief_style(_SELF(self)), GTK_TYPE_RELIEF_STYLE);
}
#endif

static VALUE
tbar_append_space(self)
    VALUE self;
{
    gtk_toolbar_append_space(_SELF(self));
    return self;
}

static VALUE
tbar_prepend_space(self)
    VALUE self;
{
    gtk_toolbar_prepend_space(_SELF(self));
    return self;
}

static VALUE
tbar_insert_space(self, pos)
    VALUE self, pos;
{
    gtk_toolbar_insert_space(_SELF(self), NUM2INT(pos));
    return self;
}

static VALUE
tbar_set_tooltips(self, enable)
    VALUE self, enable;
{
    gtk_toolbar_set_tooltips(_SELF(self), RTEST(enable));
    return self;
}

static VALUE
tbar_set_icon_size(self, size)
    VALUE self, size;
{
    gtk_toolbar_set_icon_size(_SELF(self), RVAL2GENUM(size, GTK_TYPE_ICON_SIZE));
    return self;
}

static VALUE
tbar_get_icon_size(self)
    VALUE self;
{
    return GENUM2RVAL(gtk_toolbar_get_icon_size(_SELF(self)), GTK_TYPE_ICON_SIZE);
}

static VALUE
tbar_get_tooltips(self)
    VALUE self;
{
    return gtk_toolbar_get_tooltips(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
tbar_remove_space(self, position)
    VALUE self, position;
{
    gtk_toolbar_remove_space(_SELF(self), NUM2INT(position));
    return self;
}

static VALUE
tbar_unset_icon_size(self)
    VALUE self;
{
    gtk_toolbar_unset_icon_size(_SELF(self));
    return self;
}

static VALUE
tbar_unset_style(self)
    VALUE self;
{
    gtk_toolbar_unset_style(_SELF(self));
    return self;
}

void 
Init_gtk_toolbar()
{
    VALUE gToolbar = G_DEF_CLASS(GTK_TYPE_TOOLBAR, "Toolbar", mGtk);

    rb_define_method(gToolbar, "initialize", tbar_initialize, 0);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gToolbar, "item_index", tbar_get_item_index, 1);
    rb_define_method(gToolbar, "n_items", tbar_get_n_items, 0);
    rb_define_method(gToolbar, "nth_item", tbar_get_nth_item, 1);
    rb_define_method(gToolbar, "drop_index", tbar_get_drop_index, 2);
    rb_define_method(gToolbar, "set_drop_highlight_item", tbar_set_drop_highlight_item, 2);
    rb_define_method(gToolbar, "relief_style", tbar_get_relief_style, 0);
#endif
    rb_define_method(gToolbar, "append", tbar_append, -1);
    rb_define_method(gToolbar, "prepend", tbar_prepend, -1);
    rb_define_method(gToolbar, "insert", tbar_insert, -1);
    rb_define_method(gToolbar, "append_space", tbar_append_space, 0);
    rb_define_method(gToolbar, "prepend_space", tbar_prepend_space, 0);
    rb_define_method(gToolbar, "insert_space", tbar_insert_space, 1);
    rb_define_method(gToolbar, "set_tooltips", tbar_set_tooltips, 1);
    rb_define_method(gToolbar, "set_icon_size", tbar_set_icon_size, 1);
    rb_define_method(gToolbar, "icon_size", tbar_get_icon_size, 0);
    rb_define_method(gToolbar, "tooltips?", tbar_get_tooltips, 0);
    rb_define_method(gToolbar, "remove_space", tbar_remove_space, 1);
    rb_define_method(gToolbar, "unset_icon_size", tbar_unset_icon_size, 0);
    rb_define_method(gToolbar, "unset_style", tbar_unset_style, 0);

    G_DEF_SETTERS(gToolbar);

    /* GtkToolbarChildType */
    G_DEF_CLASS(GTK_TYPE_TOOLBAR_CHILD_TYPE, "ChildType", gToolbar);
    G_DEF_CONSTANTS(gToolbar, GTK_TYPE_TOOLBAR_CHILD_TYPE, "GTK_TOOLBAR_");

    /* GtkToolbarSpaceStyle */
    G_DEF_CLASS(GTK_TYPE_TOOLBAR_SPACE_STYLE, "SpaceStyle", gToolbar);
    G_DEF_CONSTANTS(gToolbar, GTK_TYPE_TOOLBAR_SPACE_STYLE, "GTK_TOOLBAR_");

    /* GtkToolbarStyle(from Standard Constants) */
    G_DEF_CLASS(GTK_TYPE_TOOLBAR_STYLE, "Style", gToolbar);
    G_DEF_CONSTANTS(gToolbar, GTK_TYPE_TOOLBAR_STYLE, "GTK_TOOLBAR_");
}
