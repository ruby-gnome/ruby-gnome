/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbpoppler-page.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:33 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"

#define SELF(self) (POPPLER_PAGE(RVAL2GOBJ(self)))
#define RVAL2LM(obj) ((PopplerLinkMapping *)RVAL2BOXED(obj, POPPLER_TYPE_LINK_MAPPING))
#define RVAL2IM(obj) ((PopplerImageMapping *)RVAL2BOXED(obj, POPPLER_TYPE_IMAGE_MAPPING))
#define RVAL2FF(obj) ((PopplerFormField *)RVAL2BOXED(obj, POPPLER_TYPE_FORM_FIELD))
#define RVAL2TF(obj) (&(RVAL2FF(obj)->text))
#define RVAL2BF(obj) (&(RVAL2FF(obj)->button))
#define RVAL2CF(obj) (&(RVAL2FF(obj)->choice))

#define TT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_TYPE))
#define RVAL2TT(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_TYPE))
#define TA2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT))
#define RVAL2TA(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT))
#define TD2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_DIRECTION))
#define RVAL2TD(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_DIRECTION))
#define FFT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_FIELD_TYPE))
#define RVAL2FFT(obj) (RVAL2GENUM(obj, POPPLER_TYPE_FORM_FIELD_TYPE))

static VALUE cPSFile;

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
        return page_render_to_pixbuf(self, argv[0], argv[1], argv[2], argv[3],
                                     argv[4], argv[5], argv[6]);
    } else {
        rb_raise(rb_eArgError,
                 "wrong number of arguments (%d for 1 or 7)", argc);
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

#if POPPLER_CHECK_VERSION(0, 5, 9)
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
#endif

static VALUE
page_get_thumbnail(VALUE self)
{
    return GOBJ2RVAL(poppler_page_get_thumbnail(SELF(self)));
}

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
    VALUE rb_text, rb_rect;
    PopplerPage *page;

    rb_scan_args(argc, argv, "01", &rb_rect);

    page = SELF(self);
    if (NIL_P(rb_rect)) {
        PopplerRectangle rect;
        double width, height;

        rect.x1 = 0;
        rect.y1 = 0;
        poppler_page_get_size(page, &width, &height);
        rect.x2 = width;
        rect.y2 = height;
        text = poppler_page_get_text(page, &rect);
    } else {
        text = poppler_page_get_text(page, RVAL2RECT(rb_rect));
    }

    rb_text = CSTR2RVAL(text);
    g_free(text);
    return rb_text;
}

static VALUE
page_get_link_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_link_mapping(SELF(self)),
                       POPPLER_TYPE_LINK_MAPPING);
}

static VALUE
page_get_selection_region(VALUE self, VALUE scale, VALUE selection)
{
    return REGION2RVAL(poppler_page_get_selection_region(SELF(self),
                                                         NUM2DBL(scale),
                                                         RVAL2RECT(selection)));
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
        old_selection = RVAL2RECT(rb_old_selection);
    poppler_page_render_selection(SELF(self), RVAL2CRCONTEXT(cairo),
                                  RVAL2RECT(selection),
                                  old_selection,
                                  RVAL2SELSTYLE(style),
                                  RVAL2COLOR(glyph_color),
                                  RVAL2COLOR(background_color));
    return Qnil;
}
#endif

#ifndef HAVE_POPPLER_PAGE_RENDER_SELECTION_TO_PIXBUF
#  define poppler_page_render_selection_to_pixbuf poppler_page_render_selection
#endif

static VALUE
page_render_selection_to_pixbuf(VALUE self, VALUE scale, VALUE rotation,
                                VALUE pixbuf, VALUE selection,
                                VALUE rb_old_selection,
#ifdef HAVE_POPPLER_PAGE_RENDER_SELECTION_TO_PIXBUF
                                VALUE style,
#endif
                                VALUE glyph_color, VALUE background_color)
{
    PopplerRectangle *old_selection = NULL;

    if (!NIL_P(rb_old_selection))
        old_selection = RVAL2RECT(rb_old_selection);
    poppler_page_render_selection_to_pixbuf(SELF(self),
                                            NUM2DBL(scale),
                                            NUM2INT(rotation),
                                            RVAL2GOBJ(pixbuf),
                                            RVAL2RECT(selection),
                                            old_selection,
#ifdef HAVE_POPPLER_PAGE_RENDER_SELECTION_TO_PIXBUF
                                            RVAL2SELSTYLE(style),
#endif
                                            RVAL2COLOR(glyph_color),
                                            RVAL2COLOR(background_color));
    return Qnil;
}

