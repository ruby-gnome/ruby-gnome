/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksettings.c -

  $Author: mutoh $
  $Date: 2003/06/26 15:15:32 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"

static VALUE
settings_s_get_default(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_settings_get_default());
}

static VALUE
settings_s_install_property(self, pspec)
    VALUE self, pspec;
{
    gtk_settings_install_property(G_PARAM_SPEC(RVAL2GOBJ(pspec)));
    return pspec;
}

/* How can I implement them?
gboolean
_rc_property_parser(pspec, rc_string, property_value)
   const GParamSpec *pspec;
   const GString *rc_string;
   GValue *property_value;
{
   
}

static VALUE
settings_s_install_property_parser(pspec, parser)
   VALUE pspec, parser;
{
   volatile VALUE func = G_BLOCK_PROC();
   gtk_settings_install_property_parser(G_PARAM_SPEC(RVAL2GOBJ(pspec)), 
   (GtkRcPropertyParser)_rc_property_parser);
}
gboolean    gtk_rc_property_parse_color     (const GParamSpec *pspec,
                                             const GString *gstring,
                                             GValue *property_value);
gboolean    gtk_rc_property_parse_enum      (const GParamSpec *pspec,
                                             const GString *gstring,
                                             GValue *property_value);
gboolean    gtk_rc_property_parse_flags     (const GParamSpec *pspec,
                                             const GString *gstring,
                                             GValue *property_value);
gboolean    gtk_rc_property_parse_requisition
                                            (const GParamSpec *pspec,
                                             const GString *gstring,
                                             GValue *property_value);
gboolean    gtk_rc_property_parse_border    (const GParamSpec *pspec,
                                             const GString *gstring,
                                             GValue *property_value);
*/

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

    rb_define_singleton_method(gSettings, "default", settings_s_get_default, 0);
    rb_define_singleton_method(gSettings, "install_property", settings_s_install_property, 1);

    rb_define_method(gSettings, "set_property_value", settings_set_property_value, 3);
}
