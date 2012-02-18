/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#include "rbpangoprivate.h"

#define RG_TARGET_NAMESPACE cLayout
#define _SELF(self) (RVAL2PANGOLAYOUT(self))

static VALUE
rg_initialize(VALUE self, VALUE context)
{
    G_INITIALIZE(self, pango_layout_new(RVAL2PANGOCONTEXT(context)));
    return Qnil;
}

static VALUE
rg_copy(VALUE self)
{
    return GOBJ2RVAL_UNREF(pango_layout_copy(_SELF(self)));
}

static VALUE
rg_context(VALUE self)
{
    return GOBJ2RVAL(pango_layout_get_context(_SELF(self)));
}

static VALUE
rg_context_changed(VALUE self)
{
    pango_layout_context_changed(_SELF(self));
    return Qnil;
}

static VALUE
rg_set_text(VALUE self, VALUE text)
{
    StringValue(text);

    pango_layout_set_text(_SELF(self), RSTRING_PTR(text), RSTRING_LEN(text));

    return self;
}

static VALUE
rg_text(VALUE self)
{
    return CSTR2RVAL(pango_layout_get_text(_SELF(self)));
}

static VALUE
rg_set_markup(int argc, VALUE *argv, VALUE self)
{
    VALUE markup, accel_marker;
    gunichar accel_char = 0;

    rb_scan_args(argc, argv, "11", &markup, &accel_marker);

    StringValue(markup);

    if (NIL_P(accel_marker)){
        pango_layout_set_markup(_SELF(self),
                                RSTRING_PTR(markup),
                                RSTRING_LEN(markup));
    } else {
        pango_layout_set_markup_with_accel(_SELF(self),
                                           RSTRING_PTR(markup),
                                           RSTRING_LEN(markup),
                                           NUM2CHR(accel_marker), &accel_char);
    }
    return CHR2FIX(accel_char);
}

static VALUE
rg_operator_layout_set_markup_eq(VALUE self, VALUE markup)
{
    StringValue(markup);

    pango_layout_set_markup(_SELF(self), RSTRING_PTR(markup), RSTRING_LEN(markup));

    return markup;
}

static VALUE
rg_set_attributes(VALUE self, VALUE attrs)
{
    pango_layout_set_attributes(_SELF(self), 
                                RVAL2PANGOATTRLIST(attrs));
    return self;
}

static VALUE
rg_attributes(VALUE self)
{
    return PANGOATTRLIST2RVAL(pango_layout_get_attributes(_SELF(self)));
}

static VALUE
rg_set_font_description(VALUE self, VALUE rb_desc)
{
    PangoFontDescription *desc;
    gboolean desc_created = FALSE;

    if (RVAL2CBOOL(rb_obj_is_kind_of(rb_desc, rb_cString))) {
        desc = pango_font_description_from_string(RVAL2CSTR(rb_desc));
        desc_created = TRUE;
    } else {
        desc = RVAL2PANGOFONTDESCRIPTION(rb_desc);
    }

    pango_layout_set_font_description(_SELF(self), desc);

    if (desc_created)
        pango_font_description_free(desc);

    return self;
}

#ifdef HAVE_PANGO_LAYOUT_GET_FONT_DESCRIPTION
static VALUE
rg_font_description(VALUE self)
{
    const PangoFontDescription* desc = pango_layout_get_font_description(_SELF(self));
    return PANGOFONTDESCRIPTION2RVAL((gpointer)desc);
}
#endif

static VALUE
rg_set_width(VALUE self, VALUE width)
{
    pango_layout_set_width(_SELF(self), NUM2INT(width));
    return self;
}

static VALUE
rg_width(VALUE self)
{
    return INT2NUM(pango_layout_get_width(_SELF(self)));
}

static VALUE
rg_set_wrap(VALUE self, VALUE wrap)
{
    pango_layout_set_wrap(_SELF(self), RVAL2PANGOWRAPMODE(wrap));
    return self;
}

