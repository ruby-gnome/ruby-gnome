/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktoolbar.c -

  $Author: mutoh $
  $Date: 2002/11/12 18:15:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_TOOLBAR(RVAL2GOBJ(self)))
#define N_RVAL2CSTR(text) (NIL_P(text) ? NULL : RVAL2CSTR(text))
#define N_RVAL2WIDGET(w)  (NIL_P(w) ? NULL : GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
tbar_get_gobject(widget, type)
    GtkWidget *widget;
    VALUE type;
{
    VALUE ret = Qnil;
    switch (NUM2INT(type)){
      case GTK_TOOLBAR_CHILD_SPACE:
        ret = Qnil;
        break;
      case GTK_TOOLBAR_CHILD_BUTTON:
      case GTK_TOOLBAR_CHILD_TOGGLEBUTTON:
      case GTK_TOOLBAR_CHILD_RADIOBUTTON:
      case GTK_TOOLBAR_CHILD_WIDGET:
        ret = GOBJ2RVAL(widget);
        break;
    }
    return ret;
}

static VALUE
tbar_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_toolbar_new());
    return Qnil;
}

static VALUE
tbar_append_item(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    GtkWidget *ret = NULL;
    VALUE text, ttext, ptext, icon, func;

    rb_scan_args(argc, argv, "05", &text, &ttext, &ptext, &icon, &func);
    if (NIL_P(func)) {
        func = rb_f_lambda();
    }
    G_RELATIVE(self, func);
    ret = gtk_toolbar_append_item(_SELF(self), N_RVAL2CSTR(text),
                                  N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                  N_RVAL2WIDGET(icon),
                                  GTK_SIGNAL_FUNC(exec_callback),
                                  (gpointer)func);
    return ret ? GOBJ2RVAL(ret) : Qnil;
}

static VALUE
tbar_prepend_item(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    GtkWidget *ret = NULL;
    VALUE text, ttext, ptext, icon, func;

    rb_scan_args(argc, argv, "05", &text, &ttext, &ptext, &icon, &func);

    if (NIL_P(func)) {
        func = rb_f_lambda();
    }
    G_RELATIVE(self, func);
    ret = gtk_toolbar_prepend_item(_SELF(self),N_RVAL2CSTR(text),
                                   N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                   N_RVAL2WIDGET(icon),
                                   GTK_SIGNAL_FUNC(exec_callback),
                                   (gpointer)func);
    return ret ? GOBJ2RVAL(ret) : Qnil;
}

static VALUE
tbar_insert_item(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    GtkWidget *ret = NULL;
    VALUE text, ttext, ptext, icon, func, pos;
    rb_scan_args(argc, argv, "15", &pos, &text, &ttext, &ptext, &icon, &func);

    if (NIL_P(func)) {
        func = rb_f_lambda();
    }
    G_RELATIVE(self, func);
    ret = gtk_toolbar_insert_item(_SELF(self),N_RVAL2CSTR(text),
                                  N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                  N_RVAL2WIDGET(icon),
                                  GTK_SIGNAL_FUNC(exec_callback),
                                  (gpointer)func,
                                  NUM2INT(pos));
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
tbar_append_element(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE type, widget, text, ttext, ptext, icon;
    VALUE func = (VALUE)NULL;
    GtkWidget *ret = NULL;

    rb_scan_args(argc, argv, "07", &type, &widget, &text, &ttext, &ptext, &icon, &func);
    if (NIL_P(func)) {
        func = rb_f_lambda();
    }
    G_RELATIVE(self, func);
    ret = gtk_toolbar_append_element(_SELF(self), NUM2INT(type), 
                                     N_RVAL2WIDGET(widget), N_RVAL2CSTR(text),
                                     N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                     N_RVAL2WIDGET(icon),
                                     GTK_SIGNAL_FUNC(exec_callback),
                                     (gpointer)func);
    return tbar_get_gobject(ret, type);
}

static VALUE
tbar_prepend_element(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE type, widget, text, ttext, ptext, icon;
    VALUE func = (VALUE)NULL;
    GtkWidget *ret = NULL;

    rb_scan_args(argc, argv, "07", &type, &widget, &text, &ttext, &ptext, &icon, &func);
    if (NIL_P(func)) {
        func = rb_f_lambda();
    }
    G_RELATIVE(self, func);
    ret = gtk_toolbar_prepend_element(_SELF(self),
                                      NUM2INT(type), N_RVAL2WIDGET(widget), 
                                      N_RVAL2CSTR(text), N_RVAL2CSTR(ttext),
                                      N_RVAL2CSTR(ptext), N_RVAL2WIDGET(icon),
                                      GTK_SIGNAL_FUNC(exec_callback),
                                      (gpointer)func);
    return tbar_get_gobject(ret, type);
}

static VALUE
tbar_insert_element(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE pos, type, widget, text, ttext, ptext, icon, func;
    GtkWidget *ret = NULL;

    rb_scan_args(argc, argv, "17", &pos, &type, &widget, &text, &ttext, &ptext, &icon, &func);
    if (NIL_P(func)) {
        func = rb_f_lambda();
    }
    G_RELATIVE(self, func);
    ret = gtk_toolbar_insert_element(_SELF(self),
                                     NUM2INT(type),N_RVAL2WIDGET(widget), 
                                     N_RVAL2CSTR(text), N_RVAL2CSTR(ttext),
                                     N_RVAL2CSTR(ptext), N_RVAL2WIDGET(icon),
                                     GTK_SIGNAL_FUNC(exec_callback),
                                     (gpointer)func,
                                     NUM2INT(pos));
    return tbar_get_gobject(ret, type);
}

static VALUE
tbar_append_widget(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE widget, ttext, ptext;
    rb_scan_args(argc, argv, "12", &widget, &ttext, &ptext);
    gtk_toolbar_append_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                              N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext));
    return self;
}

