/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2017  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003,2004 Masao Mutoh
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
#include <rb-pango.h>

#define RG_TARGET_NAMESPACE mGdkPango

static VALUE
rg_m_context(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE screen, ret;
    rb_scan_args(argc, argv, "01", &screen);
    if (NIL_P(screen)){
        ret = GOBJ2RVAL(gdk_pango_context_get());
    } else {
        ret = GOBJ2RVAL(gdk_pango_context_get_for_screen(GDK_SCREEN(RVAL2GOBJ(screen))));
    }
    return ret;
}

static VALUE
gdkpango_context_set_colormap(VALUE self, VALUE colormap)
{
    gdk_pango_context_set_colormap(PANGO_CONTEXT(RVAL2GOBJ(self)),
                                   GDK_COLORMAP(RVAL2GOBJ(colormap)));
    return self;
}

static VALUE
gdkpango_attr_embossed_initialize(VALUE self, VALUE embossed)
{
    G_INITIALIZE(self, gdk_pango_attr_embossed_new(RVAL2CBOOL(embossed)));
    return Qnil;
}

static VALUE
gdkpango_attr_embossed_value(VALUE self)
{
    return CBOOL2RVAL(((GdkPangoAttrEmbossed*)RVAL2PANGOATTR(self))->embossed);
}

static VALUE
gdkpango_attr_stipple_initialize(VALUE self, VALUE stipple)
{
    G_INITIALIZE(self,
                 gdk_pango_attr_stipple_new(GDK_BITMAP(RVAL2GOBJ(stipple))));
    return Qnil;
}

static VALUE
gdkpango_attr_stipple_value(VALUE self)
{
    return GOBJ2RVAL(((GdkPangoAttrStipple*)RVAL2PANGOATTR(self))->stipple);
}

static VALUE
gdkpango_layout_get_clip_region(VALUE self, VALUE rbx_origin, VALUE rby_origin, VALUE rbindex_ranges)
{
    PangoLayout *layout = PANGO_LAYOUT(RVAL2GOBJ(self));
    gint x_origin = NUM2INT(rbx_origin);
    gint y_origin = NUM2INT(rby_origin);
    long n;
    gint *index_ranges = RVAL2GINTS(rbindex_ranges, n);
    GdkRegion *result;

    if (n % 2 != 0) {
        g_free(index_ranges);

        rb_raise(rb_eArgError,
                 "an even number of byte indexes must be given");
    }

    result = gdk_pango_layout_get_clip_region(layout, x_origin, y_origin, index_ranges, n / 2);

    g_free(index_ranges);

    return BOXED2RVAL(result, GDK_TYPE_REGION);
}

static VALUE
gdkpango_layout_line_get_clip_region(VALUE self, VALUE rbx_origin, VALUE rby_origin, VALUE rbindex_ranges)
{
    PangoLayoutLine *line = (PangoLayoutLine *)RVAL2BOXED(self, PANGO_TYPE_LAYOUT_LINE);
    gint x_origin = NUM2INT(rbx_origin);
    gint y_origin = NUM2INT(rby_origin);
    long n;
    gint *index_ranges = RVAL2GINTS(rbindex_ranges, n);
    GdkRegion *result;

    if (n % 2 != 0) {
        g_free(index_ranges);

        rb_raise(rb_eArgError,
                 "an even number of byte indexes must be given");
    }

    result = gdk_pango_layout_line_get_clip_region(line, x_origin, y_origin, index_ranges, n / 2);

    g_free(index_ranges);

    return BOXED2RVAL(result, GDK_TYPE_REGION);
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
gdkpango_attr_emboss_color_initialize(VALUE self, VALUE color)
{
    G_INITIALIZE(self, gdk_pango_attr_emboss_color_new(RVAL2GDKCOLOR(color)));
    return Qnil;
}

static VALUE
gdkpango_attr_emboss_color_value(VALUE self)
{
    return BOXED2RVAL(&(((GdkPangoAttrEmbossColor *)RVAL2PANGOATTR(self))->color),
                      PANGO_TYPE_COLOR);
}
#endif

void
Init_gtk_gdk_pango(VALUE mGdk)
{
    VALUE klass;
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGdk, "Pango");
    VALUE context = GTYPE2CLASS(PANGO_TYPE_CONTEXT);
    VALUE layout = GTYPE2CLASS(PANGO_TYPE_LAYOUT);
    VALUE layoutline = GTYPE2CLASS(PANGO_TYPE_LAYOUT_LINE);
    VALUE mPango = rb_const_get(rb_cObject, rb_intern("Pango"));
    VALUE pattr = rb_const_get(mPango, rb_intern("Attribute"));
    VALUE pattrbool = rb_const_get(mPango, rb_intern("AttrBool"));
    VALUE pattr_color = rb_const_get(mPango, rb_intern("AttrColor"));

    RG_DEF_MODFUNC(context, -1);

    rb_define_method(context, "set_colormap", gdkpango_context_set_colormap, 1);
    G_DEF_SETTER(context, "colormap");
    rb_define_method(layout, "get_clip_region", gdkpango_layout_get_clip_region, 3);
    rb_define_method(layoutline, "get_clip_region", gdkpango_layout_line_get_clip_region, 3);

    klass = rb_define_class_under(mGdk, "PangoAttrEmbossed", pattrbool);
    rb_define_method(klass, "initialize", gdkpango_attr_embossed_initialize, 1);
    rb_define_method(klass, "value", gdkpango_attr_embossed_value, 0);

    klass = rb_define_class_under(mGdk, "PangoAttrStipple", pattr);
    rb_define_method(klass, "initialize", gdkpango_attr_stipple_initialize, 1);
    rb_define_method(klass, "value", gdkpango_attr_stipple_value, 0);

#if GTK_CHECK_VERSION(2, 12, 0)
    klass = rb_define_class_under(mGdk, "PangoAttrEmbossColor", pattr_color);
    rb_define_method(klass, "initialize",
                     gdkpango_attr_emboss_color_initialize, 1);
    rb_define_method(klass, "value", gdkpango_attr_emboss_color_value, 0);
#endif
}