static VALUE
rg_wrap(VALUE self)
{
    return PANGOWRAPMODE2RVAL(pango_layout_get_wrap(_SELF(self)));
}

#ifdef HAVE_PANGO_LAYOUT_SET_ELLIPSIZE
static VALUE
rg_set_ellipsize(VALUE self, VALUE ellipsize)
{
    pango_layout_set_ellipsize(_SELF(self), RVAL2PANGOELLIPSIZEMODE(ellipsize));
    return self;
}
static VALUE
rg_ellipsize(VALUE self)
{
    return PANGOELLIPSIZEMODE2RVAL(pango_layout_get_ellipsize(_SELF(self)));
}
#endif

static VALUE
rg_set_indent(VALUE self, VALUE indent)
{
    pango_layout_set_indent(_SELF(self), NUM2INT(indent));
    return self;
}

static VALUE
rg_indent(VALUE self)
{
    return INT2NUM(pango_layout_get_indent(_SELF(self)));
}

static VALUE
rg_spacing(VALUE self)
{
    return INT2NUM(pango_layout_get_spacing(_SELF(self)));
}

static VALUE
rg_set_spacing(VALUE self, VALUE spacing)
{
    pango_layout_set_spacing(_SELF(self), NUM2INT(spacing));
    return self;
}

