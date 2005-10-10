/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui-print-config-dialog.c -

  $Author: ktou $
  $Date: 2005/10/10 01:45:36 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgpui.h"

#ifdef HAVE_LIBGNOMEPRINTUI_GNOME_PRINT_CONFIG_DIALOG_H
#include <libgnomeprintui/gnome-print-config-dialog.h>

#define _SELF(self) (RVAL2GOBJ(self))

static VALUE
gpui_config_dialog_new(int argc, VALUE *argv, VALUE self)
{
    VALUE config;
    GnomePrintConfig *gpc = NULL;
  
    rb_scan_args(argc, argv, "01", &config);

    if (!NIL_P(config)) {
        gpc = RVAL2GOBJ(config);
    }
  
    RBGTK_INITIALIZE(self, gnome_print_config_dialog_new(gpc));
    return Qnil;
}
#endif

void
Init_gnome_print_config_dialog(VALUE mGnome)
{
#ifdef HAVE_LIBGNOMEPRINTUI_GNOME_PRINT_CONFIG_DIALOG_H
    VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_CONFIG_DIALOG,
                          "PrintConfigDialog", mGnome);

    rb_define_method(c, "initialize", gpui_config_dialog_new, -1);
#endif
}
