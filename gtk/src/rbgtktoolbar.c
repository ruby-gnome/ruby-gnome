/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktoolbar.c -

  $Author: sakai $
  $Date: 2002/08/02 13:44:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
tbar_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GtkOrientation orientation = GTK_ORIENTATION_HORIZONTAL;
    GtkToolbarStyle style = GTK_TOOLBAR_BOTH;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);
    if (!NIL_P(arg1)) orientation = (GtkOrientation)NUM2INT(arg1);
    if (!NIL_P(arg2)) style = (GtkToolbarStyle)NUM2INT(arg2);

    {
        GtkWidget* w = gtk_toolbar_new();
        gtk_toolbar_set_orientation(GTK_TOOLBAR(w), orientation);
        gtk_toolbar_set_style(GTK_TOOLBAR(w), style);
        RBGTK_INITIALIZE(self, w);
    }    
    return Qnil;
}

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
tbar_append_item(self, text, ttext, ptext, icon, func)
    VALUE self, text, ttext, ptext, icon, func;
{
    GtkWidget *ret = NULL;

    if (NIL_P(func)) {
		func = rb_f_lambda();
    }
    add_relative(self, func);
    ret = gtk_toolbar_append_item(GTK_TOOLBAR(RVAL2GOBJ(self)),
								  NIL_P(text)?NULL:STR2CSTR(text),
								  NIL_P(ttext)?NULL:STR2CSTR(ttext),
								  NIL_P(ptext)?NULL:STR2CSTR(ptext),
								  NIL_P(icon)?NULL:GTK_WIDGET(RVAL2GOBJ(icon)),
								  GTK_SIGNAL_FUNC(exec_callback),
								  (gpointer)func);
    return ret ? GOBJ2RVAL(ret) : Qnil;
}

static VALUE
tbar_prepend_item(self, text, ttext, ptext, icon, func)
    VALUE self, text, ttext, ptext, icon, func;
{
    GtkWidget *ret = NULL;

    if (NIL_P(func)) {
		func = rb_f_lambda();
    }
    add_relative(self, func);
    ret = gtk_toolbar_prepend_item(GTK_TOOLBAR(RVAL2GOBJ(self)),
								   NIL_P(text)?NULL:STR2CSTR(text),
								   NIL_P(ttext)?NULL:STR2CSTR(ttext),
								   NIL_P(ptext)?NULL:STR2CSTR(ptext),
								   NIL_P(icon)?NULL:GTK_WIDGET(RVAL2GOBJ(icon)),
								   GTK_SIGNAL_FUNC(exec_callback),
								   (gpointer)func);
    return ret ? GOBJ2RVAL(ret) : Qnil;
}

static VALUE
tbar_insert_item(self, text, ttext, ptext, icon, func, pos)
    VALUE self, text, ttext, ptext, icon, func, pos;
{
    GtkWidget *ret = NULL;

    if (NIL_P(func)) {
		func = rb_f_lambda();
    }
    add_relative(self, func);
    ret = gtk_toolbar_insert_item(GTK_TOOLBAR(RVAL2GOBJ(self)),
								  NIL_P(text)?NULL:STR2CSTR(text),
								  NIL_P(ttext)?NULL:STR2CSTR(ttext),
								  NIL_P(ptext)?NULL:STR2CSTR(ptext),
								  NIL_P(icon)?NULL:GTK_WIDGET(RVAL2GOBJ(icon)),
								  GTK_SIGNAL_FUNC(exec_callback),
								  (gpointer)func,
								  NUM2INT(pos));
    return ret ? GOBJ2RVAL(ret) : Qnil;
}

static VALUE
tbar_append_space(self)
    VALUE self;
{
    gtk_toolbar_append_space(GTK_TOOLBAR(RVAL2GOBJ(self)));
    return self;
}

static VALUE
tbar_prepend_space(self)
    VALUE self;
{
    gtk_toolbar_prepend_space(GTK_TOOLBAR(RVAL2GOBJ(self)));
    return self;
}

static VALUE
tbar_insert_space(self, pos)
    VALUE self, pos;
{
    gtk_toolbar_insert_space(GTK_TOOLBAR(RVAL2GOBJ(self)), NUM2INT(pos));
    return self;
}

