/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbpoppler-page.c -

  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler-private.h"

#define SELF(self) (POPPLER_PAGE(RVAL2GOBJ(self)))

#ifndef GDK_TYPE_REGION
extern GType gdk_region_get_type(void);
#  define GDK_TYPE_REGION (gdk_region_get_type())
#endif

#define GDK_REGION2RVAL(obj) (BOXED2RVAL(obj, GDK_TYPE_REGION))
#define RVAL2GDK_PIXBUF(pixbuf) (GDK_PIXBUF(RVAL2GOBJ(pixbuf)))


#define SEL_STYLE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_SELECTION_STYLE))
#define RVAL2SEL_STYLE(obj) (RVAL2GENUM(obj, POPPLER_TYPE_SELECTION_STYLE))

#define RVAL2TRANS(obj) ((PopplerPageTransition *)RVAL2BOXED(obj, POPPLER_TYPE_PAGE_TRANSITION))
#define TRANS2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION))
#define RVAL2LM(obj) ((PopplerLinkMapping *)RVAL2BOXED(obj, POPPLER_TYPE_LINK_MAPPING))
#define RVAL2IM(obj) ((PopplerImageMapping *)RVAL2BOXED(obj, POPPLER_TYPE_IMAGE_MAPPING))
#define RVAL2FFM(obj) ((PopplerFormFieldMapping *)RVAL2BOXED(obj, POPPLER_TYPE_FORM_FIELD_MAPPING))
#define RVAL2AM(obj) ((PopplerAnnotMapping *)RVAL2BOXED(obj, POPPLER_TYPE_ANNOT_MAPPING))

#define TT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_TYPE))
#define RVAL2TT(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_TYPE))
#define TA2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT))
#define RVAL2TA(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT))
#define TD2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_DIRECTION))
#define RVAL2TD(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_DIRECTION))

static VALUE cPSFile, cRectangle;

#ifdef POPPLER_TYPE_COLOR
VALUE rb_cPopplerColor;

PopplerColor *
rb_poppler_ruby_object_to_color(VALUE color)
{
#ifdef POPPLER_WITH_GDK
    if (RTEST(rb_obj_is_kind_of(color, rb_cGdkColor))) {
	GdkColor *gdk_color;

	gdk_color = RVAL2GDKCOLOR(color);
	color = rb_funcall(rb_cPopplerColor, rb_intern("new"),
			   3,
			   UINT2NUM(gdk_color->red),
			   UINT2NUM(gdk_color->green),
			   UINT2NUM(gdk_color->blue));
    }
#endif

    return RVAL2BOXED(color, POPPLER_TYPE_COLOR);
}

VALUE
rb_poppler_ruby_object_from_color_with_free(PopplerColor *color)
{
    VALUE rb_color;

    rb_color = POPPLER_COLOR2RVAL(color);
    g_free(color);
    return rb_color;
}
#endif

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
page_render_generic(int argc, VALUE *argv, VALUE self)
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
page_render_for_printing_generic(int argc, VALUE *argv, VALUE self)
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

#if defined(RB_POPPLER_CAIRO_AVAILABLE) && \
      defined(HAVE_POPPLER_PAGE_RENDER_SELECTION_TO_PIXBUF)
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
                                  RVAL2POPPLER_COLOR(glyph_color),
                                  RVAL2POPPLER_COLOR(background_color));
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
page_render_selection_generic(int argc, VALUE *argv, VALUE self)
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
page_get_size(VALUE self)
{
    double width, height;
    poppler_page_get_size(SELF(self), &width, &height);
    return rb_ary_new3(2, rb_float_new(width), rb_float_new(height));
}

static VALUE
page_get_index(VALUE self)
{
    return INT2NUM(poppler_page_get_index(SELF(self)));
}

static VALUE
page_get_duration(VALUE self)
{
    return rb_float_new(poppler_page_get_duration(SELF(self)));
}

static VALUE
page_get_transition(VALUE self)
{
    return TRANS2RVAL(poppler_page_get_transition(SELF(self)));
}

#if RB_POPPLER_CAIRO_AVAILABLE
static VALUE
page_get_thumbnail(VALUE self)
{
    return CRSURFACE2RVAL(poppler_page_get_thumbnail(SELF(self)));
}
#endif

#ifdef POPPLER_WITH_GDK
static VALUE
page_get_thumbnail_pixbuf(VALUE self)
{
    return GOBJ2RVAL(poppler_page_get_thumbnail_pixbuf(SELF(self)));
}
#endif

