/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui-print-paper-selector.c -

  $Author: ktou $
  $Date: 2007/05/26 02:23:08 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgpui.h"

#include <gtk/gtkwidget.h>
#include <libgnomeprintui/gnome-print-paper-selector.h>

#define _SELF(self) (RVAL2GOBJ(self))
#define RVAL2GPSF(obj) (RVAL2GENUM(obj, GNOME_TYPE_PRINT_PAPER_SELECTOR_FLAGS))

static VALUE
gpui_paper_selector_new(int argc, VALUE *argv, VALUE self)
{
    VALUE config, flags;
    GnomePrintConfig *pc;
    int n_args;
  
    n_args = rb_scan_args(argc, argv, "02", &config, &flags);

    if (NIL_P(config)) {
        pc = gnome_print_config_default();
    } else {
        pc = RVAL2GOBJ(config);
    }
  
    if (n_args == 2) {
        RBGTK_INITIALIZE(self,
                         gnome_paper_selector_new_with_flags(pc,
                                                             RVAL2GPSF(flags)));
    } else {
        RBGTK_INITIALIZE(self, gnome_paper_selector_new(pc));
    }
    return Qnil;
}

void
Init_gnome_paper_selector(VALUE mGnome)
{
    VALUE c = G_DEF_CLASS(GNOME_TYPE_PAPER_SELECTOR, "PaperSelector", mGnome);

    /* GnomePaperSelectorFlags */
    G_DEF_CLASS(GNOME_TYPE_PRINT_PAPER_SELECTOR_FLAGS, "Flags", c);
    G_DEF_CONSTANTS(c, GNOME_TYPE_PRINT_PAPER_SELECTOR_FLAGS,
                    "GNOME_PAPER_SELECTOR_");

    rb_define_method(c, "initialize", gpui_paper_selector_new, -1);
}