static VALUE
tbar_append_widget(self, widget, ttext, ptext)
    VALUE self, widget, ttext, ptext;
{
    gtk_toolbar_append_widget(GTK_TOOLBAR(RVAL2GOBJ(self)),
                              GTK_WIDGET(RVAL2GOBJ(widget)),
                              NIL_P(ttext)?NULL:STR2CSTR(ttext),
                              NIL_P(ptext)?NULL:STR2CSTR(ptext));
    return self;
}

static VALUE
tbar_prepend_widget(self, widget, ttext, ptext)
    VALUE self, widget, ttext, ptext;
{
    gtk_toolbar_prepend_widget(GTK_TOOLBAR(RVAL2GOBJ(self)),
							   GTK_WIDGET(RVAL2GOBJ(widget)),
							   NIL_P(ttext)?NULL:STR2CSTR(ttext),
							   NIL_P(ptext)?NULL:STR2CSTR(ptext));
    return self;
}

static VALUE
tbar_insert_widget(self, widget, ttext, ptext, pos)
    VALUE self, widget, ttext, ptext, pos;
{
    gtk_toolbar_insert_widget(GTK_TOOLBAR(RVAL2GOBJ(self)),
                              GTK_WIDGET(RVAL2GOBJ(widget)),
                              NIL_P(ttext)?NULL:STR2CSTR(ttext),
                              NIL_P(ptext)?NULL:STR2CSTR(ptext),
                              NUM2INT(pos));
    return self;
}

static VALUE
tbar_append_element(self, type, widget, text, ttext, ptext, icon)
    VALUE self, type, widget, text, ttext, ptext, icon;
{
    VALUE func = (VALUE)NULL;
    void *callback = NULL;
    GtkWidget *ret = NULL;

    if (rb_block_given_p()) {
		func = rb_f_lambda();
		add_relative(self, func);
		callback = exec_callback;
    }
    ret = gtk_toolbar_append_element(GTK_TOOLBAR(RVAL2GOBJ(self)),
									 NUM2INT(type),
									 NIL_P(widget)?NULL:GTK_WIDGET(RVAL2GOBJ(widget)),
									 NIL_P(text)?NULL:STR2CSTR(text),
									 NIL_P(ttext)?NULL:STR2CSTR(ttext),
									 NIL_P(ptext)?NULL:STR2CSTR(ptext),
									 NIL_P(icon)?NULL:GTK_WIDGET(RVAL2GOBJ(icon)),
									 GTK_SIGNAL_FUNC(callback),
									 (gpointer)func);
    return tbar_get_gobject(ret, type);
}

static VALUE
tbar_prepend_element(self, type, widget, text, ttext, ptext, icon)
    VALUE self, type, widget, text, ttext, ptext, icon;
{
    VALUE func = (VALUE)NULL;
    void *callback = NULL;
    GtkWidget *ret = NULL;

    if (rb_block_given_p()) {
		func = rb_f_lambda();
		add_relative(self, func);
		callback = exec_callback;
    }
    ret = gtk_toolbar_prepend_element(GTK_TOOLBAR(RVAL2GOBJ(self)),
									  NUM2INT(type),
									  NIL_P(widget)?NULL:GTK_WIDGET(RVAL2GOBJ(widget)),
									  NIL_P(text)?NULL:STR2CSTR(text),
									  NIL_P(ttext)?NULL:STR2CSTR(ttext),
									  NIL_P(ptext)?NULL:STR2CSTR(ptext),
									  NIL_P(icon)?NULL:GTK_WIDGET(RVAL2GOBJ(icon)),
									  GTK_SIGNAL_FUNC(callback),
									  (gpointer)func);
    return tbar_get_gobject(ret, type);
}

static VALUE
tbar_insert_element(self, type, widget, text, ttext, ptext, icon, position)
    VALUE self, type, widget, text, ttext, ptext, icon, position;
{
    VALUE func = (VALUE)NULL;
    void *callback = NULL;
    GtkWidget *ret = NULL;

    if (rb_block_given_p()) {
		func = rb_f_lambda();
		add_relative(self, func);
		callback = exec_callback;
    }
    ret = gtk_toolbar_insert_element(GTK_TOOLBAR(RVAL2GOBJ(self)),
									 NUM2INT(type),
									 NIL_P(widget)?NULL:GTK_WIDGET(RVAL2GOBJ(widget)),
									 NIL_P(text)?NULL:STR2CSTR(text),
									 NIL_P(ttext)?NULL:STR2CSTR(ttext),
									 NIL_P(ptext)?NULL:STR2CSTR(ptext),
									 NIL_P(icon)?NULL:GTK_WIDGET(RVAL2GOBJ(icon)),
									 GTK_SIGNAL_FUNC(callback),
									 (gpointer)func,
									 NUM2INT(position));
    return tbar_get_gobject(ret, type);
}

