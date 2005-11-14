/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkpango.c -

  $Author: mutoh $
  $Date: 2005/11/14 07:04:40 $

  Copyright (C) 2003,2004 Masao Mutoh
************************************************/

#include "global.h"
#include "rbpango.h"

static VALUE
gdkpango_s_context_get(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE screen, ret;
    rb_scan_args(argc, argv, "01", &screen);
    if (NIL_P(screen)){
        ret = GOBJ2RVAL(gdk_pango_context_get());
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        ret = GOBJ2RVAL(gdk_pango_context_get_for_screen(GDK_SCREEN(RVAL2GOBJ(screen))));
#else
        rb_warn("Gdk::Pango.context_get: Not supported arguments in GTK+-2.0.x.");
        ret = GOBJ2RVAL(gdk_pango_context_get());
#endif
    }
    return ret;
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
    DATA_PTR(self) = gdk_pango_attr_embossed_new(RTEST(embossed));
    return Qnil;
}

static VALUE
gdkpango_attr_embossed_value(self)
    VALUE self;
{
    return CBOOL2RVAL(((GdkPangoAttrEmbossed*)RVAL2ATTR(self))->embossed);
}

static VALUE
gdkpango_attr_stipple_initialize(self, stipple)
    VALUE self, stipple;
{
    DATA_PTR(self) = gdk_pango_attr_stipple_new(GDK_BITMAP(RVAL2GOBJ(stipple)));
    return Qnil;
}

static VALUE
gdkpango_attr_stipple_value(self)
    VALUE self;
{
    return GOBJ2RVAL(((GdkPangoAttrStipple*)RVAL2ATTR(self))->stipple);
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
    VALUE pattrbool = ATTRTYPE2CLASS(CSTR2RVAL("AttrBool"));

    rb_define_module_function(mGdkPango, "context", gdkpango_s_context_get, -1);

    rb_define_method(context, "set_colormap", gdkpango_context_set_colormap, 1);
    G_DEF_SETTER(context, "colormap");
    rb_define_method(layout, "get_clip_region", gdkpango_layout_get_clip_region, 3);
    rb_define_method(layoutline, "get_clip_region", gdkpango_layout_line_get_clip_region, 3);

    klass = rb_define_class_under(mGdk, "PangoAttrEmbossed", pattrbool);
    rb_define_method(klass, "initialize", gdkpango_attr_embossed_initialize, 1);
    tmpattr = gdk_pango_attr_embossed_new(TRUE);
    rb_define_method(klass, "value", gdkpango_attr_embossed_value, 0);
    RBPANGO_ADD_ATTRIBUTE(tmpattr->klass->type, klass);

    klass = rb_define_class_under(mGdk, "PangoAttrStipple", pattr);
    rb_define_method(klass, "initialize", gdkpango_attr_stipple_initialize, 1);
    rb_define_method(klass, "value", gdkpango_attr_stipple_value, 0);
    tmpattr = gdk_pango_attr_stipple_new(NULL);
    RBPANGO_ADD_ATTRIBUTE(tmpattr->klass->type, klass);
}