static VALUE
tbar_prepend_widget(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE widget, ttext, ptext;
    rb_scan_args(argc, argv, "12", &widget, &ttext, &ptext);
    gtk_toolbar_prepend_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                               N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext));
    return self;
}

static VALUE
tbar_insert_widget(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE pos, widget, ttext, ptext;
    rb_scan_args(argc, argv, "22", &pos, &widget, &ttext, &ptext);
    gtk_toolbar_insert_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                              N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                              NUM2INT(pos));
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
tbar_insert_stock(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE stock_id, pos, ttext, ptext, func;

    rb_scan_args(argc, argv, "23", &pos, &stock_id, &ttext, &ptext, &func);

    if (NIL_P(func)) {
        func = rb_f_lambda();
    }
    G_RELATIVE(self, func);

    return GOBJ2RVAL(gtk_toolbar_insert_stock(_SELF(self), RVAL2CSTR(stock_id),
                                              N_RVAL2CSTR(ttext), N_RVAL2CSTR(ptext),
                                              GTK_SIGNAL_FUNC(exec_callback),
                                              (gpointer)func,
                                              NUM2INT(pos)));
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
    rb_define_method(gToolbar, "append_item", tbar_append_item, -1);
    rb_define_method(gToolbar, "prepend_item", tbar_prepend_item, -1);
    rb_define_method(gToolbar, "insert_item", tbar_insert_item, -1);
    rb_define_method(gToolbar, "append_space", tbar_append_space, 0);
    rb_define_method(gToolbar, "prepend_space", tbar_prepend_space, 0);
    rb_define_method(gToolbar, "insert_space", tbar_insert_space, 1);
    rb_define_method(gToolbar, "append_widget", tbar_append_widget, -1);
    rb_define_method(gToolbar, "prepend_widget", tbar_prepend_widget, -1);
    rb_define_method(gToolbar, "insert_widget", tbar_insert_widget, -1);
    rb_define_method(gToolbar, "append_element", tbar_append_element, -1);
    rb_define_method(gToolbar, "prepend_element", tbar_prepend_element, -1);
    rb_define_method(gToolbar, "insert_element", tbar_insert_element, -1);
    rb_define_method(gToolbar, "set_tooltips", tbar_set_tooltips, 1);
    rb_define_method(gToolbar, "insert_stock", tbar_insert_stock, -1);
    rb_define_method(gToolbar, "set_icon_size", tbar_set_icon_size, 1);
    rb_define_method(gToolbar, "icon_size", tbar_get_icon_size, 0);
    rb_define_method(gToolbar, "tooltips?", tbar_get_tooltips, 0);
    rb_define_method(gToolbar, "remove_space", tbar_remove_space, 1);
    rb_define_method(gToolbar, "unset_iconsize", tbar_unset_icon_size, 0);
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
