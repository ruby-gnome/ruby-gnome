/*
 * Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgpui.h"

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
  
  G_INITIALIZE(self, gnome_print_config_dialog_new(gpc));
  return Qnil;
}

void
Init_gnome_print_config_dialog(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_CONFIG_DIALOG,
                        "PrintConfigDialog", mGnome);

  rb_define_method(c, "initialize", gpui_config_dialog_new, -1);
}
