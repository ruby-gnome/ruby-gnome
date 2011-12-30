/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team
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

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE cPage
#define SELF(self) (RVAL2POPPLERPAGE(self))

#ifndef GDK_TYPE_REGION
extern GType gdk_region_get_type(void);
#  define GDK_TYPE_REGION (gdk_region_get_type())
#endif

#define GDK_REGION2RVAL(obj) (GDKREGION2RVAL(obj))
#define RVAL2GDK_PIXBUF(pixbuf) (RVAL2GDKPIXBUF(pixbuf))

#define SEL_STYLE2RVAL(obj) (POPPLERSELECTIONSTYLE2RVAL(obj))
#define RVAL2SEL_STYLE(obj) (RVAL2POPPLERSELECTIONSTYLE(obj))

#define TRANS2RVAL(obj) (POPPLERPAGETRANSITION2RVAL(obj))

static VALUE cRectangle;
static VALUE cPSFile;

#ifdef POPPLER_WITH_GDK
static VALUE
page_render_to_pixbuf(VALUE self, VALUE src_x, VALUE src_y, VALUE src_width,
                      VALUE src_height, VALUE scale, VALUE rotation,
                      VALUE pixbuf)
{
    poppler_page_render_to_pixbuf(SELF(self), NUM2INT(src_x),
                                  NUM2INT(src_y), NUM2INT(src_width),
                                  NUM2INT(src_height), NUM2DBL(scale),
                                  NUM2INT(rotation), RVAL2GOBJ(pixbuf));
    return Qnil;
}
#endif

#ifdef RB_POPPLER_CAIRO_AVAILABLE
static VALUE
page_render(VALUE self, VALUE cairo)
{
    poppler_page_render(SELF(self), RVAL2CRCONTEXT(cairo));
    return Qnil;
}
#endif

static VALUE
page_render_to_ps(VALUE self, VALUE ps_file)
{
    poppler_page_render_to_ps(SELF(self), RVAL2GOBJ(ps_file));
    return Qnil;
}

static VALUE
rg_render(int argc, VALUE *argv, VALUE self)
{
    if (argc == 1) {
        if (RVAL2CBOOL(rb_obj_is_kind_of(argv[0], cPSFile))) {
            return page_render_to_ps(self, argv[0]);
        } else {
#ifdef RB_POPPLER_CAIRO_AVAILABLE
            return page_render(self, argv[0]);
#else
            rb_raise(rb_eArgError, "cairo is not available");
#endif
        }
    } else if (argc == 7) {
#ifdef POPPLER_WITH_GDK
        return page_render_to_pixbuf(self, argv[0], argv[1], argv[2], argv[3],
                                     argv[4], argv[5], argv[6]);
#else
        rb_raise(rb_eArgError, "GDK is not available");
#endif
    } else {
        rb_raise(rb_eArgError,
                 "wrong number of arguments (%d for 1 or 7)",
                 argc);
    }
}

#ifdef POPPLER_WITH_GDK
static VALUE
page_render_to_pixbuf_for_printing(VALUE self, VALUE src_x, VALUE src_y,
                                   VALUE src_width, VALUE src_height,
                                   VALUE scale, VALUE rotation, VALUE pixbuf)
{
    poppler_page_render_to_pixbuf_for_printing(SELF(self), NUM2INT(src_x),
                                               NUM2INT(src_y),
                                               NUM2INT(src_width),
                                               NUM2INT(src_height),
                                               NUM2DBL(scale),
                                               NUM2INT(rotation),
                                               RVAL2GOBJ(pixbuf));
    return Qnil;
}
#endif

#ifdef RB_POPPLER_CAIRO_AVAILABLE
static VALUE
page_render_for_printing(VALUE self, VALUE cairo)
{
    poppler_page_render_for_printing(SELF(self), RVAL2CRCONTEXT(cairo));
    return Qnil;
}
#endif

static VALUE
rg_render_for_printing(int argc, VALUE *argv, VALUE self)
{
    if (argc == 1) {
#ifdef RB_POPPLER_CAIRO_AVAILABLE
        return page_render_for_printing(self, argv[0]);
#else
        rb_raise(rb_eArgError, "cairo is not available");
#endif
    } else if (argc == 7) {
#ifdef POPPLER_WITH_GDK
        return page_render_to_pixbuf_for_printing(self, argv[0], argv[1],
                                                  argv[2], argv[3],
                                                  argv[4], argv[5], argv[6]);
#else
        rb_raise(rb_eArgError, "GDK is not available");
#endif
    } else {
        rb_raise(rb_eArgError,
                 "wrong number of arguments (%d for 1 or 7)", argc);
    }
}

