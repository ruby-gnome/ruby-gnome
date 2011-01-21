/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcestyle.c -

  $Author: mutoh $
  $Date: 2005/10/02 18:40:34 $

  Copyright (C) 2005  Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Module: Gtk::SourceStyle
 */

#define _SELF(self) (GTK_SOURCE_STYLE(RVAL2GOBJ(self)))

static VALUE
sourcestyle_copy (self)
	VALUE self;
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
	VALUE style = G_DEF_CLASS (GTK_TYPE_SOURCE_STYLE, "SourceStyle", mGtk);

	rb_define_method(style, "copy", sourcestyle_copy, 0);
}
