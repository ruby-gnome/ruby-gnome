/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui-print-content-selector.c -

  $Author: ktou $
  $Date: 2005/10/10 01:45:36 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgpui.h"

#if LIBGNOMEPRINTUI_CHECK_VERSION(2, 12, 0)
#include <libgnomeprintui/gnome-print-content-selector.h>

static VALUE
gpui_pcs_new(VALUE self)
{
    RBGTK_INITIALIZE(self,
                     g_object_new(GNOME_TYPE_PRINT_CONTENT_SELECTOR, NULL));
    return Qnil;
}
#endif

void
Init_gnome_print_content_selector(VALUE mGnome)
{
#if LIBGNOMEPRINTUI_CHECK_VERSION(2, 12, 0)
    VALUE cPrintContentSelector =
        G_DEF_CLASS(GNOME_TYPE_PRINT_CONTENT_SELECTOR, "PrintContentSelector",
                    mGnome);
#endif
}
