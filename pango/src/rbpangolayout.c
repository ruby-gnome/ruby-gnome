/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangolayout.c -

  $Author: mutoh $
  $Date: 2005/02/13 17:31:33 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_LAYOUT(RVAL2GOBJ(self)))
#define RVAL2CONTEXT(v) (PANGO_CONTEXT(RVAL2GOBJ(v)))

static VALUE
layout_initialize(self, context)
    VALUE self, context;
{
    G_INITIALIZE(self, pango_layout_new(RVAL2CONTEXT(context)));
    return Qnil;
}

static VALUE
layout_copy(self)
    VALUE self;
{
    return GOBJ2RVAL(pango_layout_copy(_SELF(self)));
}

static VALUE
layout_get_context(self)
    VALUE self;
{
    return GOBJ2RVAL(pango_layout_get_context(_SELF(self)));
}

static VALUE
layout_context_changed(self)
    VALUE self;
{
    pango_layout_context_changed(_SELF(self));
    return Qnil;
}

static VALUE
layout_set_text(self, text)
    VALUE self, text;
{
    pango_layout_set_text(_SELF(self), RVAL2CSTR(text), RSTRING(text)->len);
    return self;
}

static VALUE
layout_get_text(self)
    VALUE self;
{
    return CSTR2RVAL(pango_layout_get_text(_SELF(self)));
}

static VALUE
layout_set_markup(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE markup, accel_marker;
    gunichar accel_char = 0;

    rb_scan_args(argc, argv, "11", &markup, &accel_marker);

    if (NIL_P(accel_marker)){
        pango_layout_set_markup(_SELF(self), RVAL2CSTR(markup), RSTRING(markup)->len);
    } else {
        pango_layout_set_markup_with_accel(_SELF(self), 
                                           RVAL2CSTR(markup), RSTRING(markup)->len,
                                           NUM2CHR(accel_marker), &accel_char);
    }
    return CHR2FIX(accel_char);
}

static VALUE
layout_set_attributes(self, attrs)
    VALUE self, attrs;
{
    pango_layout_set_attributes(_SELF(self), 
                                (PangoAttrList*)(RVAL2BOXED(attrs, PANGO_TYPE_ATTR_LIST)));
    return self;
}

static VALUE
layout_get_attributes(self)
    VALUE self;
{
    return BOXED2RVAL(pango_layout_get_attributes(_SELF(self)), PANGO_TYPE_ATTR_LIST);
}

static VALUE
layout_set_font_description(self, desc)
    VALUE self;
{
    pango_layout_set_font_description(_SELF(self), 
                                      (PangoFontDescription*)RVAL2BOXED(desc, PANGO_TYPE_FONT_DESCRIPTION));
    return self;
}

#ifdef HAVE_PANGO_LAYOUT_GET_FONT_DESCRIPTION
static VALUE
layout_get_font_description(self)
    VALUE self;
{
    const PangoFontDescription* desc = pango_layout_get_font_description(_SELF(self));
    return desc ? BOXED2RVAL((gpointer)desc, PANGO_TYPE_FONT_DESCRIPTION) : Qnil;
}
#endif

static VALUE
layout_set_width(self, width)
    VALUE self, width;
{
    pango_layout_set_width(_SELF(self), NUM2INT(width));
    return self;
}

static VALUE
layout_get_width(self)
    VALUE self;
{
    return INT2NUM(pango_layout_get_width(_SELF(self)));
}

static VALUE
layout_set_wrap(self, wrap)
    VALUE self, wrap;
{
    pango_layout_set_wrap(_SELF(self), RVAL2GENUM(wrap, PANGO_TYPE_WRAP_MODE));
    return self;
}

static VALUE
layout_get_wrap(self)
    VALUE self;
{
    return GENUM2RVAL(pango_layout_get_wrap(_SELF(self)), PANGO_TYPE_WRAP_MODE);
}

#ifdef HAVE_PANGO_LAYOUT_SET_ELLIPSIZE
static VALUE
layout_set_ellipsize(self, ellipsize)
    VALUE self, ellipsize;
{
    pango_layout_set_ellipsize(_SELF(self), RVAL2GENUM(ellipsize, 
                                                       PANGO_TYPE_ELLIPSIZE_MODE));
    return self;
}
static VALUE
layout_get_ellipsize(self)
    VALUE self;
{
    return GENUM2RVAL(pango_layout_get_ellipsize(_SELF(self)), PANGO_TYPE_ELLIPSIZE_MODE);
}
#endif

static VALUE
layout_set_indent(self, indent)
    VALUE self, indent;
{
    pango_layout_set_indent(_SELF(self), NUM2INT(indent));
    return self;
}

