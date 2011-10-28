/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgtksourcemain.h"

/* Module: Gtk::SourceStyle
 */

#define RG_TARGET_NAMESPACE cSourceStyle
#define _SELF(self) (GTK_SOURCE_STYLE(RVAL2GOBJ(self)))

static VALUE
sourcestyle_copy(VALUE self)
{
	return GOBJ2RVAL (gtk_source_style_copy (_SELF (self)));
}

/* Defined as properties.
  "background"               gchar*                : Read / Write / Construct Only
  "background-set"           gboolean              : Read / Write / Construct Only
  "bold"                     gboolean              : Read / Write / Construct Only
  "bold-set"                 gboolean              : Read / Write / Construct Only
  "foreground"               gchar*                : Read / Write / Construct Only
  "foreground-set"           gboolean              : Read / Write / Construct Only
  "italic"                   gboolean              : Read / Write / Construct Only
  "italic-set"               gboolean              : Read / Write / Construct Only
  "line-background"          gchar*                : Read / Write / Construct Only
  "line-background-set"      gboolean              : Read / Write / Construct Only
  "strikethrough"            gboolean              : Read / Write / Construct Only
  "strikethrough-set"        gboolean              : Read / Write / Construct Only
  "underline"                gboolean              : Read / Write / Construct Only
  "underline-set"            gboolean              : Read / Write / Construct Only
*/

void
Init_gtk_sourcestyle ()
{
	VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS (GTK_TYPE_SOURCE_STYLE, "SourceStyle", mGtk);

	rb_define_method(RG_TARGET_NAMESPACE, "copy", sourcestyle_copy, 0);
}
