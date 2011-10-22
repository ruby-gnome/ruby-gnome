/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
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

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(s) (GTK_PRINT_CONTEXT(RVAL2GOBJ(s)))

#  ifdef HAVE_RB_CAIRO_H
#include <rb_cairo.h>

/* Rendering */
static VALUE
pc_get_cairo_context(VALUE self)
{
    return CRCONTEXT2RVAL(gtk_print_context_get_cairo_context(_SELF(self)));
}
#  endif

static VALUE
pc_get_page_setup(VALUE self)
{
    return GOBJ2RVAL(gtk_print_context_get_page_setup(_SELF(self)));
}

static VALUE
pc_get_width(VALUE self)
{
    return rb_float_new(gtk_print_context_get_width(_SELF(self)));
}

static VALUE
pc_get_height(VALUE self)
{
    return rb_float_new(gtk_print_context_get_height(_SELF(self)));
}

static VALUE
pc_get_dpi_x(VALUE self)
{
    return rb_float_new(gtk_print_context_get_dpi_x(_SELF(self)));
}

static VALUE
pc_get_dpi_y(VALUE self)
{
    return rb_float_new(gtk_print_context_get_dpi_y(_SELF(self)));
}

/* Fonts */
static VALUE
pc_get_pango_fontmap(VALUE self)
{
    return GOBJ2RVAL(gtk_print_context_get_pango_fontmap(_SELF(self)));
}

static VALUE
pc_create_pango_context(VALUE self)
{
    return GOBJ2RVALU(gtk_print_context_create_pango_context(_SELF(self)));
}

static VALUE
pc_create_pango_layout(VALUE self)
{
    return GOBJ2RVALU(gtk_print_context_create_pango_layout(_SELF(self)));
}

/* Needed for preview implementations */
#  ifdef HAVE_RB_CAIRO_H
static VALUE
pc_set_cairo_context(VALUE self, VALUE cr, VALUE dpi_x, VALUE dpi_y)
{
    gtk_print_context_set_cairo_context(_SELF(self),
                                        RVAL2CRCONTEXT(cr),
                                        NUM2DBL(dpi_x),
                                        NUM2DBL(dpi_y));
    return self;
}
#  endif
#endif

void
Init_gtk_print_context(void)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE gPrintContext = G_DEF_CLASS(GTK_TYPE_PRINT_CONTEXT,
                                      "PrintContext", mGtk);

    /* Rendering */
#  ifdef HAVE_RB_CAIRO_H
    rb_define_method(gPrintContext, "cairo_context", pc_get_cairo_context, 0);
#  endif
    rb_define_method(gPrintContext, "page_setup", pc_get_page_setup, 0);
    rb_define_method(gPrintContext, "width", pc_get_width, 0);
    rb_define_method(gPrintContext, "height", pc_get_height, 0);
    rb_define_method(gPrintContext, "dpi_x", pc_get_dpi_x, 0);
    rb_define_method(gPrintContext, "dpi_y", pc_get_dpi_y, 0);

    /* Fonts */
    rb_define_method(gPrintContext, "pango_fontmap", pc_get_pango_fontmap, 0);
    rb_define_method(gPrintContext, "create_pango_context",
                     pc_create_pango_context, 0);
    rb_define_method(gPrintContext, "create_pango_layout",
                     pc_create_pango_layout, 0);

    /* Needed for preview implementations */
#  ifdef HAVE_RB_CAIRO_H
    rb_define_method(gPrintContext, "set_cairo_context",
                     pc_set_cairo_context, 3);
#  endif

    G_DEF_SETTERS(gPrintContext);
#endif
}