static VALUE
page_render_selection_generic(int argc, VALUE *argv, VALUE self)
{
    if (argc == 6) {
#if defined(RB_POPPLER_CAIRO_AVAILABLE) && \
      defined(HAVE_POPPLER_PAGE_RENDER_SELECTION_TO_PIXBUF)
        return page_render_selection(self, argv[0], argv[1], argv[2],
                                     argv[3], argv[4], argv[5]);
#else
        rb_raise(rb_eArgError, "cairo is not available");
#endif
#ifdef HAVE_POPPLER_PAGE_RENDER_SELECTION_TO_PIXBUF
    } else if (argc == 8) {
        return page_render_selection_to_pixbuf(self, argv[0], argv[1],
                                               argv[2], argv[3], argv[4],
                                               argv[5], argv[6], argv[7]);
#else
    } else if (argc == 7) {
        return page_render_selection_to_pixbuf(self, argv[0], argv[1],
                                               argv[2], argv[3], argv[4],
                                               argv[5], argv[6]);
#endif
    } else {
        rb_raise(rb_eArgError,
                 "wrong number of arguments (%d for 5 or %d)", argc,
#ifdef HAVE_POPPLER_PAGE_RENDER_SELECTION_TO_PIXBUF
                 8
#else
                 7
#endif
                );
    }
}

#if POPPLER_CHECK_VERSION(0, 5, 9)
static VALUE
page_get_form_fields(VALUE self)
{
    VALUE rb_fields;
    GList *fields;

    fields = poppler_page_get_form_fields(SELF(self));
    rb_fields = GLIST2ARY2(fields, POPPLER_TYPE_FORM_FIELD);
    poppler_page_free_form_fields(fields);
    return rb_fields;
}

static VALUE
page_get_crop_box(VALUE self)
{
    PopplerRectangle rect;

    poppler_page_get_crop_box(SELF(self), &rect);
    return RECT2RVAL(&rect);
}
#endif


#define DEF_READER(prefix, name, member, self_to_c, member_to_rb)       \
static VALUE                                                            \
prefix ## _get_ ## name(VALUE self)                                     \
{                                                                       \
    return member_to_rb((self_to_c(self))->member);                     \
}

#define DEF_WRITER(prefix, name, member, self_to_c, value_to_c) \
static VALUE                                                    \
prefix ## _set_ ## name(VALUE self, VALUE value)                \
{                                                               \
    (self_to_c(self))->member = value_to_c(value);              \
    return Qnil;                                                \
}

#define DEF_WRITER_WITH_SETTER(prefix, name, member,    \
                               self_to_c, value_setter) \
static VALUE                                            \
prefix ## _set_ ## name(VALUE self, VALUE value)        \
{                                                       \
    value_setter((self_to_c(self))->member, value);     \
    return Qnil;                                        \
}

#define DEF_ACCESSOR(prefix, member, self_to_c, member_to_rb, value_to_c) \
    DEF_READER(prefix, member, member, self_to_c, member_to_rb)         \
    DEF_WRITER(prefix, member, member, self_to_c, value_to_c)           \

#define DEF_ACCESSOR_WITH_SETTER(prefix, member, self_to_c,             \
                                 member_to_rb, value_setter)            \
    DEF_READER(prefix, member, member, self_to_c, member_to_rb)         \
    DEF_WRITER_WITH_SETTER(prefix, member, member, self_to_c, value_setter)

#define DEF_ACCESSOR_WITH_NAME(prefix, name, member, self_to_c, \
                               member_to_rb, value_to_c)        \
    DEF_READER(prefix, name, member, self_to_c, member_to_rb)   \
    DEF_WRITER(prefix, name, member, self_to_c, value_to_c)


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

DEF_ACCESSOR(rectangle, x1, RVAL2RECT, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, y1, RVAL2RECT, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, x2, RVAL2RECT, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, y2, RVAL2RECT, rb_float_new, NUM2DBL)

static VALUE
rectangle_to_a(VALUE self)
{
    PopplerRectangle *rectangle = RVAL2RECT(self);
    return rb_ary_new3(4,
                       rb_float_new(rectangle->x1),
                       rb_float_new(rectangle->y1),
                       rb_float_new(rectangle->x2),
                       rb_float_new(rectangle->y2));
}


/* Mapping between areas on the current page and PopplerActions */
#define RECT_ENTITY2RVAL(rect) RECT2RVAL(&(rect))
#define RECT_ENTITY_SET(rect, rb_rect) rectangle_set(&(rect), rb_rect)
static void
rectangle_set(PopplerRectangle *rect, VALUE rb_rect)
{
    *rect = *(RVAL2RECT(rb_rect));
}

