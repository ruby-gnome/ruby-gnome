/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkpango.c -

  $Author: mutoh $
  $Date: 2003/01/08 16:46:57 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"
#include "rbpango.h"

static VALUE
gdkpango_s_context_get(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pango_context_get());
}

static VALUE
gdkpango_context_set_colormap(self, colormap)
    VALUE self, colormap;
{
    gdk_pango_context_set_colormap(PANGO_CONTEXT(RVAL2GOBJ(self)),
                                   GDK_COLORMAP(RVAL2GOBJ(self)));
    return self;
}

static VALUE
gdkpango_attr_embossed_initialize(self, embossed)
    VALUE self, embossed;
{
    G_INITIALIZE(self, gdk_pango_attr_embossed_new(RTEST(embossed)));
    return Qnil;
}

static VALUE
gdkpango_attr_stipple_initialize(self, stipple)
    VALUE self, stipple;
{
    G_INITIALIZE(self, gdk_pango_attr_stipple_new(GDK_BITMAP(RVAL2GOBJ(stipple))));
    return Qnil;
}

static VALUE
gdkpango_layout_get_clip_region(self, x_origin, y_origin, index_ranges)
    VALUE self, x_origin, y_origin, index_ranges;
{
    int i;
    gint len = RARRAY(index_ranges)->len;
    gint* ranges = g_new(gint, len);

    for (i = 0; i < len; i++) {
        ranges[i] = RARRAY(index_ranges)->ptr[i];
    }
    return BOXED2RVAL(gdk_pango_layout_get_clip_region(PANGO_LAYOUT(RVAL2GOBJ(self)),
                                                       NUM2INT(x_origin),
                                                       NUM2INT(y_origin),
                                                       ranges, len), GDK_TYPE_REGION);
}

static VALUE
gdkpango_layout_line_get_clip_region(self, x_origin, y_origin, index_ranges)
    VALUE self, x_origin, y_origin, index_ranges;
{
    int i;
    gint len = RARRAY(index_ranges)->len;
    gint* ranges = g_new(gint, len);

    for (i = 0; i < len; i++) {
        ranges[i] = RARRAY(index_ranges)->ptr[i];
    }
    return BOXED2RVAL(gdk_pango_layout_line_get_clip_region(
                          (PangoLayoutLine*)RVAL2BOXED(self, PANGO_TYPE_LAYOUT_LINE),
                          NUM2INT(x_origin),
                          NUM2INT(y_origin),
                          ranges, len), GDK_TYPE_REGION);
}

void
Init_gtk_gdk_pango()
{
    VALUE klass;
    PangoAttribute* tmpattr;

    VALUE mGdkPango = rb_define_module_under(mGdk, "Pango");
    VALUE context = GTYPE2CLASS(PANGO_TYPE_CONTEXT);
    VALUE layout = GTYPE2CLASS(PANGO_TYPE_LAYOUT);
    VALUE layoutline = GTYPE2CLASS(PANGO_TYPE_LAYOUT_LINE);
    VALUE pattr = ATTRTYPE2CLASS(CSTR2RVAL("Attribute"));

    rb_define_module_function(mGdkPango, "context", gdkpango_s_context_get, 0);

    rb_define_method(context, "set_colormap", gdkpango_context_set_colormap, 1);
    G_DEF_SETTER(context, "set_colormap");
    rb_define_method(layout, "get_clip_region", gdkpango_layout_get_clip_region, 3);
    rb_define_method(layoutline, "get_clip_region", gdkpango_layout_line_get_clip_region, 3);

    klass = rb_define_class_under(mGdkPango, "AttrEmbossed", pattr);
    rb_define_method(klass, "initialize", gdkpango_attr_embossed_initialize, 1);
    tmpattr = gdk_pango_attr_embossed_new(TRUE);
    RBPANGO_ADD_ATTRIBUTE(tmpattr->klass->type, klass);

    klass = rb_define_class_under(mGdkPango, "AttrStipple", pattr);
    rb_define_method(klass, "initialize", gdkpango_attr_stipple_initialize, 1);
    tmpattr = gdk_pango_attr_stipple_new(NULL);
    RBPANGO_ADD_ATTRIBUTE(tmpattr->klass->type, klass);
}