static VALUE
page_get_thumbnail_size(VALUE self)
{
    int width, height;

    if (poppler_page_get_thumbnail_size(SELF(self), &width, &height))
        return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
    else
        return Qnil;
}

static VALUE
page_find_text(VALUE self, VALUE text)
{
    return GLIST2ARY2F(poppler_page_find_text(SELF(self), RVAL2CSTR(text)),
                       POPPLER_TYPE_RECTANGLE);
}

static VALUE
page_get_text(int argc, VALUE *argv, VALUE self)
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
    } else {
        text = poppler_page_get_text(page,
                                     style,
                                     RVAL2POPPLER_RECT(rb_rect));
    }

    rb_text = CSTR2RVAL(text);
    g_free(text);
    return rb_text;
}

static VALUE
page_get_selection_region(VALUE self, VALUE scale, VALUE style, VALUE selection)
{
    return GLIST2ARY2F(poppler_page_get_selection_region(SELF(self),
							 NUM2DBL(scale),
							 RVAL2SEL_STYLE(style),
							 RVAL2POPPLER_RECT(selection)),
                       POPPLER_TYPE_RECTANGLE);
}

static VALUE
page_get_link_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_link_mapping(SELF(self)),
                       POPPLER_TYPE_LINK_MAPPING);
}

static VALUE
page_get_image_mapping(VALUE self)
{
    VALUE mappings;
    GList *image_mapping, *node;

    mappings = rb_ary_new();
    image_mapping = poppler_page_get_image_mapping(SELF(self));
    for (node = image_mapping; node; node = g_list_next(node)) {
	PopplerImageMapping *image_mapping;
	VALUE mapping;

	image_mapping = node->data;
	mapping = BOXED2RVAL(image_mapping, POPPLER_TYPE_IMAGE_MAPPING);
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
_page_get_image(VALUE self, gint image_id)
{
    return CRSURFACE2RVAL(poppler_page_get_image(SELF(self), image_id));
}

static VALUE
page_get_image(VALUE self, VALUE image_id)
{
    return _page_get_image(self, NUM2INT(image_id));
}
#endif

static VALUE
page_get_form_field_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_form_field_mapping(SELF(self)),
                       POPPLER_TYPE_FORM_FIELD_MAPPING);
}

static VALUE
page_get_annot_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_annot_mapping(SELF(self)),
                       POPPLER_TYPE_ANNOT_MAPPING);
}

static VALUE
page_get_crop_box(VALUE self)
{
    PopplerRectangle rect;

    poppler_page_get_crop_box(SELF(self), &rect);
    return POPPLER_RECT2RVAL(&rect);
}


/* A rectangle on a page, with coordinates in PDF points. */
static VALUE
rectangle_initialize(VALUE self, VALUE x1, VALUE y1, VALUE x2, VALUE y2)
{
    PopplerRectangle rectangle;

    rectangle.x1 = NUM2DBL(x1);
    rectangle.y1 = NUM2DBL(y1);
    rectangle.x2 = NUM2DBL(x2);
    rectangle.y2 = NUM2DBL(y2);

    G_INITIALIZE(self, &rectangle);
    return Qnil;
}

DEF_ACCESSOR(rectangle, x1, RVAL2POPPLER_RECT, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, y1, RVAL2POPPLER_RECT, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, x2, RVAL2POPPLER_RECT, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, y2, RVAL2POPPLER_RECT, rb_float_new, NUM2DBL)

static VALUE
rectangle_to_a(VALUE self)
{
    PopplerRectangle *rectangle = RVAL2POPPLER_RECT(self);
    return rb_ary_new3(4,
                       rb_float_new(rectangle->x1),
                       rb_float_new(rectangle->y1),
                       rb_float_new(rectangle->x2),
                       rb_float_new(rectangle->y2));
}

static VALUE
rectangle_inspect(VALUE self)
{
    VALUE inspected;
    gchar *points;
    PopplerRectangle *rectangle;

    rectangle = RVAL2POPPLER_RECT(self);
    inspected = rb_call_super(0, NULL);
    rb_str_resize(inspected, RSTRING_LEN(inspected) - 1);
    points = g_strdup_printf(": [%g, %g, %g, %g]>",
			     rectangle->x1, rectangle->y1,
			     rectangle->x2, rectangle->y2);
    rb_str_cat2(inspected, points);
    g_free(points);
    return inspected;
}


