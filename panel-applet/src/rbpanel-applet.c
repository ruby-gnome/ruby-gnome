/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpanelapplet.c

  $Author: ggc $
  $Date: 2007/07/13 16:07:33 $

  Copyright (C) 2003,2004 Masao Mutoh
************************************************/

#include "rbgobject.h"
#include "rbgtk.h"

#ifdef _
#  undef _
#endif

#include <orbit/orbit.h>
#include <panel-applet.h>
#include <panel-applet-gconf.h>

#include <panel-applet-enums.h>

#include "rbpanelappletversion.h"

#define _SELF(s) (PANEL_APPLET(RVAL2GOBJ(s)))

static VALUE action_table;
static guint action_id = 0;

static VALUE
rbpanel_applet_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, panel_applet_new());
    return Qnil;
}

static VALUE
rbpanel_applet_get_orient(self)
    VALUE self;
{
    return INT2NUM(panel_applet_get_orient(_SELF(self)));
}

static VALUE
rbpanel_applet_get_size(self)
    VALUE self;
{
    return UINT2NUM(panel_applet_get_size(_SELF(self)));
}

static VALUE
rbpanel_applet_get_background(self)
    VALUE self;
{
    GdkColor color;
    GdkPixmap* pixmap;
    VALUE ret;
    PanelAppletBackgroundType type = panel_applet_get_background(_SELF(self),
                                                                 &color, &pixmap);
    if (type == PANEL_NO_BACKGROUND){
        ret = Qnil;
    } else if (type == PANEL_COLOR_BACKGROUND){
        ret = BOXED2RVAL(&color, GDK_TYPE_COLOR);
    } else {
        ret = GOBJ2RVAL(pixmap);
    }
    return rb_assoc_new(GENUM2RVAL(type, PANEL_TYPE_PANEL_APPLET_BACKGROUND_TYPE), ret);
}

static VALUE
rbpanel_applet_get_preferences_key(self)
    VALUE self;
{
    gchar* key = panel_applet_get_preferences_key(_SELF(self));
    VALUE ret = CSTR2RVAL(key);
    g_free(key);
    return ret;
}

static VALUE
rbpanel_applet_add_preferences(self, schema_dir)
    VALUE self, schema_dir;
{
    GError* error = NULL;
    
    panel_applet_add_preferences(_SELF(self), RVAL2CSTR(schema_dir),
                                 &error);
    if (error) RAISE_GERROR(error);

    return self;
}

static VALUE
rbpanel_applet_get_flags(self)
    VALUE self;
{
    return GFLAGS2RVAL(panel_applet_get_flags(_SELF(self)), PANEL_TYPE_PANEL_APPLET_FLAGS);
}

static VALUE
rbpanel_applet_set_flags(self, flags)
    VALUE self, flags;
{
    panel_applet_set_flags(_SELF(self), RVAL2GFLAGS(flags, PANEL_TYPE_PANEL_APPLET_FLAGS));
    return self;
}

static VALUE
rbpanel_applet_request_focus(self,timestamp)
    VALUE self, timestamp;
{
    panel_applet_request_focus(_SELF(self), NUM2UINT(timestamp));
    return self;
}

static VALUE
rbpanel_applet_set_size_hints(self, size_hints, base_size)
    VALUE self, size_hints, base_size;
{
	gint len = NIL_P(size_hints) ? 0 : RARRAY_LEN(size_hints);
	gint *hints = g_new0(gint, len);
	gint i;
	for(i = 0; i < len; i++) {
		hints[i] = NUM2INT(RARRAY_PTR(size_hints)[i]);
	}
	panel_applet_set_size_hints(_SELF(self), hints, len, NUM2INT(base_size));
	g_free(hints);
	return self;
}

static VALUE
rbpanel_applet_get_control(self)
    VALUE self;
{
    return GOBJ2RVAL(panel_applet_get_control(_SELF(self)));
}

static VALUE
rbpanel_applet_get_popup_component(self)
    VALUE self;
{
    return GOBJ2RVAL(panel_applet_get_popup_component(_SELF(self)));
}

/*
void        panel_applet_setup_menu_from_file
                                            (PanelApplet *applet,
                                             const gchar *opt_datadir,
                                             const gchar *file,
                                             const gchar *opt_app_name,
                                             const BonoboUIVerb *verb_list,
                                             gpointer user_data);
*/

