/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpanelapplet.c

  $Author: mutoh $
  $Date: 2003/08/27 17:32:18 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbgobject.h"
#include "rbgtk.h"
#include <panel-applet.h>

static ID id_call;

/*
gboolean    (*PanelAppletFactoryCallback)   (PanelApplet *applet,
                                             const gchar *iid,
                                             gpointer user_data);
GtkWidget*  panel_applet_new                (void);
PanelAppletOrient panel_applet_get_orient   (PanelApplet *applet);
guint       panel_applet_get_size           (PanelApplet *applet);
PanelAppletBackgroundType panel_applet_get_background
                                            (PanelApplet *applet,
                                             GdkColor *color,
                                             GdkPixmap **pixmap);
gchar*      panel_applet_get_preferences_key
                                            (PanelApplet *applet);
void        panel_applet_add_preferences    (PanelApplet *applet,
                                             const gchar *schema_dir,
                                             GError **opt_error);
PanelAppletFlags panel_applet_get_flags     (PanelApplet *applet);
void        panel_applet_set_flags          (PanelApplet *applet,
                                             PanelAppletFlags flags);
void        panel_applet_set_size_hints     (PanelApplet *applet,
                                             const int *size_hints,
                                             int n_elements,
                                             int base_size);
BonoboControl* panel_applet_get_control     (PanelApplet *applet);
BonoboUIComponent* panel_applet_get_popup_component
                                            (PanelApplet *applet);
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
int         panel_applet_factory_main       (const gchar *iid,
                                             GType applet_type,
                                             PanelAppletFactoryCallback callback,
                                             gpointer data);
*/

static gboolean
rbpanel_cb(applet, iid, data)
    PanelApplet* applet;
    const gchar* iid;
    gpointer data;
{
    VALUE ret = rb_yield(rb_ary_new3(2, GOBJ2RVAL(applet), CSTR2RVAL(iid)));
    printf("aaa\n");
    g_object_unref(applet);
    return RTEST(ret);
}
   

static VALUE
rbpanel_s_main(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE iid, type, name, version;
    GType gtype;
    int ret;

    if (argc > 3){
        rb_scan_args(argc, argv, "40", &iid, &type, &name, &version);
        gtype = NUM2INT(type);
    } else {
        rb_scan_args(argc, argv, "30", &iid, &name, &version);
        gtype = PANEL_TYPE_APPLET;
    }

    ret = INT2NUM(panel_applet_factory_main(STR2CSTR(iid), gtype, 
                                            (PanelAppletFactoryCallback)rbpanel_cb, 
                                            NULL));
    printf("bbb\n");
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
    VALUE mGnome = rb_define_module("Gnome");
    VALUE cApplet = rb_define_class_under(mGnome, "PanelApplet", GTYPE2CLASS(PANEL_TYPE_APPLET));

    id_call = rb_intern("call");

    rb_define_singleton_method(cApplet, "main", rbpanel_s_main, -1);

//    rb_define_method(cAppletWidget, "queue_resize", rb_applet_widget_queue_resize, 0);

}
