/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkcolorsel.c -

  $Author: sakai $
  $Date: 2002/08/02 13:44:31 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * ColorSelection
 */

static VALUE
colorsel_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_color_selection_new());
    return Qnil;
}

static VALUE
colorsel_set_update_policy(self, policy)
    VALUE self, policy;
{
    gtk_color_selection_set_update_policy(GTK_COLOR_SELECTION(RVAL2GOBJ(self)),
										  (GtkUpdateType)NUM2INT(policy));
    return self;
}

static VALUE
colorsel_set_color(self, color)
    VALUE self, color;
{
    double buf[4];
    int arylen;
    GtkColorSelection *colorsel;

    Check_Type(color, T_ARRAY);
    colorsel = GTK_COLOR_SELECTION(RVAL2GOBJ(self));
    if (gtk_color_selection_get_has_opacity_control(colorsel)) {
		arylen = 4;
    } else {
		arylen = 3;
    }
    if (RARRAY(color)->len < arylen) {
		rb_raise(rb_eArgError, "color array too small");
    }
    buf[0] = NUM2DBL(RARRAY(color)->ptr[0]);
    buf[1] = NUM2DBL(RARRAY(color)->ptr[1]);
    buf[2] = NUM2DBL(RARRAY(color)->ptr[2]);
    if (arylen == 4) {
		buf[3] = NUM2DBL(RARRAY(color)->ptr[3]);
    }

    gtk_color_selection_set_color(colorsel, buf);
    return self;
}

static VALUE
colorsel_get_color(self)
    VALUE self;
{
    double buf[4];
    VALUE ary;
    int arylen;
    GtkColorSelection *colorsel;

    colorsel = GTK_COLOR_SELECTION(RVAL2GOBJ(self));
    gtk_color_selection_get_color(colorsel, buf);
    if (gtk_color_selection_get_has_opacity_control(colorsel)) {
		arylen = 4;
    } else {
		arylen = 3;
    }
    ary = rb_ary_new2(arylen);
    rb_ary_push(ary, rb_float_new(buf[0]));
    rb_ary_push(ary, rb_float_new(buf[1]));
    rb_ary_push(ary, rb_float_new(buf[2]));
    if (arylen == 4) {
		rb_ary_push(ary, rb_float_new(buf[3]));
    }
    return ary;
}

void 
Init_gtk_color_selection()
{
	VALUE gColorSel = G_DEF_CLASS(GTK_TYPE_COLOR_SELECTION, "ColorSelection", mGtk);

    rb_define_method(gColorSel, "initialize", colorsel_initialize, 0);
    rb_define_method(gColorSel, "set_update_policy", colorsel_set_update_policy, 1);
    rb_define_method(gColorSel, "set_color", colorsel_set_color, 1);
    rb_define_method(gColorSel, "get_color", colorsel_get_color, 0);
}

/*
 * ColorSelectionDialog
 */
static VALUE
cdialog_initialize(self, title)
    VALUE self, title;
{
    RBGTK_INITIALIZE(self, gtk_color_selection_dialog_new(STR2CSTR(title)));
    return Qnil;
}

static VALUE
cdialog_get_colorsel(self)
    VALUE self;
{
    VALUE sel = rb_iv_get(self, "colorsel");

    if (NIL_P(sel)) {
		GtkWidget *w = GTK_COLOR_SELECTION_DIALOG(RVAL2GOBJ(self))->colorsel;
		sel = GOBJ2RVAL(w);
		rb_iv_set(self, "colorsel", sel);
    }

    return sel;
}

static VALUE
cdialog_get_ok_button(self)
    VALUE self;
{
    VALUE b = rb_iv_get(self, "ok_button");

    if (NIL_P(b)) {
		GtkWidget *w = GTK_COLOR_SELECTION_DIALOG(RVAL2GOBJ(self))->ok_button;
		b = GOBJ2RVAL(w);
		rb_iv_set(self, "ok_button", b);
    }

    return b;
}

static VALUE
cdialog_get_cancel_button(self)
    VALUE self;
{
    VALUE b = rb_iv_get(self, "cancel_button");

    if (NIL_P(b)) {
		GtkWidget *w
			= GTK_COLOR_SELECTION_DIALOG(RVAL2GOBJ(self))->cancel_button;
		b = GOBJ2RVAL(w);
		rb_iv_set(self, "cancel_button", b);
    }

    return b;
}

static VALUE
cdialog_get_help_button(self)
    VALUE self;
{
    VALUE b = rb_iv_get(self, "help_button");

    if (NIL_P(b)) {
		GtkWidget *w
			= GTK_COLOR_SELECTION_DIALOG(RVAL2GOBJ(self))->help_button;
		b = GOBJ2RVAL(w);
		rb_iv_set(self, "help_button", b);
    }

    return b;
}

void 
Init_gtk_color_selection_dialog()
{
    VALUE gColorSelDialog = G_DEF_CLASS(GTK_TYPE_COLOR_SELECTION_DIALOG, 
										"ColorSelectionDialog", mGtk);

    rb_define_method(gColorSelDialog, "initialize", cdialog_initialize, 1);
    rb_define_method(gColorSelDialog, "colorsel", cdialog_get_colorsel, 0);
    rb_define_method(gColorSelDialog, "ok_button", cdialog_get_ok_button, 0);
    rb_define_method(gColorSelDialog, "cancel_button", cdialog_get_cancel_button, 0);
    rb_define_method(gColorSelDialog, "help_button", cdialog_get_help_button, 0);
}