#ifdef RB_POPPLER_CAIRO_AVAILABLE
static VALUE
page_render_selection(VALUE self, VALUE cairo,
                      VALUE selection, VALUE rb_old_selection,
                      VALUE style, VALUE glyph_color, VALUE background_color)
{
    PopplerRectangle *old_selection = NULL;

    if (!NIL_P(rb_old_selection))
        old_selection = RVAL2POPPLER_RECT(rb_old_selection);
    poppler_page_render_selection(SELF(self), RVAL2CRCONTEXT(cairo),
                                  RVAL2POPPLER_RECT(selection),
                                  old_selection,
                                  RVAL2SEL_STYLE(style),
                                  RVAL2POPPLERCOLOR(glyph_color),
                                  RVAL2POPPLERCOLOR(background_color));
    return Qnil;
}
#endif

#ifdef POPPLER_WITH_GDK
static VALUE
page_render_selection_to_pixbuf(VALUE self, VALUE scale, VALUE rotation,
                                VALUE pixbuf, VALUE selection,
                                VALUE rb_old_selection,
                                VALUE style,
                                VALUE glyph_color, VALUE background_color)
{
    PopplerRectangle *old_selection = NULL;

    if (!NIL_P(rb_old_selection))
        old_selection = RVAL2POPPLER_RECT(rb_old_selection);
    poppler_page_render_selection_to_pixbuf(SELF(self),
                                            NUM2DBL(scale),
                                            NUM2INT(rotation),
                                            RVAL2GOBJ(pixbuf),
                                            RVAL2POPPLER_RECT(selection),
                                            old_selection,
                                            RVAL2SEL_STYLE(style),
                                            RVAL2GDKCOLOR(glyph_color),
                                            RVAL2GDKCOLOR(background_color));
    return Qnil;
}
#endif

static VALUE
rg_render_selection(int argc, VALUE *argv, VALUE self)
{
    if (argc == 6) {
#ifdef RB_POPPLER_CAIRO_AVAILABLE
        return page_render_selection(self, argv[0], argv[1], argv[2],
                                     argv[3], argv[4], argv[5]);
#else
        rb_raise(rb_eArgError, "cairo is not available");
#endif
    } else if (argc == 8) {
#ifdef POPPLER_WITH_GDK
        return page_render_selection_to_pixbuf(self, argv[0], argv[1],
                                               argv[2], argv[3], argv[4],
                                               argv[5], argv[6], argv[7]);
#else
        rb_raise(rb_eArgError, "GDK is not available");
#endif
    } else {
        rb_raise(rb_eArgError,
                 "wrong number of arguments (%d for 5 or 8)", argc);
    }
}

static VALUE
rg_size(VALUE self)
{
    double width, height;
    poppler_page_get_size(SELF(self), &width, &height);
    return rb_ary_new3(2, rb_float_new(width), rb_float_new(height));
}

static VALUE
rg_index(VALUE self)
{
    return INT2NUM(poppler_page_get_index(SELF(self)));
}

static VALUE
rg_duration(VALUE self)
{
    return rb_float_new(poppler_page_get_duration(SELF(self)));
}

static VALUE
rg_transition(VALUE self)
{
    return TRANS2RVAL(poppler_page_get_transition(SELF(self)));
}

#if RB_POPPLER_CAIRO_AVAILABLE
static VALUE
rg_thumbnail(VALUE self)
{
    return CRSURFACE2RVAL(poppler_page_get_thumbnail(SELF(self)));
}
#endif

#ifdef POPPLER_WITH_GDK
static VALUE
rg_thumbnail_pixbuf(VALUE self)
{
    return GOBJ2RVAL(poppler_page_get_thumbnail_pixbuf(SELF(self)));
}
#endif

static VALUE
rg_thumbnail_size(VALUE self)
{
    int width, height;

    if (poppler_page_get_thumbnail_size(SELF(self), &width, &height))
        return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
    else
        return Qnil;
}

static VALUE
rg_find_text(VALUE self, VALUE text)
{
    return GLIST2ARY2F(poppler_page_find_text(SELF(self), RVAL2CSTR(text)),
                       POPPLER_TYPE_RECTANGLE);
}