static VALUE
layout_get_indent(self)
    VALUE self;
{
    return INT2NUM(pango_layout_get_indent(_SELF(self)));
}

static VALUE
layout_get_spacing(self)
    VALUE self;
{
    return INT2NUM(pango_layout_get_spacing(_SELF(self)));
}

static VALUE
layout_set_spacing(self, spacing)
    VALUE self, spacing;
{
    pango_layout_set_spacing(_SELF(self), NUM2INT(spacing));
    return self;
}

static VALUE
layout_set_justify(self, justify)
    VALUE self, justify;
{
    pango_layout_set_justify(_SELF(self), RTEST(justify));
    return self;
}
static VALUE
layout_get_justify(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_layout_get_justify(_SELF(self)));
}

static VALUE
layout_set_auto_dir(self, auto_dir)
    VALUE self, auto_dir;
{
    pango_layout_set_auto_dir(_SELF(self), RTEST(auto_dir));
    return self;
}
static VALUE
layout_get_auto_dir(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_layout_get_auto_dir(_SELF(self)));
}

static VALUE
layout_set_alignment(self, align)
    VALUE self, align;
{
    pango_layout_set_alignment(_SELF(self), RVAL2GENUM(align, PANGO_TYPE_ALIGNMENT));
    return self;
}

static VALUE
layout_get_alignment(self)
    VALUE self;
{
    return GENUM2RVAL(pango_layout_get_alignment(_SELF(self)), PANGO_TYPE_ALIGNMENT);
}

static VALUE
layout_set_tabs(self, tabs)
    VALUE self;
{
    pango_layout_set_tabs(_SELF(self), 
                          (PangoTabArray*)RVAL2BOXED(self, PANGO_TYPE_TAB_ARRAY));
    return self;
}

static VALUE
layout_get_tabs(self)
    VALUE self;
{
    return BOXED2RVAL(pango_layout_get_tabs(_SELF(self)), PANGO_TYPE_TAB_ARRAY);
}

static VALUE
layout_set_single_paragraph_mode(self, setting)
    VALUE self, setting;
{
    pango_layout_set_single_paragraph_mode(_SELF(self), RTEST(setting));
    return self;
}

static VALUE
layout_get_single_paragraph_mode(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_layout_get_single_paragraph_mode(_SELF(self)));
}

static VALUE
layout_get_log_attrs(self)
    VALUE self;
{
    PangoLogAttr* attrs;
    gint i, n_attrs;
    VALUE ary;

    pango_layout_get_log_attrs(_SELF(self), &attrs, &n_attrs);

    ary = rb_ary_new();

    for (i = 0; i < n_attrs; i++) {
        rb_ary_assoc(ary, BOXED2RVAL(&attrs[i], PANGO_TYPE_LOG_ATTR));
    }
    g_free(attrs);

    return ary;
}

static VALUE
layout_xy_to_index(self, x, y)
    VALUE self, x, y;
{
    int index, trailing;
    gboolean ret = pango_layout_xy_to_index(_SELF(self), 
                                            NUM2INT(x), NUM2INT(y), 
                                            &index, &trailing);

    return rb_ary_new3(3, CBOOL2RVAL(ret), INT2NUM(index), INT2NUM(trailing));
}

static VALUE
layout_index_to_pos(self, index)
    VALUE self, index;
{
    PangoRectangle pos;
    pango_layout_index_to_pos(_SELF(self), NUM2INT(index), &pos);
    return BOXED2RVAL(&pos, PANGO_TYPE_RECTANGLE);
}