static VALUE
rg_set_justify(VALUE self, VALUE justify)
{
    pango_layout_set_justify(_SELF(self), RVAL2CBOOL(justify));
    return self;
}
static VALUE
rg_justify_p(VALUE self)
{
    return CBOOL2RVAL(pango_layout_get_justify(_SELF(self)));
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
rg_set_auto_dir(VALUE self, VALUE auto_dir)
{
    pango_layout_set_auto_dir(_SELF(self), RVAL2CBOOL(auto_dir));
    return self;
}
static VALUE
rg_auto_dir_p(VALUE self)
{
    return CBOOL2RVAL(pango_layout_get_auto_dir(_SELF(self)));
}
#endif

static VALUE
rg_set_alignment(VALUE self, VALUE align)
{
    pango_layout_set_alignment(_SELF(self), RVAL2PANGOALIGNMENT(align));
    return self;
}

static VALUE
rg_alignment(VALUE self)
{
    return PANGOALIGNMENT2RVAL(pango_layout_get_alignment(_SELF(self)));
}

static VALUE
rg_set_tabs(VALUE self, VALUE tabs)
{
    pango_layout_set_tabs(_SELF(self), 
                          RVAL2PANGOTABARRAY(tabs));
    return self;
}

static VALUE
rg_tabs(VALUE self)
{
    VALUE ret = Qnil;
    PangoTabArray* tabs = pango_layout_get_tabs(_SELF(self));

    if (tabs) {
        ret = PANGOTABARRAY2RVAL(tabs);
        pango_tab_array_free(tabs);
    }
    return ret;
}

static VALUE
rg_set_single_paragraph_mode(VALUE self, VALUE setting)
{
    pango_layout_set_single_paragraph_mode(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_single_paragraph_mode_p(VALUE self)
{
    return CBOOL2RVAL(pango_layout_get_single_paragraph_mode(_SELF(self)));
}

static VALUE
rg_log_attrs(VALUE self)
{
    PangoLogAttr* attrs;
    gint i, n_attrs;
    VALUE ary;

    pango_layout_get_log_attrs(_SELF(self), &attrs, &n_attrs);

    ary = rb_ary_new();

    for (i = 0; i < n_attrs; i++) {
        rb_ary_assoc(ary, PANGOLOGATTR2RVAL(&attrs[i]));
    }
    g_free(attrs);

    return ary;
}

static VALUE
rg_xy_to_index(VALUE self, VALUE x, VALUE y)
{
    int index, trailing;
    gboolean ret = pango_layout_xy_to_index(_SELF(self), 
                                            NUM2INT(x), NUM2INT(y), 
                                            &index, &trailing);

    return rb_ary_new3(3, CBOOL2RVAL(ret), INT2NUM(index), INT2NUM(trailing));
}

static VALUE
rg_index_to_pos(VALUE self, VALUE index)
{
    PangoRectangle pos;
    pango_layout_index_to_pos(_SELF(self), NUM2INT(index), &pos);
    return PANGORECTANGLE2RVAL(&pos);
}

static VALUE
rg_get_cursor_pos(VALUE self, VALUE index)
{
    PangoRectangle strong_pos, weak_pos;
    pango_layout_get_cursor_pos(_SELF(self), NUM2INT(index), &strong_pos, &weak_pos);
    return rb_ary_new3(2, PANGORECTANGLE2RVAL(&strong_pos),
                       PANGORECTANGLE2RVAL(&weak_pos));
}

static VALUE
rg_move_cursor_visually(VALUE self, VALUE strong, VALUE old_index, VALUE old_trailing, VALUE direction)
{
    int new_index, new_trailing;
    pango_layout_move_cursor_visually(_SELF(self), RVAL2CBOOL(strong),
                                      NUM2INT(old_index), NUM2INT(old_trailing),
                                      NUM2INT(direction),
                                      &new_index, &new_trailing);
    return rb_ary_new3(2, INT2NUM(new_index), INT2NUM(new_trailing));
}

static VALUE
rg_get_extents(int argc, VALUE *argv, VALUE self)
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
        PangoRectangle* rect = RVAL2PANGORECTANGLE(ink_rect);
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
        PangoRectangle* rect = RVAL2PANGORECTANGLE(logical_rect);
        rlog.x = rect->x;
        rlog.y = rect->y;
        rlog.width = rect->width;
        rlog.height = rect->height;
    }   

    pango_layout_get_extents(_SELF(self), &rink, &rlog);

    return rb_assoc_new(PANGORECTANGLE2RVAL(&rink),
                        PANGORECTANGLE2RVAL(&rlog));
}

static VALUE
rg_extents(VALUE self)
{
    PangoRectangle rink = {0, 0, 0, 0};
    PangoRectangle rlog = {0, 0, 0, 0};

    pango_layout_get_extents(_SELF(self), &rink, &rlog);
    return rb_assoc_new(PANGORECTANGLE2RVAL(&rink),
                        PANGORECTANGLE2RVAL(&rlog));
}

static VALUE
rg_get_pixel_extents(int argc, VALUE *argv, VALUE self)
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
        PangoRectangle* rect = RVAL2PANGORECTANGLE(ink_rect);
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
        PangoRectangle* rect = RVAL2PANGORECTANGLE(logical_rect);
        rlog.x = rect->x;
        rlog.y = rect->y;
        rlog.width = rect->width;
        rlog.height = rect->height;
    }   

    pango_layout_get_pixel_extents(_SELF(self), &rink, &rlog);

    return rb_assoc_new(PANGORECTANGLE2RVAL(&rink),
                        PANGORECTANGLE2RVAL(&rlog));
}

static VALUE
rg_pixel_extents(VALUE self)
{
    PangoRectangle rink = {0, 0, 0, 0};
    PangoRectangle rlog = {0, 0, 0, 0};

    pango_layout_get_pixel_extents(_SELF(self), &rink, &rlog);
    return rb_assoc_new(PANGORECTANGLE2RVAL(&rink),
                        PANGORECTANGLE2RVAL(&rlog));
}

