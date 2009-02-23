/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpanel-applet-main.c

  Copyright (C) 2003,2004 Masao Mutoh
  Copyright (C) 2008-2009 Ruby-GNOME2 Project Team
************************************************/

#include "rbgtk.h"

#ifdef _
#  undef _
#endif

#include <panel-applet.h>

#ifndef LIBGNOMEUI_MODULE
#  include <libbonobo.h>
#endif

static ID id_call;

static gboolean
rbpanel_cb(PanelApplet *applet, const gchar *iid, gpointer user_data)
{
    VALUE *func = user_data;
    return RVAL2CBOOL(rb_funcall(*func, id_call,
				 2, GOBJ2RVAL(applet), CSTR2RVAL(iid)));
}


static VALUE
rbpanel_s_main(int argc, VALUE *argv, VALUE self)
{
    VALUE func;
    VALUE iid, klass, name, version;
    GType gtype;
    int ret;
    int index;
    int sys_argc;
    char **sys_argv_p;

    if (!rb_block_given_p()) {
        rb_raise(rb_eArgError, "PanelApplet.main requires a block");
    }
    func = rb_block_proc();
    G_RELATIVE(self, func);

    if (argc > 3){
        rb_scan_args(argc, argv, "40", &iid, &klass, &name, &version);
        gtype = CLASS2GTYPE(klass);
    } else {
        rb_scan_args(argc, argv, "30", &iid, &name, &version);
        gtype = PANEL_TYPE_APPLET;
    }

    sys_argc = RARRAY_LEN(rb_argv) + 1;
    sys_argv_p = (char **)g_new0(char *, sys_argc);

    sys_argv_p[0] = RVAL2CSTR(rb_argv0);
    for(index = 1; index <= RARRAY_LEN(rb_argv); index++) {
        sys_argv_p[index] = RVAL2CSTR(RARRAY_PTR(rb_argv)[index - 1]);
    }

#ifdef LIBGNOMEUI_MODULE
    gnome_program_init(RVAL2CSTR(name), RVAL2CSTR(version),
		       LIBGNOMEUI_MODULE,
		       RARRAY_LEN(rb_argv) + 1, sys_argv_p,
		       GNOME_CLIENT_PARAM_SM_CONNECT, FALSE,
		       GNOME_PARAM_NONE);
#else
    bonobo_init(&sys_argc, sys_argv_p);
#endif

    ret = INT2NUM(panel_applet_factory_main(RVAL2CSTR(iid),
					    gtype,
                                            rbpanel_cb,
                                            (gpointer)&func));

    g_free(sys_argv_p);

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
Init_panelapplet2_main()
{
    VALUE cApplet;

    cApplet = rb_const_get(rb_cObject, rb_intern("PanelApplet"));
    id_call = rb_intern("call");

    rb_define_singleton_method(cApplet, "main", rbpanel_s_main, -1);
}