static VALUE
layout_get_cursor_pos(self, index)
    VALUE self, index;
{
    PangoRectangle strong_pos, weak_pos;
    pango_layout_get_cursor_pos(_SELF(self), NUM2INT(index), &strong_pos, &weak_pos);
    return rb_ary_new3(2, BOXED2RVAL(&strong_pos, PANGO_TYPE_RECTANGLE),
                       BOXED2RVAL(&weak_pos, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_move_cursor_visually(self, strong, old_index, old_trailing, direction)
    VALUE self, strong, old_index, old_trailing, direction;
{
    int new_index, new_trailing;
    pango_layout_move_cursor_visually(_SELF(self), RTEST(strong),
                                      NUM2INT(old_index), NUM2INT(old_trailing),
                                      NUM2INT(direction),
                                      &new_index, &new_trailing);
    return rb_ary_new3(2, INT2NUM(new_index), INT2NUM(new_trailing));
}

static VALUE
layout_get_extents(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE ink_rect, logical_rect;
    PangoRectangle rink, rlog;

    rb_scan_args(argc, argv, "02", &ink_rect, &logical_rect);

    if (NIL_P(ink_rect)){
        rink.x = 0;
        rink.y = 0;
        rink.width = 0;
        rink.height = 0;
    } else {
        PangoRectangle* rect = (PangoRectangle*)RVAL2BOXED(ink_rect, PANGO_TYPE_RECTANGLE);
        rink.x = rect->x;
        rink.y = rect->y;
        rink.width = rect->width;
        rink.height = rect->height;
    }
    if (NIL_P(logical_rect)){
        rlog.x = 0;
        rlog.y = 0;
        rlog.width = 0;
        rlog.height = 0;
    } else {
        PangoRectangle* rect = (PangoRectangle*)RVAL2BOXED(logical_rect, PANGO_TYPE_RECTANGLE);
        rlog.x = rect->x;
        rlog.y = rect->y;
        rlog.width = rect->width;
        rlog.height = rect->height;
    }   

    pango_layout_get_extents(_SELF(self), &rink, &rlog);

    return rb_assoc_new(BOXED2RVAL(&rink, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&rlog, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_extents(self)
    VALUE self;
{
    PangoRectangle rink = {0, 0, 0, 0};
    PangoRectangle rlog = {0, 0, 0, 0};

    pango_layout_get_extents(_SELF(self), &rink, &rlog);
    return rb_assoc_new(BOXED2RVAL(&rink, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&rlog, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_get_pixel_extents(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE ink_rect, logical_rect;
    PangoRectangle rink, rlog;

    rb_scan_args(argc, argv, "02", &ink_rect, &logical_rect);

    if (NIL_P(ink_rect)){
        rink.x = 0;
        rink.y = 0;
        rink.width = 0;
        rink.height = 0;
    } else {
        PangoRectangle* rect = (PangoRectangle*)RVAL2BOXED(ink_rect, PANGO_TYPE_RECTANGLE);
        rink.x = rect->x;
        rink.y = rect->y;
        rink.width = rect->width;
        rink.height = rect->height;
    }
    if (NIL_P(logical_rect)){
        rlog.x = 0;
        rlog.y = 0;
        rlog.width = 0;
        rlog.height = 0;
    } else {
        PangoRectangle* rect = (PangoRectangle*)RVAL2BOXED(logical_rect, PANGO_TYPE_RECTANGLE);
        rlog.x = rect->x;
        rlog.y = rect->y;
        rlog.width = rect->width;
        rlog.height = rect->height;
    }   

    pango_layout_get_pixel_extents(_SELF(self), &rink, &rlog);

    return rb_assoc_new(BOXED2RVAL(&rink, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&rlog, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_pixel_extents(self)
    VALUE self;
{
    PangoRectangle rink = {0, 0, 0, 0};
    PangoRectangle rlog = {0, 0, 0, 0};

    pango_layout_get_pixel_extents(_SELF(self), &rink, &rlog);
    return rb_assoc_new(BOXED2RVAL(&rink, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&rlog, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_get_size(self)
    VALUE self;
{
    int width, height;
    pango_layout_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
layout_get_pixel_size(self)
    VALUE self;
{
    int width, height;
    pango_layout_get_pixel_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
layout_get_line_count(self)
    VALUE self;
{
    return INT2NUM(pango_layout_get_line_count(_SELF(self)));
}

static VALUE
layout_get_line(self, line)
    VALUE self, line;
{
    return BOXED2RVAL(pango_layout_get_line(_SELF(self), NUM2INT(line)), 
                      PANGO_TYPE_LAYOUT_LINE);
}

static VALUE
layout_get_lines(self)
    VALUE self;
{
    return GSLIST2ARY2(pango_layout_get_lines(_SELF(self)), PANGO_TYPE_LAYOUT_LINE);
}


static VALUE
layout_get_iter(self)
    VALUE self;
{
    return BOXED2RVAL(pango_layout_get_iter(_SELF(self)), 
                      PANGO_TYPE_LAYOUT_ITER);
}

void
Init_pango_layout()
{
    VALUE pLayout = G_DEF_CLASS(PANGO_TYPE_LAYOUT, "Layout", mPango);

    rb_define_method(pLayout, "initialize", layout_initialize, 1);
    rb_define_method(pLayout, "copy", layout_copy, 0);
    rb_define_method(pLayout, "context", layout_get_context, 0);
    rb_define_method(pLayout, "context_changed", layout_context_changed, 0);
    rb_define_method(pLayout, "set_text", layout_set_text, 1);
    rb_define_method(pLayout, "text", layout_get_text, 0);
    rb_define_method(pLayout, "set_markup", layout_set_markup, -1);
    rb_define_method(pLayout, "set_attributes", layout_set_attributes, 1);
    rb_define_method(pLayout, "attributes", layout_get_attributes, 0);
    rb_define_method(pLayout, "set_font_description", layout_set_font_description, 1);
#ifdef HAVE_PANGO_LAYOUT_GET_FONT_DESCRIPTION
    rb_define_method(pLayout, "font_description", layout_get_font_description, 0);
#endif
    rb_define_method(pLayout, "set_width", layout_set_width, 1);
    rb_define_method(pLayout, "width", layout_get_width, 0);
    rb_define_method(pLayout, "set_wrap", layout_set_wrap, 1);
    rb_define_method(pLayout, "wrap", layout_get_wrap, 0);
#ifdef HAVE_PANGO_LAYOUT_SET_ELLIPSIZE
    rb_define_method(pLayout, "set_ellipsize", layout_set_ellipsize, 1);
    rb_define_method(pLayout, "ellipsize", layout_get_ellipsize, 0);
#endif
    rb_define_method(pLayout, "set_indent", layout_set_indent, 1);
    rb_define_method(pLayout, "indent", layout_get_indent, 0);
    rb_define_method(pLayout, "spacing", layout_get_spacing, 0);
    rb_define_method(pLayout, "set_spacing", layout_set_spacing, 1);
    rb_define_method(pLayout, "set_justify", layout_set_justify, 1);
    rb_define_method(pLayout, "justify?", layout_get_justify, 0);
    rb_define_method(pLayout, "set_auto_dir", layout_set_auto_dir, 1);
    rb_define_method(pLayout, "auto_dir?", layout_get_auto_dir, 0);
    rb_define_method(pLayout, "set_alignment", layout_set_alignment, 1);
    rb_define_method(pLayout, "alignment", layout_get_alignment, 0);
    rb_define_method(pLayout, "set_tabs", layout_set_tabs, 1);
    rb_define_method(pLayout, "tabs", layout_get_tabs, 0);
    rb_define_method(pLayout, "set_single_paragraph_mode", layout_set_single_paragraph_mode, 1);
    rb_define_method(pLayout, "single_paragraph_mode?", layout_get_single_paragraph_mode, 0);
    rb_define_method(pLayout, "log_attrs", layout_get_log_attrs, 0);
    rb_define_method(pLayout, "xy_to_index", layout_xy_to_index, 2);
    rb_define_method(pLayout, "index_to_pos", layout_index_to_pos, 1);
    rb_define_method(pLayout, "get_cursor_pos", layout_get_cursor_pos, 1);
    rb_define_method(pLayout, "move_cursor_visually", layout_move_cursor_visually, 4);
    rb_define_method(pLayout, "get_extents", layout_get_extents, -1);
    rb_define_method(pLayout, "extents", layout_extents, 0);
    rb_define_method(pLayout, "get_pixel_extents", layout_get_pixel_extents, -1);
    rb_define_method(pLayout, "pixel_extents", layout_pixel_extents, 0);
    rb_define_method(pLayout, "size", layout_get_size, 0);
    rb_define_method(pLayout, "pixel_size", layout_get_pixel_size, 0);
    rb_define_method(pLayout, "line_count", layout_get_line_count, 0);
    rb_define_method(pLayout, "get_line", layout_get_line, 1);
    rb_define_method(pLayout, "lines", layout_get_lines, 0);
    rb_define_method(pLayout, "iter", layout_get_iter, 0);

    G_DEF_SETTERS(pLayout);

    /* PangoWrapMode */
    G_DEF_CLASS(PANGO_TYPE_WRAP_MODE, "WrapMode", pLayout);
    G_DEF_CONSTANTS(pLayout, PANGO_TYPE_WRAP_MODE, "PANGO_");

    /* PangoAlignment */
    G_DEF_CLASS(PANGO_TYPE_ALIGNMENT, "Alignment", pLayout);
    G_DEF_CONSTANTS(pLayout, PANGO_TYPE_ALIGNMENT, "PANGO_");

#ifdef HAVE_PANGO_LAYOUT_SET_ELLIPSIZE
    /* PangoEllipsizeMode */
    G_DEF_CLASS(PANGO_TYPE_ELLIPSIZE_MODE, "EllipsizeMode", pLayout);
    G_DEF_CONSTANTS(pLayout, PANGO_TYPE_ELLIPSIZE_MODE, "PANGO_");
#endif
}