static VALUE
rg_size(VALUE self)
{
    int width, height;
    pango_layout_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
rg_pixel_size(VALUE self)
{
    int width, height;
    pango_layout_get_pixel_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
rg_line_count(VALUE self)
{
    return INT2NUM(pango_layout_get_line_count(_SELF(self)));
}

static VALUE
rg_get_line(VALUE self, VALUE line)
{
    return PANGOLAYOUTLINE2RVAL(pango_layout_get_line(_SELF(self), NUM2INT(line)));
}

static VALUE
rg_lines(VALUE self)
{
    return GSLIST2ARY2(pango_layout_get_lines(_SELF(self)), PANGO_TYPE_LAYOUT_LINE);
}

static VALUE
rg_iter(VALUE self)
{
    return PANGOLAYOUTITER2RVAL(pango_layout_get_iter(_SELF(self)));
}

void
Init_pango_layout(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_LAYOUT, "Layout", mPango);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(copy, 0);
    RG_DEF_METHOD(context, 0);
    RG_DEF_METHOD(context_changed, 0);
    RG_DEF_METHOD(set_text, 1);
    RG_DEF_METHOD(text, 0);
    RG_DEF_METHOD(set_markup, -1);
    RG_DEF_METHOD_OPERATOR("markup=", layout_set_markup_eq, 1);
    RG_DEF_METHOD(set_attributes, 1);
    RG_DEF_METHOD(attributes, 0);
    RG_DEF_METHOD(set_font_description, 1);
#ifdef HAVE_PANGO_LAYOUT_GET_FONT_DESCRIPTION
    RG_DEF_METHOD(font_description, 0);
#endif
    RG_DEF_METHOD(set_width, 1);
    RG_DEF_METHOD(width, 0);
    RG_DEF_METHOD(set_wrap, 1);
    RG_DEF_METHOD(wrap, 0);
#ifdef HAVE_PANGO_LAYOUT_SET_ELLIPSIZE
    RG_DEF_METHOD(set_ellipsize, 1);
    RG_DEF_METHOD(ellipsize, 0);
#endif
    RG_DEF_METHOD(set_indent, 1);
    RG_DEF_METHOD(indent, 0);
    RG_DEF_METHOD(spacing, 0);
    RG_DEF_METHOD(set_spacing, 1);
    RG_DEF_METHOD(set_justify, 1);
    RG_DEF_METHOD_P(justify, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    RG_DEF_METHOD(set_auto_dir, 1);
    RG_DEF_METHOD_P(auto_dir, 0);
#endif
    RG_DEF_METHOD(set_alignment, 1);
    RG_DEF_METHOD(alignment, 0);
    RG_DEF_METHOD(set_tabs, 1);
    RG_DEF_METHOD(tabs, 0);
    RG_DEF_METHOD(set_single_paragraph_mode, 1);
    RG_DEF_METHOD_P(single_paragraph_mode, 0);
    RG_DEF_METHOD(log_attrs, 0);
    RG_DEF_METHOD(xy_to_index, 2);
    RG_DEF_METHOD(index_to_pos, 1);
    RG_DEF_METHOD(get_cursor_pos, 1);
    RG_DEF_METHOD(move_cursor_visually, 4);
    RG_DEF_METHOD(get_extents, -1);
    RG_DEF_METHOD(extents, 0);
    RG_DEF_METHOD(get_pixel_extents, -1);
    RG_DEF_METHOD(pixel_extents, 0);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(pixel_size, 0);
    RG_DEF_METHOD(line_count, 0);
    RG_DEF_METHOD(get_line, 1);
    RG_DEF_METHOD(lines, 0);
    RG_DEF_METHOD(iter, 0);

    /* PangoWrapMode */
    G_DEF_CLASS(PANGO_TYPE_WRAP_MODE, "WrapMode", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_WRAP_MODE, "PANGO_");

    /* PangoAlignment */
    G_DEF_CLASS(PANGO_TYPE_ALIGNMENT, "Alignment", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_ALIGNMENT, "PANGO_");

#ifdef HAVE_PANGO_LAYOUT_SET_ELLIPSIZE
    /* PangoEllipsizeMode */
    G_DEF_CLASS(PANGO_TYPE_ELLIPSIZE_MODE, "EllipsizeMode", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_ELLIPSIZE_MODE, "PANGO_");
#endif
}
