/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpanelapplet.c

  $Author: mutoh $
  $Date: 2004/08/01 04:25:34 $

  Copyright (C) 2003,2004 Masao Mutoh
************************************************/

#include "rbgobject.h"
#include "rbgtk.h"
#include <panel-applet.h>
#include <panel-applet-enums.h>

static ID id_call;

#define _SELF(s) (PANEL_APPLET(RVAL2GOBJ(s)))

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

/*
void        panel_applet_set_size_hints     (PanelApplet *applet,
                                             const int *size_hints,
                                             int n_elements,
                                             int base_size);
*/

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
void        panel_applet_setup_menu         (PanelApplet *applet,
                                             const gchar *xml,
                                             const BonoboUIVerb *verb_list,
                                             gpointer user_data);
void        panel_applet_setup_menu_from_file
                                            (PanelApplet *applet,
                                             const gchar *opt_datadir,
                                             const gchar *file,
                                             const gchar *opt_app_name,
                                             const BonoboUIVerb *verb_list,
                                             gpointer user_data);
*/

static gboolean
rbpanel_cb(applet, iid, func)
    PanelApplet* applet;
    const gchar* iid;
    gpointer func;
{
    return RTEST(rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(applet), CSTR2RVAL(iid)));
}
   

static VALUE
rbpanel_s_main(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE func;
    VALUE iid, klass, name, version;
    GType gtype;
    int ret;
    int index;
    char **sys_argv_p;
    EXTERN VALUE rb_progname;

    if (!rb_block_given_p()){
        rb_raise( rb_eArgError, "PanelApplet.main requires a block" );
    }
    func = G_BLOCK_PROC();
    G_RELATIVE(self, func);

    if (argc > 3){
        rb_scan_args(argc, argv, "40", &iid, &klass, &name, &version);
        gtype = CLASS2GTYPE(klass);
    } else {
        rb_scan_args(argc, argv, "30", &iid, &name, &version);
        gtype = PANEL_TYPE_APPLET;
    }

    sys_argv_p = (char**)g_new0(char*, RARRAY(rb_argv)->len + 1);

    sys_argv_p[0] = RVAL2CSTR(rb_progname);
    for(index = 1; index <= RARRAY(rb_argv)->len; index++) {
        sys_argv_p[index] = RVAL2CSTR(RARRAY(rb_argv)->ptr[index - 1]);
    }

    gnome_program_init(RVAL2CSTR(name), RVAL2CSTR(version),
                        LIBGNOMEUI_MODULE,
                        RARRAY(rb_argv)->len + 1, sys_argv_p,
                        GNOME_CLIENT_PARAM_SM_CONNECT, FALSE,
                        GNOME_PARAM_NONE);
                        
    ret = INT2NUM(panel_applet_factory_main(STR2CSTR(iid), gtype, 
                                            (PanelAppletFactoryCallback)rbpanel_cb, 
                                            (void*)func));

    g_free( sys_argv_p );

    return ret;
}

/*
int         panel_applet_factory_main_closure
                                            (const gchar *iid,
                                             GType applet_type,
                                             GClosure *closure);

Bonobo_Unknown panel_applet_shlib_factory   (const char *iid,
                                             GType applet_type,
                                             PortableServer_POA poa,
                                             gpointer impl_ptr,
                                             PanelAppletFactoryCallback callback,
                                             gpointer user_data,
                                             CORBA_Environment *ev);
Bonobo_Unknown panel_applet_shlib_factory_closure
                                            (const char *iid,
                                             GType applet_type,
                                             PortableServer_POA poa,
                                             gpointer impl_ptr,
                                             GClosure *closure,
                                             CORBA_Environment *ev);
#define     PANEL_APPLET_BONOBO_SHLIB_FACTORY(iid, type, descr, callback, data)
*/

void
Init_panelapplet2()
{
    VALUE cApplet = G_DEF_CLASS(PANEL_TYPE_APPLET, "PanelApplet", rb_cObject);
    id_call = rb_intern("call");

    rb_define_method(cApplet, "initialize", rbpanel_applet_initialize, 0);

    rb_define_method(cApplet, "orient", rbpanel_applet_get_orient, 0);
    rb_define_method(cApplet, "size", rbpanel_applet_get_size, 0);
    rb_define_method(cApplet, "background", rbpanel_applet_get_background, 0);
    rb_define_method(cApplet, "preferences_key", rbpanel_applet_get_preferences_key, 0);
    rb_define_method(cApplet, "add_preferences", rbpanel_applet_add_preferences, 1);
    rb_define_method(cApplet, "flags", rbpanel_applet_get_flags, 0);
    rb_define_method(cApplet, "set_flags", rbpanel_applet_set_flags, 1);
    rb_define_method(cApplet, "control", rbpanel_applet_get_control, 0);
    rb_define_method(cApplet, "popup_component", rbpanel_applet_get_popup_component, 0);

    rb_define_singleton_method(cApplet, "main", rbpanel_s_main, -1);

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
    
}