static void menu_callback_wrap (uic, action_id, verbname)
    BonoboUIComponent *uic;
    guint action_id;
    const gchar *verbname;
{
    ID id_call = rb_intern("call");
    VALUE action = rb_hash_aref(action_table, UINT2NUM(action_id));
    
    if (!NIL_P(action)) {
        rb_funcall(action, id_call, 0);
    }
}

static void ui_verb_set_data (self, verb, name, proc)
    VALUE self;
    BonoboUIVerb *verb;
    VALUE name;
    VALUE proc;
{
    verb->cname = (const gchar *)g_strdup (RVAL2CSTR(name));
    verb->cb = (BonoboUIVerbFn)menu_callback_wrap;
    verb->user_data = GUINT_TO_POINTER (action_id);

    rb_hash_aset(action_table, UINT2NUM(action_id), proc);
    action_id++;
}

static VALUE
rbpanel_applet_set_menu(self, xml, verbs)
    VALUE self;
    VALUE xml;
    VALUE verbs;
{
    int i;
    int len;
    
    Check_Type(verbs, T_ARRAY);
    len = RARRAY_LEN(verbs);
    if (!len){
        bonobo_ui_component_rm (panel_applet_get_popup_component (_SELF(self)),
                                "/", NULL);
        return Qnil;
    }
    
    BonoboUIVerb *menu_verbs = g_new (BonoboUIVerb, len + 1);
    menu_verbs[len].cname = NULL;
    menu_verbs[len].cb = NULL;
    
    for (i = 0; i < len; i++) {
        Check_Type(RARRAY_PTR(verbs)[i], T_ARRAY);
        if (RARRAY_LEN(RARRAY_PTR(verbs)[i]) != 2)
        rb_raise(rb_eArgError, "invalid parameter length in verbs");
        ui_verb_set_data (self, &menu_verbs[i],
                          RARRAY_PTR(RARRAY_PTR(verbs)[i])[0],
                          RARRAY_PTR(RARRAY_PTR(verbs)[i])[1]);
    }
    
    panel_applet_setup_menu (_SELF(self),
                             RVAL2CSTR (xml),
                             menu_verbs,
                             NULL);
    return Qnil;
}

static VALUE
rbpanel_applet_gconf_get_string(self, key)
    VALUE self, key;
{
    GError* error = NULL;

    char* value = panel_applet_gconf_get_string(_SELF(self), RVAL2CSTR(key), &error);
    VALUE ret = CSTR2RVAL(value);
    g_free(value);

    if (error) RAISE_GERROR(error);

    return ret;
}

static VALUE
rbpanel_applet_gconf_get_int(self, key)
    VALUE self, key;
{
    GError* error = NULL;

    gint value = panel_applet_gconf_get_int(_SELF(self), RVAL2CSTR(key), &error);
    VALUE ret = INT2NUM(value);

    if (error) RAISE_GERROR(error);

    return ret;
}

static VALUE
rbpanel_applet_gconf_get_bool(self, key)
    VALUE self, key;
{
    GError* error = NULL;

    gboolean value = panel_applet_gconf_get_bool(_SELF(self), RVAL2CSTR(key), &error);
    VALUE ret = CBOOL2RVAL(value);

    if (error) RAISE_GERROR(error);

    return ret;
}

static VALUE
rbpanel_applet_gconf_get_float(self, key)
    VALUE self, key;
{
    GError* error = NULL;

    gdouble value = panel_applet_gconf_get_float(_SELF(self), RVAL2CSTR(key), &error);
    VALUE ret = rb_float_new(value);

    if (error) RAISE_GERROR(error);

    return ret;
}

static VALUE
rbpanel_applet_gconf_set_int(self, key, value)
    VALUE self, key, value;
{
    GError* error = NULL;

    panel_applet_gconf_set_int(_SELF(self), RVAL2CSTR(key), NUM2INT(value), &error);

    if (error) RAISE_GERROR(error);

    return self;
}

static VALUE
rbpanel_applet_gconf_set_bool(self, key, value)
    VALUE self, key, value;
{
    GError* error = NULL;

    panel_applet_gconf_set_bool(_SELF(self), RVAL2CSTR(key), RVAL2CBOOL(value), &error);

    if (error) RAISE_GERROR(error);

    return self;
}

static VALUE
rbpanel_applet_gconf_set_string(self, key, value)
    VALUE self, key, value;
{
    GError* error = NULL;

    panel_applet_gconf_set_string(_SELF(self), RVAL2CSTR(key), RVAL2CSTR(value), &error);

    if (error) RAISE_GERROR(error);

    return self;
}

