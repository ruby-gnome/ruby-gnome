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

#include <rbgp.h>

#include <gtk/gtkwidget.h>
#include <libgnomeprintui/gnome-print-unit-selector.h>

#define _SELF(self) (RVAL2GOBJ(self))

static VALUE
gpui_unit_selector_new(VALUE self, VALUE bases)
{
  RBGTK_INITIALIZE(self, gnome_print_unit_selector_new(RVAL2GPUB(bases)));
  return Qnil;
}

static VALUE
gpui_unit_selector_set_bases(VALUE self, VALUE bases)
{
  gnome_print_unit_selector_set_bases(_SELF(self), RVAL2GPUB(bases));
  return Qnil;
}

static VALUE
gpui_unit_selector_set_unit(VALUE self, VALUE unit)
{
  gnome_print_unit_selector_set_unit(_SELF(self), RVAL2GPU(unit));
  return Qnil;
}

static VALUE
gpui_unit_selector_get_unit(VALUE self)
{
  return CONST_GPU2RVAL(gnome_print_unit_selector_get_unit(_SELF(self)));
}

static VALUE
gpui_unit_selector_add_adjustment(VALUE self, VALUE adjustment)
{
  gnome_print_unit_selector_add_adjustment(_SELF(self), RVAL2GOBJ(adjustment));
  return Qnil;
}

static VALUE
gpui_unit_selector_remove_adjustment(VALUE self, VALUE adjustment)
{
  gnome_print_unit_selector_remove_adjustment(_SELF(self),
                                              RVAL2GOBJ(adjustment));
  return Qnil;
}


void
Init_gnome_print_unit_selector(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_UNIT_SELECTOR,
                        "PrintUnitSelector", mGnome);

  rb_define_method(c, "initialize", gpui_unit_selector_new, 1);

  rb_define_method(c, "set_bases", gpui_unit_selector_set_bases, 1);
  rb_define_method(c, "unit", gpui_unit_selector_get_unit, 0);
  rb_define_method(c, "set_unit", gpui_unit_selector_set_unit, 1);
  rb_define_method(c, "add_adjustment", gpui_unit_selector_add_adjustment, 1);
  rb_define_method(c, "remove_adjustment",
                   gpui_unit_selector_remove_adjustment, 1);

  G_DEF_SETTERS(c);
}
