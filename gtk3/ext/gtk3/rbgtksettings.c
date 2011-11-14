/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003,2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cSettings

static VALUE prop_func_table;

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gtk_settings_get_default());
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_s_get_for_screen(G_GNUC_UNUSED VALUE self, VALUE screen)
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
rc_property_parser(const GParamSpec *pspec, const GString *rc_string, GValue *property_value)
{
    VALUE spec = GOBJ2RVAL((gpointer)pspec);
    VALUE func = rb_hash_aref(prop_func_table, spec);
    VALUE ret = rb_funcall(func, id_call, 2, spec, CSTR2RVAL(rc_string->str));
    if (NIL_P(ret)) {
        return FALSE;
    } else if (RVAL2CBOOL(ret)){
        rbgobj_rvalue_to_gvalue(ret, property_value);
        return TRUE;
    } else {
        rb_raise(rb_eArgError, "Gtk::Settings#install_property() block should return new value or nil");
    }
}

static VALUE
rg_s_install_property(VALUE self, VALUE spec)
{
    GParamSpec* pspec = G_PARAM_SPEC(RVAL2GOBJ(spec));
    if (rb_block_given_p()){
        VALUE func = rb_block_proc();
        rb_hash_aset(prop_func_table, spec, func);
        gtk_settings_install_property_parser(pspec, (GtkRcPropertyParser)rc_property_parser);
    } else {
        gtk_settings_install_property(pspec);
    }
    return self;
}

static VALUE
settings_rc_property_parse(G_GNUC_UNUSED VALUE self, VALUE rbspec, VALUE rbstring, GtkRcPropertyParser parser)
{
    GParamSpec *spec = RVAL2GOBJ(rbspec);
    GString *string = g_string_new(RVAL2CSTR(rbstring));
    GValue property = G_VALUE_INIT;
    gboolean parsed;

    g_value_init(&property, spec->value_type);

    parsed = parser(spec, string, &property);

    g_string_free(string, TRUE);

    return parsed ? GVAL2RVAL_UNSET(&property) : Qnil;
}

static VALUE
rg_s_rc_property_parse_color(VALUE self, VALUE rbspec, VALUE rbstring)
{
    return settings_rc_property_parse(self, rbspec, rbstring, gtk_rc_property_parse_color);
}

static VALUE
rg_s_rc_property_parse_enum(VALUE self, VALUE rbspec, VALUE rbstring)
{
    return settings_rc_property_parse(self, rbspec, rbstring, gtk_rc_property_parse_enum);
}

static VALUE
rg_s_rc_property_parse_flags(VALUE self, VALUE rbspec, VALUE rbstring)
{
    return settings_rc_property_parse(self, rbspec, rbstring, gtk_rc_property_parse_flags);
}

static VALUE
rg_s_rc_property_parse_requisition(G_GNUC_UNUSED VALUE self, VALUE rbspec, VALUE rbstring)
{
    GParamSpec *spec = RVAL2GOBJ(rbspec);
    GString *string = g_string_new(RVAL2CSTR(rbstring));
    GValue property = G_VALUE_INIT;
    gboolean parsed;
    GtkRequisition *requisition;
    gint width;
    gint height;

    g_value_init(&property, GTK_TYPE_REQUISITION);

    parsed = gtk_rc_property_parse_requisition(spec, string, &property);

    g_string_free(string, TRUE);

    if (!parsed)
        return Qnil;

    requisition = g_value_get_boxed(&property);
    width = requisition->width;
    height = requisition->height;

    g_value_unset(&property);

    return rb_assoc_new(INT2NUM(width), INT2NUM(height));
}

static VALUE
settings_rc_property_parse_border_result(VALUE value)
{
    return BOXED2RVAL(g_value_get_boxed((GValue *)value), GTK_TYPE_BORDER);
}

static VALUE
settings_rc_property_parse_border_ensure(VALUE value)
{
    g_value_unset((GValue *)value);

    return Qnil;
}

static VALUE
rg_s_rc_property_parse_border(G_GNUC_UNUSED VALUE self, VALUE rbspec, VALUE rbstring)
{
    GParamSpec *spec = RVAL2GOBJ(rbspec);
    GString *string = g_string_new(RVAL2CSTR(rbstring));
    GValue property = G_VALUE_INIT;
    gboolean parsed;

    g_value_init(&property, GTK_TYPE_BORDER);

    parsed = gtk_rc_property_parse_border(spec, string, &property);

    g_string_free(string, TRUE);

    if (!parsed)
        return Qnil;

    return rb_ensure(settings_rc_property_parse_border_result, (VALUE)&property,
                     settings_rc_property_parse_border_ensure, (VALUE)&property);
}

static VALUE
rg_set_property_value(VALUE self, VALUE rbname, VALUE rbvalue, VALUE origin)
{
    GtkSettings *settings = GTK_SETTINGS(RVAL2GOBJ(self));
    GtkSettingsValue svalue = { (gchar *)RVAL2CSTR(origin), G_VALUE_INIT };
    const gchar *name = RVAL2CSTR(rbname);
    g_value_init(&svalue.value, RVAL2GTYPE(rbvalue));
    rbgobj_rvalue_to_gvalue(rbvalue, &svalue.value);

    gtk_settings_set_property_value(settings, name, &svalue);

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
Init_gtk_settings(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SETTINGS, "Settings", mGtk);

    rb_global_variable(&prop_func_table);
    prop_func_table = rb_hash_new();

    RG_DEF_SMETHOD(default, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_SMETHOD(get_for_screen, 1);
#endif
    RG_DEF_SMETHOD(install_property, 1);

    RG_DEF_SMETHOD(rc_property_parse_color, 2);
    RG_DEF_SMETHOD(rc_property_parse_enum, 2);
    RG_DEF_SMETHOD(rc_property_parse_flags, 2);
    RG_DEF_SMETHOD(rc_property_parse_requisition, 2);
    RG_DEF_SMETHOD(rc_property_parse_border, 2);

    RG_DEF_METHOD(set_property_value, 3);
}
