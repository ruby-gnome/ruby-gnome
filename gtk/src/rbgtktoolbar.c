/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktoolbar.c -

  $Author: mutoh $
  $Date: 2003/06/26 15:15:32 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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

    if (type == Qnil || TYPE(type) == T_STRING){
        rb_scan_args(argc, argv, "05", &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_append_item(_SELF(self), N_RVAL2CSTR(text),
                                      N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                      N_RVAL2WIDGET(icon),
                                      GTK_SIGNAL_FUNC(exec_callback),
                                      (gpointer)func);
    } else if (TYPE(type) == T_FIXNUM) {
        rb_scan_args(argc, argv, "07", &element_type, &widget, &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_append_element(_SELF(self), NUM2INT(element_type), 
                                         N_RVAL2WIDGET(widget), N_RVAL2CSTR(text),
                                         N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                         N_RVAL2WIDGET(icon),
                                         GTK_SIGNAL_FUNC(exec_callback),
                                         (gpointer)func);
        if (NUM2INT(element_type) == GTK_TOOLBAR_CHILD_SPACE) 
            ret = NULL;
    } else if (TYPE(type) == T_SYMBOL) {
        rb_scan_args(argc, argv, "13", &stock_id, &ttext, &ptext, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
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

    if (type == Qnil || TYPE(type) == T_STRING){
        rb_scan_args(argc, argv, "05", &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_prepend_item(_SELF(self), N_RVAL2CSTR(text),
                                      N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                      N_RVAL2WIDGET(icon),
                                      GTK_SIGNAL_FUNC(exec_callback),
                                      (gpointer)func);
    } else if (TYPE(type) == T_FIXNUM) {
        rb_scan_args(argc, argv, "07", &element_type, &widget, &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_prepend_element(_SELF(self), NUM2INT(element_type), 
                                         N_RVAL2WIDGET(widget), N_RVAL2CSTR(text),
                                         N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                         N_RVAL2WIDGET(icon),
                                         GTK_SIGNAL_FUNC(exec_callback),
                                         (gpointer)func);
        if (NUM2INT(element_type) == GTK_TOOLBAR_CHILD_SPACE) 
            ret = NULL;
    } else if (TYPE(type) == T_SYMBOL) {
        rb_scan_args(argc, argv, "13", &stock_id, &ttext, &ptext, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
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

    if (type == Qnil || TYPE(type) == T_STRING){
        rb_scan_args(argc, argv, "15", &pos, &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
        G_RELATIVE(self, func);
        ret = gtk_toolbar_insert_item(_SELF(self),N_RVAL2CSTR(text),
                                      N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                      N_RVAL2WIDGET(icon),
                                      GTK_SIGNAL_FUNC(exec_callback),
                                      (gpointer)func,
                                      NUM2INT(pos));
    } else if (TYPE(type) == T_FIXNUM) {
        rb_scan_args(argc, argv, "17", &pos, &element_type, &widget, &text, &ttext, &ptext, &icon, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
        G_RELATIVE(self, func);

        ret = gtk_toolbar_insert_element(_SELF(self),
                                         NUM2INT(element_type), N_RVAL2WIDGET(widget), 
                                         N_RVAL2CSTR(text), N_RVAL2CSTR(ttext),
                                         N_RVAL2CSTR(ptext), N_RVAL2WIDGET(icon),
                                         GTK_SIGNAL_FUNC(exec_callback),
                                         (gpointer)func,
                                         NUM2INT(pos));
        if (NUM2INT(element_type) == GTK_TOOLBAR_CHILD_SPACE) 
            ret = NULL;
    } else if (TYPE(type) == T_SYMBOL) {
        rb_scan_args(argc, argv, "14", &pos, &stock_id, &ttext, &ptext, &func);
        if (NIL_P(func)) func = G_BLOCK_PROC();
        G_RELATIVE(self, func);
        
        ret = gtk_toolbar_insert_stock(_SELF(self), rb_id2name(SYM2ID(stock_id)),
                                       N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                       GTK_SIGNAL_FUNC(exec_callback),
                                       (gpointer)func, NUM2INT(pos));
    } else {
        rb_scan_args(argc, argv, "22", &pos, &widget, &ttext, &ptext);
        gtk_toolbar_insert_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                                  N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                  NUM2INT(pos));
        return widget;
    }

    return ret ? GOBJ2RVAL(ret) : Qnil;
}

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
    gtk_toolbar_set_icon_size(_SELF(self), FIX2INT(size));
    return self;
}

static VALUE
tbar_get_icon_size(self)
    VALUE self;
{
    return INT2FIX(gtk_toolbar_get_icon_size(_SELF(self)));
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
    rb_define_const(gToolbar, "CHILD_SPACE", INT2FIX(GTK_TOOLBAR_CHILD_SPACE));
    rb_define_const(gToolbar, "CHILD_BUTTON", INT2FIX(GTK_TOOLBAR_CHILD_BUTTON));
    rb_define_const(gToolbar, "CHILD_TOGGLEBUTTON", INT2FIX(GTK_TOOLBAR_CHILD_TOGGLEBUTTON));
    rb_define_const(gToolbar, "CHILD_RADIOBUTTON", INT2FIX(GTK_TOOLBAR_CHILD_RADIOBUTTON));
    rb_define_const(gToolbar, "CHILD_WIDGET", INT2FIX(GTK_TOOLBAR_CHILD_WIDGET));

    /* GtkToolbarSpaceStyle */
    rb_define_const(gToolbar, "SPACE_EMPTY", INT2FIX(GTK_TOOLBAR_SPACE_EMPTY));
    rb_define_const(gToolbar, "SPACE_LINE", INT2FIX(GTK_TOOLBAR_SPACE_LINE));

    /* GtkToolbarStyle(from Standard Constants) */
    rb_define_const(gToolbar, "ICONS", INT2FIX(GTK_TOOLBAR_ICONS));
    rb_define_const(gToolbar, "TEXT", INT2FIX(GTK_TOOLBAR_TEXT));
    rb_define_const(gToolbar, "BOTH", INT2FIX(GTK_TOOLBAR_BOTH));
    rb_define_const(gToolbar, "BOTH_HORIZ", INT2FIX(GTK_TOOLBAR_BOTH_HORIZ));
}
