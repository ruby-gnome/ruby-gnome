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
#include "libgnomeprintui-enum-types.h"

#include <libgnomeprintui/gnome-print-dialog.h>

#define _SELF(self) (RVAL2GOBJ(self))
#define RVAL2GPDF(obj) (RVAL2GFLAGS(obj, GNOME_TYPE_PRINTUI_PRINT_DIALOG_FLAGS))
/* #define RVAL2GPDF(obj) (NUM2INT(obj)) */
#define GPRT2RVAL(obj) (GENUM2RVAL(obj, GNOME_TYPE_PRINTUI_PRINT_RANGE_TYPE))
#define GPDR2RVAL(obj) (GENUM2RVAL(obj, GNOME_TYPE_PRINTUI_PRINT_BUTTONS))

static VALUE
gpui_dialog_new(VALUE self, VALUE job, VALUE title, VALUE flags)
{
  G_INITIALIZE(self, gnome_print_dialog_new(RVAL2GOBJ(job),
                                            RVAL2CSTR(title),
                                            RVAL2GPDF(flags)));
  /* Is it good way??? */
  g_object_ref(_SELF(self));
  return Qnil;
}

static VALUE
gpui_dialog_run(VALUE self)
{
  return GPDR2RVAL(gtk_dialog_run(_SELF(self)));
}

static VALUE
gpui_dialog_get_copies(VALUE self)
{
  gint copies;
  gboolean collate;
  gnome_print_dialog_get_copies(_SELF(self), &copies, &collate);
  return rb_ary_new3(2, INT2NUM(copies), CBOOL2RVAL(collate));
}

static VALUE
gpui_dialog_set_copies(VALUE self, VALUE copies, VALUE collate)
{
  gnome_print_dialog_set_copies(_SELF(self), NUM2INT(copies), NUM2INT(collate));
  return Qnil;
}

static VALUE
gpui_dialog_get_config(VALUE self)
{
  return GOBJ2RVAL(gnome_print_dialog_get_config(_SELF(self)));
}

static VALUE
gpui_dialog_get_range(VALUE self)
{
  return GPRT2RVAL(gnome_print_dialog_get_range(_SELF(self)));
}

static VALUE
gpui_dialog_get_range_page(VALUE self)
{
  gint start, end;
  gnome_print_dialog_get_range_page(_SELF(self), &start, &end);
  return rb_ary_new3(2, INT2NUM(start), INT2NUM(end));
}


void
Init_gnome_print_dialog(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_DIALOG, "PrintDialog", mGnome);
  G_DEF_CLASS(GNOME_TYPE_PRINTUI_PRINT_RANGE_TYPE,
              "PrintRangeType", mGnome);
  G_DEF_CLASS(GNOME_TYPE_PRINTUI_PRINT_DIALOG_RANGE_FLAGS,
              "PrintDialogRangeFlags", mGnome);
  G_DEF_CLASS(GNOME_TYPE_PRINTUI_PRINT_DIALOG_FLAGS,
              "PrintDialogFlags", mGnome);
  G_DEF_CLASS(GNOME_TYPE_PRINTUI_PRINT_BUTTONS,
              "PrintDialogResponse", mGnome);
  
              
  rb_define_method(c, "initialize", gpui_dialog_new, 3);

  rb_define_method(c, "run", gpui_dialog_run, 0);
  rb_define_method(c, "copies", gpui_dialog_get_copies, 0);
  rb_define_method(c, "set_copies", gpui_dialog_set_copies, 1);
  rb_define_method(c, "config", gpui_dialog_get_config, 0);
  rb_define_method(c, "range", gpui_dialog_get_range, 0);
  rb_define_method(c, "range_page", gpui_dialog_get_range_page, 0);

  G_DEF_SETTERS(c);
}
