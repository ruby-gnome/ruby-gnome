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

#include "rbgtksourceview3private.h"

/* Module: Gtk::SourcePrintCompositor
 */

#define RG_TARGET_NAMESPACE cPrintCompositor
#define _SELF(self) (RVAL2GTKSOURCEPRINTCOMPOSITOR(self))

/*
 * Class method: new(buffer)
 * buffer: a Gtk::SourceBuffer or Gtk::SourceView object.
 *
 * Creates a new print compositor to print buffer.
 *
 * Returns: the new print compositor object.
 */
static VALUE
rg_initialize(VALUE self, VALUE val)
{
    if (rb_obj_is_kind_of (val, GTYPE2CLASS (GTK_SOURCE_TYPE_BUFFER))) {
       G_INITIALIZE(self,
           gtk_source_print_compositor_new (RVAL2GTKSOURCEBUFFER(val)));
    } else
        if (rb_obj_is_kind_of (val, GTYPE2CLASS (GTK_SOURCE_TYPE_VIEW))) {
            G_INITIALIZE(self,
                gtk_source_print_compositor_new_from_view (RVAL2GTKSOURCEVIEW(val)));
    } else {
        rb_raise (rb_eArgError,
            "invalid argument %s (expect Gtk::SourceBuffer or Gtk::SourceView)",
            rb_class2name (CLASS_OF (val)));
    }
    return Qnil;
}

static VALUE
rg_get_top_margin(VALUE self, VALUE unit)
{
    return DBL2NUM(gtk_source_print_compositor_get_top_margin(_SELF(self), RVAL2GTKUNIT (unit)));
}

static VALUE
rg_set_top_margin(VALUE self, VALUE top, VALUE unit)
{
    gtk_source_print_compositor_set_top_margin(_SELF(self), NUM2DBL(top), RVAL2GTKUNIT (unit));
    return self;
}

static VALUE
rg_get_bottom_margin(VALUE self, VALUE unit)
{
    return DBL2NUM(gtk_source_print_compositor_get_bottom_margin(_SELF(self), RVAL2GTKUNIT (unit)));
}

static VALUE
rg_set_bottom_margin(VALUE self, VALUE bottom, VALUE unit)
{
    gtk_source_print_compositor_set_bottom_margin(_SELF(self), NUM2DBL(bottom), RVAL2GTKUNIT (unit));
    return self;
}

static VALUE
rg_get_left_margin(VALUE self, VALUE unit)
{
    return DBL2NUM(gtk_source_print_compositor_get_left_margin(_SELF(self), RVAL2GTKUNIT (unit)));
}

static VALUE
rg_set_left_margin(VALUE self, VALUE left, VALUE unit)
{
    gtk_source_print_compositor_set_left_margin(_SELF(self), NUM2DBL(left), RVAL2GTKUNIT (unit));
    return self;
}

static VALUE
rg_get_right_margin(VALUE self, VALUE unit)
{
    return DBL2NUM(gtk_source_print_compositor_get_right_margin(_SELF(self), RVAL2GTKUNIT (unit)));
}

static VALUE
rg_set_right_margin(VALUE self, VALUE right, VALUE unit)
{
    gtk_source_print_compositor_set_right_margin(_SELF(self), NUM2DBL(right), RVAL2GTKUNIT (unit));
    return self;
}

static VALUE
rg_set_header_format(VALUE self, VALUE separator, VALUE left, VALUE center, VALUE right)
{
    gtk_source_print_compositor_set_header_format(_SELF(self),
                                           RVAL2CBOOL(separator),
                                           RVAL2CSTR(left),
                                           RVAL2CSTR(center),
                                           RVAL2CSTR(right));
    return self;
}

static VALUE
rg_set_footer_format(VALUE self, VALUE separator, VALUE left, VALUE center, VALUE right)
{
    gtk_source_print_compositor_set_footer_format(_SELF(self),
                                           RVAL2CBOOL(separator),
                                           RVAL2CSTR(left),
                                           RVAL2CSTR(center),
                                           RVAL2CSTR(right));
    return self;
}

static VALUE
rg_paginate(VALUE self, VALUE context)
{
    return CBOOL2RVAL (gtk_source_print_compositor_paginate (_SELF (self),
                                                             RVAL2GTKPRINTCONTEXT(context)));
}

static VALUE
rg_pagination_progress(VALUE self)
{
    return DBL2NUM (gtk_source_print_compositor_get_pagination_progress (_SELF (self)));
}

static VALUE
rg_draw_page(VALUE self, VALUE context, VALUE page_nr)
{
    gtk_source_print_compositor_draw_page (_SELF (self),
                                           RVAL2GTKPRINTCONTEXT(context),
                                           NUM2INT (page_nr));
    return self;
}

void
Init_gtksource_printcompositor(VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_SOURCE_TYPE_PRINT_COMPOSITOR, "PrintCompositor", mGtkSource);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(get_top_margin, 1);
    RG_DEF_METHOD(set_top_margin, 2);
    RG_DEF_METHOD(get_bottom_margin, 1);
    RG_DEF_METHOD(set_bottom_margin, 2);
    RG_DEF_METHOD(get_left_margin, 1);
    RG_DEF_METHOD(set_left_margin, 2);
    RG_DEF_METHOD(get_right_margin, 1);
    RG_DEF_METHOD(set_right_margin, 2);
    RG_DEF_METHOD(set_header_format, 4);
    RG_DEF_METHOD(set_footer_format, 4);
    RG_DEF_METHOD(paginate, 1);
    RG_DEF_METHOD(pagination_progress, 0);
    RG_DEF_METHOD(draw_page, 2);
}