static VALUE
rbpanel_applet_gconf_set_float(self, key, value)
    VALUE self, key, value;
{
    GError* error = NULL;

    panel_applet_gconf_set_float(_SELF(self), RVAL2CSTR(key), NUM2DBL(value), &error);

    if (error) RAISE_GERROR(error);

    return self;
}

static VALUE
rbpanel_s_need_gnome_p(VALUE self)
{
#ifdef LIBGNOMEUI_MODULE
    return Qtrue;
#else
    return Qfalse;
#endif
}

void
Init_panelapplet2()
{
    VALUE cApplet = G_DEF_CLASS(PANEL_TYPE_APPLET, "PanelApplet", rb_cObject);

    rb_define_method(cApplet, "initialize", rbpanel_applet_initialize, 0);

    rb_define_method(cApplet, "orient", rbpanel_applet_get_orient, 0);
    rb_define_method(cApplet, "size", rbpanel_applet_get_size, 0);
    rb_define_method(cApplet, "background", rbpanel_applet_get_background, 0);
    rb_define_method(cApplet, "preferences_key", rbpanel_applet_get_preferences_key, 0);
    rb_define_method(cApplet, "add_preferences", rbpanel_applet_add_preferences, 1);
    rb_define_method(cApplet, "flags", rbpanel_applet_get_flags, 0);
    rb_define_method(cApplet, "set_flags", rbpanel_applet_set_flags, 1);
    rb_define_method(cApplet, "request_focus", rbpanel_applet_request_focus, 1);
    rb_define_method(cApplet, "set_size_hints", rbpanel_applet_set_size_hints, 2);
    rb_define_method(cApplet, "control", rbpanel_applet_get_control, 0);
    rb_define_method(cApplet, "popup_component", rbpanel_applet_get_popup_component, 0);
    rb_define_method(cApplet, "set_menu", rbpanel_applet_set_menu, 2);
    rb_define_method(cApplet, "gconf_get_int", rbpanel_applet_gconf_get_int, 1);
    rb_define_method(cApplet, "gconf_get_string", rbpanel_applet_gconf_get_string, 1);
    rb_define_method(cApplet, "gconf_get_bool", rbpanel_applet_gconf_get_bool, 1);
    rb_define_method(cApplet, "gconf_get_float", rbpanel_applet_gconf_get_float, 1);
    rb_define_method(cApplet, "gconf_set_int", rbpanel_applet_gconf_set_int, 2);
    rb_define_method(cApplet, "gconf_set_string", rbpanel_applet_gconf_set_string, 2);
    rb_define_method(cApplet, "gconf_set_bool", rbpanel_applet_gconf_set_bool, 2);
    rb_define_method(cApplet, "gconf_set_float", rbpanel_applet_gconf_set_float, 2);

    rb_define_singleton_method(cApplet, "need_gnome?",
			       rbpanel_s_need_gnome_p, 0);

    G_DEF_SETTERS(cApplet);

    /* PanelAppletBackGroundTypeFlags */
    G_DEF_CLASS(PANEL_TYPE_PANEL_APPLET_BACKGROUND_TYPE, "BackgroundType", cApplet);
    G_DEF_CONSTANTS(cApplet, PANEL_TYPE_PANEL_APPLET_BACKGROUND_TYPE, "PANEL_");

    /* PanelAppletFlags */
    G_DEF_CLASS(PANEL_TYPE_PANEL_APPLET_FLAGS, "Flags", cApplet);
    G_DEF_CONSTANTS(cApplet, PANEL_TYPE_PANEL_APPLET_FLAGS, "PANEL_APPLET_");

    rb_define_const(cApplet, "ORIENT_UP", INT2NUM(PANEL_APPLET_ORIENT_UP));
    rb_define_const(cApplet, "ORIENT_DOWN", INT2NUM(PANEL_APPLET_ORIENT_DOWN));
    rb_define_const(cApplet, "ORIENT_LEFT", INT2NUM(PANEL_APPLET_ORIENT_LEFT));
    rb_define_const(cApplet, "ORIENT_RIGHT", INT2NUM(PANEL_APPLET_ORIENT_RIGHT));

    rb_define_const(cApplet, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(PANELAPPLET_MAJOR_VERSION),
                                INT2FIX(PANELAPPLET_MINOR_VERSION),
                                INT2FIX(PANELAPPLET_MICRO_VERSION)));

    action_table = rb_hash_new();
    rb_global_variable(&action_table);
}
