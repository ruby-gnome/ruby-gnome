/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkcombo.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
combo_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_combo_new());
    return Qnil;
}

static VALUE
combo_val_in_list(self, val, ok)
    VALUE self, val, ok;
{
    gtk_combo_set_value_in_list(GTK_COMBO(get_widget(self)),
				RTEST(val), RTEST(ok));
    return self;
}

static VALUE
combo_use_arrows(self, val)
    VALUE self, val;
{
    gtk_combo_set_use_arrows(GTK_COMBO(get_widget(self)),
			     RTEST(val));
    return self;
}

static VALUE
combo_use_arrows_always(self, val)
    VALUE self, val;
{
    gtk_combo_set_use_arrows_always(GTK_COMBO(get_widget(self)),
                                    RTEST(val));
    return self;
}

static VALUE
combo_case_sensitive(self, val)
    VALUE self, val;
{
    gtk_combo_set_case_sensitive(GTK_COMBO(get_widget(self)),
				 RTEST(val));
    return self;
}

static VALUE
combo_item_string(self, item, val)
    VALUE self, item, val;
{
    gtk_combo_set_item_string(GTK_COMBO(get_widget(self)),
			      GTK_ITEM(get_widget(item)),
			      NIL_P(val)?NULL:STR2CSTR(val));
    return self;
}

static VALUE
combo_popdown_strings(self, ary)
    VALUE self, ary;
{
    int i;
    GList *glist = NULL;

    Check_Type(ary, T_ARRAY);
    for (i=0; i<RARRAY(ary)->len; i++) {
	/* check to avoid memory leak */
	STR2CSTR(RARRAY(ary)->ptr[i]);
    }
    for (i=0; i<RARRAY(ary)->len; i++) {
	glist = g_list_append(glist, STR2CSTR(RARRAY(ary)->ptr[i]));
    }

    gtk_combo_set_popdown_strings(GTK_COMBO(get_widget(self)), glist);
    return self;
}

static VALUE
combo_disable_activate(self)
    VALUE self;
{
    gtk_combo_disable_activate(GTK_COMBO(get_widget(self)));
    return self;
}

static VALUE
combo_entry(self)
    VALUE self;
{
    return make_widget(gEntry, GTK_COMBO(get_widget(self))->entry);
}

static VALUE
combo_button(self)
    VALUE self;
{
    return make_widget(gButton, GTK_COMBO(get_widget(self))->button);
}

static VALUE
combo_popup(self)
    VALUE self;
{
    return make_widget(gScrolledWin, GTK_COMBO(get_widget(self))->popup);
}

static VALUE
combo_popwin(self)
    VALUE self;
{
    return make_widget(gWindow, GTK_COMBO(get_widget(self))->popwin);
}

static VALUE
combo_list(self)
    VALUE self;
{
    return make_widget(gList, GTK_COMBO(get_widget(self))->list);
}

void Init_gtk_combo()
{
    static rbgtk_class_info cinfo;

    gCombo = rb_define_class_under(mGtk, "Combo", gHBox);
    cinfo.klass = gCombo;
    cinfo.gtype = GTK_TYPE_COMBO;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gCombo, "initialize", combo_initialize, 0);
    rb_define_method(gCombo, "set_value_in_list", combo_val_in_list, 2);
    rb_define_method(gCombo, "set_use_arrows", combo_use_arrows, 1);
    rb_define_method(gCombo, "set_use_arrows_always", combo_use_arrows_always, 1);
    rb_define_method(gCombo, "set_case_sensitive", combo_case_sensitive, 1);
    rb_define_method(gCombo, "set_item_string", combo_item_string, 2);
    rb_define_method(gCombo, "set_popdown_strings", combo_popdown_strings, 1);
    rb_define_method(gCombo, "disable_activate", combo_disable_activate, 0);

    rb_define_method(gCombo, "entry", combo_entry, 0);
    rb_define_method(gCombo, "button", combo_button, 0);
    rb_define_method(gCombo, "popup", combo_popup, 0);
    rb_define_method(gCombo, "popwin", combo_popwin, 0);
    rb_define_method(gCombo, "list", combo_list, 0);
}