static VALUE
tbar_set_orientation(self, orientation)
    VALUE self, orientation;
{
    gtk_toolbar_set_orientation(GTK_TOOLBAR(RVAL2GOBJ(self)), 
								(GtkOrientation)NUM2INT(orientation));
    return self;
}

static VALUE
tbar_set_style(self, style)
    VALUE self, style;
{
    gtk_toolbar_set_style(GTK_TOOLBAR(RVAL2GOBJ(self)), 
						  (GtkToolbarStyle)NUM2INT(style));
    return self;
}

static VALUE
tbar_set_tooltips(self, enable)
    VALUE self, enable;
{
    gtk_toolbar_set_tooltips(GTK_TOOLBAR(RVAL2GOBJ(self)), RTEST(enable));
    return self;
}

void 
Init_gtk_toolbar()
{
    VALUE gToolbar = G_DEF_CLASS(GTK_TYPE_TOOLBAR, "Toolbar", mGtk);

    rb_define_const(gToolbar, "CHILD_SPACE", INT2NUM(GTK_TOOLBAR_CHILD_SPACE));
    rb_define_const(gToolbar, "CHILD_BUTTON", INT2NUM(GTK_TOOLBAR_CHILD_BUTTON));
    rb_define_const(gToolbar, "CHILD_TOGGLEBUTTON", INT2NUM(GTK_TOOLBAR_CHILD_TOGGLEBUTTON));
    rb_define_const(gToolbar, "CHILD_RADIOBUTTON", INT2NUM(GTK_TOOLBAR_CHILD_RADIOBUTTON));
    rb_define_const(gToolbar, "CHILD_WIDGET", INT2NUM(GTK_TOOLBAR_CHILD_WIDGET));
    rb_define_const(gToolbar, "SPACE_EMPTY", INT2FIX(GTK_TOOLBAR_SPACE_EMPTY));
    rb_define_const(gToolbar, "SPACE_LINE", INT2FIX(GTK_TOOLBAR_SPACE_LINE));
    rb_define_const(gToolbar, "ICONS", INT2FIX(GTK_TOOLBAR_ICONS));
    rb_define_const(gToolbar, "TEXT", INT2FIX(GTK_TOOLBAR_TEXT));
    rb_define_const(gToolbar, "BOTH", INT2FIX(GTK_TOOLBAR_BOTH));

    rb_define_method(gToolbar, "initialize", tbar_initialize, -1);
    rb_define_method(gToolbar, "append_item", tbar_append_item, 5);
    rb_define_method(gToolbar, "prepend_item", tbar_prepend_item, 5);
    rb_define_method(gToolbar, "insert_item", tbar_insert_item, 6);
    rb_define_method(gToolbar, "append_space", tbar_append_space, 0);
    rb_define_method(gToolbar, "prepend_space", tbar_prepend_space, 0);
    rb_define_method(gToolbar, "insert_space", tbar_insert_space, 1);
    rb_define_method(gToolbar, "append_widget", tbar_append_widget, 3);
    rb_define_method(gToolbar, "prepend_widget", tbar_prepend_widget, 3);
    rb_define_method(gToolbar, "insert_widget", tbar_insert_widget, 4);
    rb_define_method(gToolbar, "append_element", tbar_append_element, 6);
    rb_define_method(gToolbar, "prepend_element", tbar_prepend_element, 6);
    rb_define_method(gToolbar, "insert_element", tbar_insert_element, 7);
    rb_define_method(gToolbar, "set_orientation", tbar_set_orientation, 1);
    rb_define_method(gToolbar, "set_style", tbar_set_style, 1);
    rb_define_method(gToolbar, "set_tooltips", tbar_set_tooltips, 1);
}