#ifdef POPPLER_TYPE_COLOR
/* A color in RGB */
static VALUE
color_initialize(VALUE self, VALUE red, VALUE green, VALUE blue)
{
    PopplerColor color;

    color.red = NUM2UINT(red);
    color.green = NUM2UINT(green);
    color.blue = NUM2UINT(blue);

    G_INITIALIZE(self, &color);
    return Qnil;
}

DEF_ACCESSOR(color, red, RVAL2POPPLER_COLOR, UINT2NUM, NUM2UINT)
DEF_ACCESSOR(color, green, RVAL2POPPLER_COLOR, UINT2NUM, NUM2UINT)
DEF_ACCESSOR(color, blue, RVAL2POPPLER_COLOR, UINT2NUM, NUM2UINT)

static VALUE
color_to_a(VALUE self)
{
    PopplerColor *color;
    color = RVAL2POPPLER_COLOR(self);
    return rb_ary_new3(3,
                       UINT2NUM(color->red),
                       UINT2NUM(color->green),
                       UINT2NUM(color->blue));
}

static VALUE
color_inspect(VALUE self)
{
    VALUE inspected;
    gchar *rgb;
    PopplerColor *color;

    color = RVAL2POPPLER_COLOR(self);
    inspected = rb_call_super(0, NULL);
    rb_str_resize(inspected, RSTRING_LEN(inspected) - 1);
    rgb = g_strdup_printf(": [%u, %u, %u]>",
			  color->red, color->green, color->blue);
    rb_str_cat2(inspected, rgb);
    g_free(rgb);
    return inspected;
}
#endif


/* Mapping between areas on the current page and PopplerActions */
#define RECT_ENTITY2RVAL(rect) POPPLER_RECT2RVAL(&(rect))
#define RECT_ENTITY_SET(rect, rb_rect) rectangle_set(&(rect), rb_rect)
static void
rectangle_set(PopplerRectangle *rect, VALUE rb_rect)
{
    *rect = *(RVAL2POPPLER_RECT(rb_rect));
}

