/*
 * Copyright (C) 2005 Kouhei Sutou <kou@cozmixng.org>
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

#if LIBGNOMEPRINTUI_CHECK_VERSION(2, 12, 0)
#include <libgnomeprintui/gnome-print-content-selector.h>

static VALUE
gpui_pcs_new(VALUE self)
{
  RBGTK_INITIALIZE(self, g_object_new(GNOME_TYPE_PRINT_CONTENT_SELECTOR, NULL));
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
