/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbbonobo-ui-main.c -

  $Author: mutoh $
  $Date: 2002/12/22 14:27:43 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbbonoboui.h"

/*
static int
bonobo_x_error_handler (Display *display, XErrorEvent *error)
void
bonobo_setup_x_error_handler (void)
*/

static VALUE
rbonoboui_is_initialized(self)
    VALUE self;
{
    return bonobo_is_initialized() ? Qtrue : Qfalse;
}

/*
static void
do_low_level_init (void)
*/


/* compat
   Shouldn't we implement them?
gboolean
bonobo_ui_init (const gchar *app_name, const gchar *app_version,
		int *argc, char **argv)
gboolean
bonobo_ui_init_full (const gchar *app_name, const gchar *app_version,
		     int *argc, char **argv, CORBA_ORB orb,
		     PortableServer_POA poa, PortableServer_POAManager manager,
		     gboolean full_init)
*/


static VALUE
rbonoboui_main(self)
    VALUE self;
{
    bonobo_ui_main();
    return self;
}

static VALUE
rbonoboui_debug_shutdown(self)
    VALUE self;
{
    return INT2NUM(bonobo_ui_debug_shutdown());
}
/*
static void
libbonoboui_post_args_parse (GnomeProgram    *program,
			     GnomeModuleInfo *mod_info)
*/

static VALUE
rbonoboui_module_info_get(self)
    VALUE self;
{
    return BOXED2RVAL((gpointer)(libbonobo_ui_module_info_get()), GNOME_TYPE_MODULE_INFO);
}

void
Init_bonobo_ui_main()
{
    rb_define_module_function(mBonoboUI, "initialized?", rbonoboui_is_initialized, 0);
    rb_define_module_function(mBonoboUI, "main", rbonoboui_main, 0);
    rb_define_module_function(mBonoboUI, "debug_shutdown", rbonoboui_debug_shutdown, 0);
    rb_define_module_function(mBonoboUI, "module_info", rbonoboui_module_info_get, 0);

}