DEF_ACCESSOR_WITH_SETTER(link_mapping, area,
                         RVAL2LM, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(link_mapping, action, RVAL2LM,
	     POPPLER_ACTION2RVAL, RVAL2POPPLER_ACTION)


/* Page Transition */
DEF_ACCESSOR(page_trans, type, RVAL2TRANS, RVAL2TT, TT2RVAL)
DEF_ACCESSOR(page_trans, alignment, RVAL2TRANS, RVAL2TA, TA2RVAL)
DEF_ACCESSOR(page_trans, direction, RVAL2TRANS, RVAL2TD, TD2RVAL)
DEF_ACCESSOR(page_trans, duration, RVAL2TRANS, NUM2INT, INT2NUM)
DEF_ACCESSOR(page_trans, angle, RVAL2TRANS, NUM2INT, INT2NUM)
DEF_ACCESSOR(page_trans, scale, RVAL2TRANS, NUM2DBL, rb_float_new)
DEF_ACCESSOR(page_trans, rectangular, RVAL2TRANS, RVAL2CBOOL, CBOOL2RVAL)


/* Mapping between areas on the current page and images */
DEF_ACCESSOR_WITH_SETTER(image_mapping, area,
                         RVAL2IM, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(image_mapping, image_id, RVAL2IM, INT2NUM, NUM2INT)
#ifdef RB_POPPLER_CAIRO_AVAILABLE
static VALUE
image_mapping_get_image(VALUE self)
{
    return _page_get_image(rb_iv_get(self, "@page"), RVAL2IM(self)->image_id);
}
#endif


/* Mapping between areas on the current page and form fields */
DEF_ACCESSOR_WITH_SETTER(form_field_mapping, area,
                         RVAL2FFM, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(form_field_mapping, field, RVAL2FFM,
	     POPPLER_FORM_FIELD2RVAL, RVAL2POPPLER_FORM_FIELD)

static VALUE
annot_mapping_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE area, annotation;
    PopplerAnnotMapping *mapping;

    rb_scan_args(argc, argv, "02", &area, &annotation);

    mapping = poppler_annot_mapping_new();
    mapping->area = *RVAL2POPPLER_RECT(area);
    mapping->annot = RVAL2POPPLER_ANNOT(annotation);
    G_INITIALIZE(self, mapping);

    return Qnil;
}

DEF_ACCESSOR_WITH_SETTER(annot_mapping, area,
                         RVAL2AM, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_READER(annot_mapping, annotation, annot, RVAL2AM, POPPLER_ANNOT2RVAL)

static VALUE
annot_mapping_set_annotation(VALUE self, VALUE annotation)
{
    PopplerAnnotMapping *mapping;

    mapping = RVAL2AM(self);
    if (mapping->annot)
	g_object_unref(mapping->annot);

    mapping->annot = RVAL2POPPLER_ANNOT(annotation);
    return Qnil;
}

void
Init_poppler_page(VALUE mPoppler)
{
    VALUE cPage, cLinkMapping;
    VALUE cPageTransition, cImageMapping, cFormFieldMapping;
    VALUE cAnnotationMapping;

    cPage = G_DEF_CLASS(POPPLER_TYPE_PAGE, "Page", mPoppler);
    cRectangle = G_DEF_CLASS(POPPLER_TYPE_RECTANGLE, "Rectangle", mPoppler);
#ifdef POPPLER_TYPE_COLOR
    rb_cPopplerColor = G_DEF_CLASS(POPPLER_TYPE_COLOR, "Color", mPoppler);
#endif
    cLinkMapping = G_DEF_CLASS(POPPLER_TYPE_LINK_MAPPING, "LinkMapping",
                               mPoppler);
    cPageTransition = G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION,
                                  "PageTransition", mPoppler);
    cImageMapping = G_DEF_CLASS(POPPLER_TYPE_IMAGE_MAPPING,
                                "ImageMapping", mPoppler);
    cFormFieldMapping = G_DEF_CLASS(POPPLER_TYPE_FORM_FIELD_MAPPING,
                                    "FormFieldMapping", mPoppler);
    cAnnotationMapping = G_DEF_CLASS(POPPLER_TYPE_ANNOT_MAPPING,
				     "AnnotationMapping", mPoppler);
    cPSFile = rb_const_get(mPoppler, rb_intern("PSFile"));

    rb_define_method(cPage, "render", page_render_generic, -1);
    rb_define_method(cPage, "render_for_printing",
		     page_render_for_printing_generic, -1);
    rb_define_method(cPage, "size", page_get_size, 0);
    rb_define_method(cPage, "index", page_get_index, 0);
    rb_define_method(cPage, "duration", page_get_duration, 0);
    rb_define_method(cPage, "transition", page_get_transition, 0);

#if RB_POPPLER_CAIRO_AVAILABLE
    rb_define_method(cPage, "thumbnail", page_get_thumbnail, 0);
#endif
#if POPPLER_WITH_GDK
    rb_define_method(cPage, "thumbnail_pixbuf", page_get_thumbnail_pixbuf, 0);
#endif
    rb_define_method(cPage, "thumbnail_size", page_get_thumbnail_size, 0);
    rb_define_method(cPage, "find_text", page_find_text, 1);
    rb_define_method(cPage, "get_text", page_get_text, -1);
    rb_define_method(cPage, "get_selection_region",
		     page_get_selection_region, 3);
    rb_define_method(cPage, "link_mapping", page_get_link_mapping, 0);
    rb_define_method(cPage, "image_mapping", page_get_image_mapping, 0);
#if RB_POPPLER_CAIRO_AVAILABLE
    rb_define_method(cPage, "get_image", page_get_image, 1);
#endif

    rb_define_method(cPage, "form_field_mapping",
                     page_get_form_field_mapping, 0);
    rb_define_method(cPage, "annotation_mapping",
                     page_get_annot_mapping, 0);
    rb_define_method(cPage, "render_selection",
                     page_render_selection_generic, -1);
    rb_define_method(cPage, "crop_box", page_get_crop_box, 0);

    G_DEF_SETTERS(cPage);

/* A rectangle on a page, with coordinates in PDF points. */
    rb_define_method(cRectangle, "initialize", rectangle_initialize, 4);
    rb_define_method(cRectangle, "x1", rectangle_get_x1, 0);
    rb_define_method(cRectangle, "y1", rectangle_get_y1, 0);
    rb_define_method(cRectangle, "x2", rectangle_get_x2, 0);
    rb_define_method(cRectangle, "y2", rectangle_get_y2, 0);
    rb_define_method(cRectangle, "set_x1", rectangle_set_x1, 1);
    rb_define_method(cRectangle, "set_y1", rectangle_set_y1, 1);
    rb_define_method(cRectangle, "set_x2", rectangle_set_x2, 1);
    rb_define_method(cRectangle, "set_y2", rectangle_set_y2, 1);
    rb_define_method(cRectangle, "to_a", rectangle_to_a, 0);
    rb_define_method(cRectangle, "inspect", rectangle_inspect, 0);

    G_DEF_SETTERS(cRectangle);

#ifdef POPPLER_TYPE_COLOR
/* A color in RGB */
    rb_define_method(rb_cPopplerColor, "initialize", color_initialize, 3);
    rb_define_method(rb_cPopplerColor, "red", color_get_red, 0);
    rb_define_method(rb_cPopplerColor, "green", color_get_green, 0);
    rb_define_method(rb_cPopplerColor, "blue", color_get_blue, 0);
    rb_define_method(rb_cPopplerColor, "set_red", color_set_red, 1);
    rb_define_method(rb_cPopplerColor, "set_green", color_set_green, 1);
    rb_define_method(rb_cPopplerColor, "set_blue", color_set_blue, 1);
    rb_define_method(rb_cPopplerColor, "to_a", color_to_a, 0);
    rb_define_method(rb_cPopplerColor, "inspect", color_inspect, 0);

    G_DEF_SETTERS(rb_cPopplerColor);
#endif

/* Mapping between areas on the current page and PopplerActions */
    rb_define_method(cLinkMapping, "area", link_mapping_get_area, 0);
    rb_define_method(cLinkMapping, "action", link_mapping_get_action, 0);

    rb_define_method(cLinkMapping, "set_area", link_mapping_set_area, 1);
    rb_define_method(cLinkMapping, "set_action", link_mapping_set_action, 1);

    G_DEF_SETTERS(cLinkMapping);

/* Page Transition */
    rb_define_method(cPageTransition, "type", page_trans_get_type, 0);
    rb_define_method(cPageTransition, "alignment", page_trans_get_alignment, 0);
    rb_define_method(cPageTransition, "direction", page_trans_get_direction, 0);
    rb_define_method(cPageTransition, "duration", page_trans_get_duration, 0);
    rb_define_method(cPageTransition, "angle", page_trans_get_angle, 0);
    rb_define_method(cPageTransition, "scale", page_trans_get_scale, 0);
    rb_define_method(cPageTransition, "rectangular",
                     page_trans_get_rectangular, 0);

    rb_define_method(cPageTransition, "set_type", page_trans_set_type, 1);
    rb_define_method(cPageTransition, "set_alignment",
                     page_trans_set_alignment, 1);
    rb_define_method(cPageTransition, "set_direction",
                     page_trans_set_direction, 1);
    rb_define_method(cPageTransition, "set_duration",
                     page_trans_set_duration, 1);
    rb_define_method(cPageTransition, "set_angle", page_trans_set_angle, 1);
    rb_define_method(cPageTransition, "set_scale", page_trans_set_scale, 1);
    rb_define_method(cPageTransition, "set_rectangular",
                     page_trans_set_rectangular, 1);

    G_DEF_SETTERS(cPageTransition);


/* Mapping between areas on the current page and images */
    rb_define_method(cImageMapping, "area", image_mapping_get_area, 0);
    rb_define_method(cImageMapping, "image_id", image_mapping_get_image_id, 0);
#ifdef RB_POPPLER_CAIRO_AVAILABLE
    rb_define_method(cImageMapping, "image", image_mapping_get_image, 0);
#endif

    rb_define_method(cImageMapping, "set_area", image_mapping_set_area, 1);
    rb_define_method(cImageMapping, "set_image_id",
		     image_mapping_set_image_id, 1);

    G_DEF_SETTERS(cImageMapping);


/* Mapping between areas on the current page and form fields */
    rb_define_method(cFormFieldMapping, "area", form_field_mapping_get_area, 0);
    rb_define_method(cFormFieldMapping, "field", form_field_mapping_get_field,
                     0);

    rb_define_method(cFormFieldMapping, "set_area",
                     form_field_mapping_set_area, 1);
    rb_define_method(cFormFieldMapping, "set_field",
                     form_field_mapping_set_field, 1);

    G_DEF_SETTERS(cFormFieldMapping);

    rb_define_method(cAnnotationMapping, "initialize",
		     annot_mapping_initialize, -1);

    rb_define_method(cAnnotationMapping, "area", annot_mapping_get_area, 0);
    rb_define_method(cAnnotationMapping, "annotation",
		     annot_mapping_get_annotation, 0);

    rb_define_method(cAnnotationMapping, "set_area", annot_mapping_set_area, 1);
    rb_define_method(cAnnotationMapping, "set_annotation",
		     annot_mapping_set_annotation, 1);

    G_DEF_SETTERS(cAnnotationMapping);
}
