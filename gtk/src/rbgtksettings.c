/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksettings.c -

  $Author: mutoh $
  $Date: 2005/01/29 11:44:15 $

  Copyright (C) 2003,2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"

static VALUE prop_func_table;

static VALUE
settings_s_get_default(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_settings_get_default());
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
settings_s_get_for_screen(self, screen)
    VALUE self, screen;
{
    return GOBJ2RVAL(gtk_settings_get_for_screen(GDK_SCREEN(RVAL2GOBJ(screen))));
}
#endif

/*
 * Gtk::Settings.install_property(...) do |spec, str|
 *   #parse str
 *   new_val = ....   # parse value from string.
 *   new_val          # return new_val or nil if you can't convert the value.
 * end
 */
static gboolean
rc_property_parser(pspec, rc_string, property_value)
    const GParamSpec *pspec;
    const GString *rc_string;
    GValue *property_value;
{
    VALUE spec = GOBJ2RVAL((gpointer)pspec);
    VALUE func = rb_hash_aref(prop_func_table, spec);
    VALUE ret = rb_funcall(func, id_call, 2, spec, CSTR2RVAL(rc_string->str));
    if (NIL_P(ret)) {
        return FALSE;
    } else if (RTEST(ret)){
        rbgobj_rvalue_to_gvalue(ret, property_value);
        return TRUE;
    } else {
        rb_raise(rb_eArgError, "Gtk::Settings#install_property() block should return new value or nil");
    }
}

static VALUE
settings_s_install_property(self, spec)
    VALUE self, spec;
{
    GParamSpec* pspec = G_PARAM_SPEC(RVAL2GOBJ(spec));
    if (rb_block_given_p()){
        VALUE func = G_BLOCK_PROC();
        rb_hash_aset(prop_func_table, spec, func);
        gtk_settings_install_property_parser(pspec, (GtkRcPropertyParser)rc_property_parser);
    } else {
        gtk_settings_install_property(pspec);
    }
    return self;
}

static VALUE
settings_rc_property_parse_color(self, spec, str)
    VALUE self, spec, str;
{
    gboolean ret; 
    GValue prop = {0,};
    VALUE val = Qnil;
    GString* gstr = g_string_new(RVAL2CSTR(str));
    g_value_init(&prop, GDK_TYPE_COLOR);
    ret = gtk_rc_property_parse_color(G_PARAM_SPEC(RVAL2GOBJ(spec)), 
                                      gstr, &prop);
    if (ret) val = GVAL2RVAL(&prop);

    g_string_free(gstr, TRUE);
    g_value_unset(&prop);
    return val;
}

static VALUE
settings_rc_property_parse_enum(self, spec, str)
    VALUE self, spec, str;
{
    gboolean ret; 
    GValue prop = {0,};
    VALUE val = Qnil;
    GString* gstr = g_string_new(RVAL2CSTR(str));
    GParamSpec* gspec = G_PARAM_SPEC(RVAL2GOBJ(spec));
    g_value_init(&prop, gspec->value_type);
    ret = gtk_rc_property_parse_enum(gspec, gstr, &prop);

    if (ret) val = GVAL2RVAL(&prop);

    g_string_free(gstr, TRUE);
    g_value_unset(&prop);
    return val;
}

static VALUE
settings_rc_property_parse_flags(self, spec, str)
    VALUE self, spec, str;
{
    gboolean ret; 
    GValue prop = {0,};
    VALUE val = Qnil;
    GString* gstr = g_string_new(RVAL2CSTR(str));
    GParamSpec* gspec = G_PARAM_SPEC(RVAL2GOBJ(spec));
    g_value_init(&prop, gspec->value_type);
    ret = gtk_rc_property_parse_flags(gspec, gstr, &prop);

    if (ret) val = GVAL2RVAL(&prop);

    g_string_free(gstr, TRUE);
    g_value_unset(&prop);
    return val;
}

static VALUE
settings_rc_property_parse_requisition(self, spec, str)
    VALUE self, spec, str;
{
    gboolean ret; 
    GValue prop = {0,};
    VALUE val = Qnil;
    GString* gstr = g_string_new(RVAL2CSTR(str));
    g_value_init(&prop, GTK_TYPE_REQUISITION);
    ret = gtk_rc_property_parse_requisition(G_PARAM_SPEC(RVAL2GOBJ(spec)), 
                                            gstr, &prop);
    if (ret){
        GtkRequisition* req = g_value_get_boxed(&prop);
        val = rb_assoc_new(INT2NUM(req->width), INT2NUM(req->height));
    }
    g_string_free(gstr, TRUE);
    g_value_unset(&prop);
    return val;    
}

static VALUE
settings_rc_property_parse_border(self, spec, str)
    VALUE self, spec, str;
{
    gboolean ret; 
    GValue prop = {0,};
    VALUE val = Qnil;
    GString* gstr = g_string_new(RVAL2CSTR(str));
    g_value_init(&prop, GTK_TYPE_BORDER);
    ret = gtk_rc_property_parse_border(G_PARAM_SPEC(RVAL2GOBJ(spec)), 
                                      gstr, &prop);
    if (ret){
        GtkBorder* border = g_value_get_boxed(&prop);
        val = BOXED2RVAL(border, GTK_TYPE_BORDER);
    }
    g_string_free(gstr, TRUE);
    g_value_unset(&prop);
    return val;
}

static VALUE
settings_set_property_value(self, name, value, origin)
    VALUE self, name, value, origin;
{
    GtkSettingsValue svalue = { NULL, { 0, }, };
    g_value_init(&svalue.value, RVAL2GTYPE(value));
    rbgobj_rvalue_to_gvalue(value, &svalue.value);

    svalue.origin = RVAL2CSTR(origin);
    gtk_settings_set_property_value(GTK_SETTINGS(RVAL2GOBJ(self)), 
                                    RVAL2CSTR(name), &svalue);
    g_value_unset(&svalue.value);

    return self;
}

/* We don't need them, Use set_property_value() instead.
void        gtk_settings_set_string_property
                                            (GtkSettings *settings,
                                             const gchar *name,
                                             const gchar *v_string,
                                             const gchar *origin);
void        gtk_settings_set_long_property  (GtkSettings *settings,
                                             const gchar *name,
                                             glong v_long,
                                             const gchar *origin);
void        gtk_settings_set_double_property
                                            (GtkSettings *settings,
                                             const gchar *name,
                                             gdouble v_double,
                                             const gchar *origin);
*/


void 
Init_gtk_settings()
{
    VALUE gSettings = G_DEF_CLASS(GTK_TYPE_SETTINGS, "Settings", mGtk);

    rb_global_variable(&prop_func_table);
    prop_func_table = rb_hash_new();

    rb_define_singleton_method(gSettings, "default", settings_s_get_default, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_singleton_method(gSettings, "get_for_screen", settings_s_get_for_screen, 1);
#endif
    rb_define_singleton_method(gSettings, "install_property", settings_s_install_property, 1);

    rb_define_singleton_method(gSettings, "rc_property_parse_color", settings_rc_property_parse_color, 2);
    rb_define_singleton_method(gSettings, "rc_property_parse_enum", settings_rc_property_parse_enum, 2);
    rb_define_singleton_method(gSettings, "rc_property_parse_flags", settings_rc_property_parse_flags, 2);
    rb_define_singleton_method(gSettings, "rc_property_parse_requisition", settings_rc_property_parse_requisition, 2);
    rb_define_singleton_method(gSettings, "rc_property_parse_border", settings_rc_property_parse_border, 2);

    rb_define_method(gSettings, "set_property_value", settings_set_property_value, 3);
}