static VALUE
rg_get_text(int argc, VALUE *argv, VALUE self)
{
    gchar *text;
    PopplerSelectionStyle style = POPPLER_SELECTION_GLYPH;
    VALUE rb_text, arg1, arg2, rb_rect;
    PopplerPage *page;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);

    page = SELF(self);
    if (NIL_P(arg1)) {
        rb_rect = arg2;
    } else {
        if (RTEST(rb_obj_is_kind_of(arg2, cRectangle))) {
            rb_rect = arg2;
        } else {
            rb_rect = Qnil;
            if (!NIL_P(arg2)) {
                style = RVAL2SEL_STYLE(arg2);
            }
        }
    }

    if (NIL_P(rb_rect)) {
#if POPPLER_CHECK_VERSION(0, 15, 0)
        text = poppler_page_get_text(page);
#else
        PopplerRectangle rect;
        double width, height;

        rect.x1 = 0;
        rect.y1 = 0;
        poppler_page_get_size(page, &width, &height);
        rect.x2 = width;
        rect.y2 = height;
        text = poppler_page_get_text(page,
                                     style,
                                     &rect);
#endif
    } else {
        PopplerRectangle *rect;

        rect = RVAL2POPPLER_RECT(rb_rect);
#if POPPLER_CHECK_VERSION(0, 15, 0)
        text = poppler_page_get_selected_text(page, style, rect);
#else
        text = poppler_page_get_text(page, style, rect);
#endif
    }

    rb_text = CSTR2RVAL(text);
    g_free(text);
    return rb_text;
}

static VALUE
rg_get_selection_region(VALUE self, VALUE scale, VALUE style, VALUE selection)
{
    return GLIST2ARY2F(poppler_page_get_selection_region(SELF(self),
                                                         NUM2DBL(scale),
                                                         RVAL2SEL_STYLE(style),
                                                         RVAL2POPPLER_RECT(selection)),
                                                         POPPLER_TYPE_RECTANGLE);
}

static VALUE
rg_link_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_link_mapping(SELF(self)),
                       POPPLER_TYPE_LINK_MAPPING);
}

static VALUE
rg_image_mapping(VALUE self)
{
    VALUE mappings;
    GList *image_mapping, *node;

    mappings = rb_ary_new();
    image_mapping = poppler_page_get_image_mapping(SELF(self));
    for (node = image_mapping; node; node = g_list_next(node)) {
        PopplerImageMapping *image_mapping;
        VALUE mapping;

        image_mapping = node->data;
        mapping = POPPLERIMAGEMAPPING2RVAL(image_mapping);
#ifdef RB_POPPLER_CAIRO_AVAILABLE
        rb_iv_set(mapping, "@page", self);
#endif
        rb_ary_push(mappings, mapping);
    }
    poppler_page_free_image_mapping(image_mapping);

    return mappings;
}

#ifdef RB_POPPLER_CAIRO_AVAILABLE
static VALUE
rg_get_image(VALUE self, VALUE image_id)
{
    return CRSURFACE2RVAL(poppler_page_get_image(SELF(self), NUM2INT(image_id)));
}
#endif

static VALUE
rg_form_field_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_form_field_mapping(SELF(self)),
                       POPPLER_TYPE_FORM_FIELD_MAPPING);
}

static VALUE
rg_annotation_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_annot_mapping(SELF(self)),
                       POPPLER_TYPE_ANNOT_MAPPING);
}

static VALUE
rg_crop_box(VALUE self)
{
    PopplerRectangle rect;

    poppler_page_get_crop_box(SELF(self), &rect);
    return POPPLER_RECT2RVAL(&rect);
}

void
Init_poppler_page(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_PAGE, "Page", mPoppler);
    cRectangle = rb_const_get(mPoppler, rb_intern("Rectangle"));
    cPSFile = rb_const_get(mPoppler, rb_intern("PSFile"));

    RG_DEF_METHOD(render, -1);
    RG_DEF_METHOD(render_for_printing, -1);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(index, 0);
    RG_DEF_METHOD(duration, 0);
    RG_DEF_METHOD(transition, 0);

#if RB_POPPLER_CAIRO_AVAILABLE
    RG_DEF_METHOD(thumbnail, 0);
#endif
#if POPPLER_WITH_GDK
    RG_DEF_METHOD(thumbnail_pixbuf, 0);
#endif
    RG_DEF_METHOD(thumbnail_size, 0);
    RG_DEF_METHOD(find_text, 1);
    RG_DEF_METHOD(get_text, -1);
    RG_DEF_METHOD(get_selection_region, 3);
    RG_DEF_METHOD(link_mapping, 0);
    RG_DEF_METHOD(image_mapping, 0);
#if RB_POPPLER_CAIRO_AVAILABLE
    RG_DEF_METHOD(get_image, 1);
#endif

    RG_DEF_METHOD(form_field_mapping, 0);
    RG_DEF_METHOD(annotation_mapping, 0);
    RG_DEF_METHOD(render_selection, -1);
    RG_DEF_METHOD(crop_box, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