DEF_ACCESSOR_WITH_SETTER(link_mapping, area,
                         RVAL2LM, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(link_mapping, action, RVAL2LM, ACTION2RVAL, RVAL2ACTION)

#if POPPLER_CHECK_VERSION(0, 5, 9)

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
DEF_ACCESSOR(image_mapping, image, RVAL2IM, GOBJ2RVAL, RVAL2GDK_PIXBUF)


/* FormField */
DEF_ACCESSOR_WITH_SETTER(form_field, area,
                         RVAL2FF, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(form_field, type, RVAL2FF, FFT2RVAL, RVAL2FFT)
DEF_ACCESSOR(form_field, id, RVAL2FF, INT2NUM, NUM2INT)
DEF_ACCESSOR(form_field, font_size, RVAL2FF, rb_float_new, NUM2DBL)


#define NOT_CBOOL2RVAL(bool) (CBOOL2RVAL(!(bool)))
#define RVAL2NOT_CBOOL(bool) (!(RVAL2CBOOL(bool)))

DEF_ACCESSOR(text_field, multiline, RVAL2TF, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR(text_field, password, RVAL2TF, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR(text_field, fileselect, RVAL2TF, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR_WITH_NAME(text_field, spell_check, do_not_spell_check,
                       RVAL2TF, NOT_CBOOL2RVAL, RVAL2NOT_CBOOL)
DEF_ACCESSOR_WITH_NAME(text_field, scroll, do_not_scroll,
                       RVAL2TF, NOT_CBOOL2RVAL, RVAL2NOT_CBOOL)
DEF_ACCESSOR(text_field, comb, RVAL2TF, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR(text_field, rich_text, RVAL2TF, CBOOL2RVAL, RVAL2CBOOL)

static VALUE
text_field_get_content(VALUE self)
{
    PopplerTextField *field;
    field = RVAL2TF(self);
    return rb_str_new(field->content, field->length);
}

static VALUE
text_field_set_content(VALUE self, VALUE content)
{
    PopplerTextField *field;

    StringValue(content);
    field = RVAL2TF(self);
    if (field->content)
        g_free(field->content);
    field->content = RSTRING_PTR(content);
    field->length = RSTRING_LEN(content);
    return Qnil;
}


DEF_ACCESSOR(button_field, state, RVAL2BF, CBOOL2RVAL, RVAL2CBOOL)


DEF_ACCESSOR(choice_field, combo, RVAL2CF, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR(choice_field, edit, RVAL2CF, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR(choice_field, multi_select, RVAL2CF, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR_WITH_NAME(choice_field, spell_check, do_not_spell_check,
                       RVAL2CF, NOT_CBOOL2RVAL, RVAL2NOT_CBOOL)
DEF_ACCESSOR_WITH_NAME(choice_field, commit_on_change, commit_on_sel_change,
                       RVAL2CF, CBOOL2RVAL, RVAL2CBOOL)
#endif

void
Init_poppler_page(VALUE mPoppler)
{
    VALUE cPage, cRectangle, cLinkMapping;
#if POPPLER_CHECK_VERSION(0, 5, 9)
    VALUE cPageTransition, cImageMapping, cFormField;
    VALUE cTextField, cButtonField, cChoiceField;
#endif

    cPage = G_DEF_CLASS(POPPLER_TYPE_PAGE, "Page", mPoppler);
    cRectangle = G_DEF_CLASS(POPPLER_TYPE_RECTANGLE, "Rectangle", mPoppler);
    cLinkMapping = G_DEF_CLASS(POPPLER_TYPE_LINK_MAPPING, "LinkMapping",
                               mPoppler);
#if POPPLER_CHECK_VERSION(0, 5, 9)
    cPageTransition = G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION,
                                  "PageTransition", mPoppler);
    cImageMapping = G_DEF_CLASS(POPPLER_TYPE_IMAGE_MAPPING,
                                "ImageMapping", mPoppler);
    cFormField = G_DEF_CLASS(POPPLER_TYPE_FORM_FIELD, "FormField", mPoppler);
    cTextField = rb_define_class_under(mPoppler, "TextField", cFormField);
    cButtonField = rb_define_class_under(mPoppler, "ButtonField", cFormField);
    cChoiceField = rb_define_class_under(mPoppler, "ChoiceField", cFormField);
#endif
    cPSFile = rb_const_get(mPoppler, rb_intern("PSFile"));

    rb_define_method(cPage, "render", page_render_generic, -1);
    rb_define_method(cPage, "size", page_get_size, 0);
    rb_define_method(cPage, "index", page_get_index, 0);
#if POPPLER_CHECK_VERSION(0, 5, 9)
    rb_define_method(cPage, "duration", page_get_duration, 0);
    rb_define_method(cPage, "transition", page_get_transition, 0);
#endif
    rb_define_method(cPage, "thumbnail", page_get_thumbnail, 0);
    rb_define_method(cPage, "thumbnail_size", page_get_thumbnail_size, 0);
    rb_define_method(cPage, "find_text", page_find_text, 1);
    rb_define_method(cPage, "get_text", page_get_text, -1);
    rb_define_method(cPage, "link_mapping", page_get_link_mapping, 0);
    rb_define_method(cPage, "get_selection_region",
                     page_get_selection_region, 2);
    rb_define_method(cPage, "render_selection",
                     page_render_selection_generic, -1);
#if POPPLER_CHECK_VERSION(0, 5, 9)
    rb_define_method(cPage, "form_fields", page_get_form_fields, 0);
    rb_define_method(cPage, "crop_box", page_get_crop_box, 0);
#endif

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

    G_DEF_SETTERS(cRectangle);

/* Mapping between areas on the current page and PopplerActions */
    rb_define_method(cLinkMapping, "area", link_mapping_get_area, 0);
    rb_define_method(cLinkMapping, "action", link_mapping_get_action, 0);

    rb_define_method(cLinkMapping, "set_area", link_mapping_set_area, 1);
    rb_define_method(cLinkMapping, "set_action", link_mapping_set_action, 1);

    G_DEF_SETTERS(cLinkMapping);

#if POPPLER_CHECK_VERSION(0, 5, 9)
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
    rb_define_method(cImageMapping, "image", image_mapping_get_image, 0);

    rb_define_method(cImageMapping, "set_area", image_mapping_set_area, 1);
    rb_define_method(cImageMapping, "set_image", image_mapping_set_image, 1);

    G_DEF_SETTERS(cImageMapping);


/* FormField */
    rb_define_method(cFormField, "area", form_field_get_area, 0);
    rb_define_method(cFormField, "type", form_field_get_type, 0);
    rb_define_method(cFormField, "id", form_field_get_id, 0);
    rb_define_method(cFormField, "font_size", form_field_get_font_size, 0);

    rb_define_method(cFormField, "set_area", form_field_set_area, 1);
    rb_define_method(cFormField, "set_type", form_field_set_type, 1);
    rb_define_method(cFormField, "set_id", form_field_set_id, 1);
    rb_define_method(cFormField, "set_font_size", form_field_set_font_size, 1);

    G_DEF_SETTERS(cFormField);


    rb_define_method(cTextField, "multiline?", text_field_get_multiline, 0);
    rb_define_method(cTextField, "password?", text_field_get_password, 0);
    rb_define_method(cTextField, "file_select?", text_field_get_fileselect, 0);
    rb_define_method(cTextField, "spell_check?", text_field_get_spell_check, 0);
    rb_define_method(cTextField, "scroll?", text_field_get_scroll, 0);
    rb_define_method(cTextField, "combo?", text_field_get_comb, 0);
    rb_define_method(cTextField, "rich_text?", text_field_get_rich_text, 0);
    rb_define_method(cTextField, "content", text_field_get_content, 0);

    rb_define_method(cTextField, "set_multiline", text_field_set_multiline, 1);
    rb_define_method(cTextField, "set_password", text_field_set_password, 1);
    rb_define_method(cTextField, "set_file_select",
                     text_field_set_fileselect, 1);
    rb_define_method(cTextField, "set_spell_check",
                     text_field_set_spell_check, 1);
    rb_define_method(cTextField, "set_scroll", text_field_set_scroll, 1);
    rb_define_method(cTextField, "set_combo", text_field_set_comb, 1);
    rb_define_method(cTextField, "set_rich_text", text_field_set_rich_text, 1);
    rb_define_method(cTextField, "set_content", text_field_set_content, 1);

    G_DEF_SETTERS(cTextField);


    rb_define_method(cButtonField, "active?", button_field_get_state, 0);

    rb_define_method(cButtonField, "set_active", button_field_set_state, 1);

    G_DEF_SETTERS(cButtonField);


    rb_define_method(cChoiceField, "combo?", choice_field_get_combo, 0);
    rb_define_method(cChoiceField, "edit?", choice_field_get_edit, 0);
    rb_define_method(cChoiceField, "multi_select?",
                     choice_field_get_multi_select, 0);
    rb_define_method(cChoiceField, "spell_check?",
                     choice_field_get_spell_check, 0);
    rb_define_method(cChoiceField, "commit_on_change?",
                     choice_field_get_commit_on_change, 0);

    rb_define_method(cChoiceField, "set_combo", choice_field_set_combo, 1);
    rb_define_method(cChoiceField, "set_edit", choice_field_set_edit, 1);
    rb_define_method(cChoiceField, "set_multi_select",
                     choice_field_set_multi_select, 1);
    rb_define_method(cChoiceField, "set_spell_check",
                     choice_field_set_spell_check, 1);
    rb_define_method(cChoiceField, "set_commit_on_change",
                     choice_field_set_commit_on_change, 1);

    G_DEF_SETTERS(cChoiceField);
#endif
}
