/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbpoppler-page.c -

  $Author: ktou $
  $Date: 2007/07/27 14:37:01 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"

#define SELF(self) (POPPLER_PAGE(RVAL2GOBJ(self)))
#define RVAL2LM(obj) ((PopplerLinkMapping *)RVAL2BOXED(obj, POPPLER_TYPE_LINK_MAPPING))
#define RVAL2IM(obj) ((PopplerImageMapping *)RVAL2BOXED(obj, POPPLER_TYPE_IMAGE_MAPPING))
#define RVAL2FFM(obj) ((PopplerFormFieldMapping *)RVAL2BOXED(obj, POPPLER_TYPE_FORM_FIELD_MAPPING))
#define RVAL2FF(obj) (POPPLER_FORM_FIELD(RVAL2GOBJ(obj)))
#define RVAL2TF(obj) RVAL2FF(obj)
#define RVAL2BF(obj) RVAL2FF(obj)
#define RVAL2CF(obj) RVAL2FF(obj)

#define TT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_TYPE))
#define RVAL2TT(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_TYPE))
#define TA2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT))
#define RVAL2TA(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT))
#define TD2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_PAGE_TRANSITION_DIRECTION))
#define RVAL2TD(obj) (RVAL2GENUM(obj, POPPLER_TYPE_PAGE_TRANSITION_DIRECTION))
#define FFT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_FIELD_TYPE))
#define RVAL2FFT(obj) (RVAL2GENUM(obj, POPPLER_TYPE_FORM_FIELD_TYPE))
#define FBT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_BUTTON_TYPE))
#define FTT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_TEXT_TYPE))
#define FCT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_CHOICE_TYPE))

static VALUE cPSFile;

#if POPPLER_CHECK_VERSION(0, 5, 9)
VALUE cUnknownField, cTextField, cButtonField, cChoiceField, cSignatureField;
#endif

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
page_get_image_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_image_mapping(SELF(self)),
                       POPPLER_TYPE_IMAGE_MAPPING);
}

static VALUE
page_get_form_field_mapping(VALUE self)
{
    return GLIST2ARY2F(poppler_page_get_form_field_mapping(SELF(self)),
                       POPPLER_TYPE_FORM_FIELD_MAPPING);
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


/* Mapping between areas on the current page and form fields */
VALUE
rb_poppler_ruby_object_from_form_field(PopplerFormField *field)
{
    VALUE obj;

    obj = rbgobj_ruby_object_from_instance2(field, FALSE);
    if (NIL_P(obj)) {
      switch (poppler_form_field_get_field_type(field)) {
        case POPPLER_FORM_FIELD_UNKNOWN:
          obj = rbgobj_create_object(cUnknownField);
          break;
        case POPPLER_FORM_FIELD_BUTTON:
          obj = rbgobj_create_object(cButtonField);
          break;
        case POPPLER_FORM_FIELD_TEXT:
          obj = rbgobj_create_object(cTextField);
          break;
        case POPPLER_FORM_FIELD_CHOICE:
          obj = rbgobj_create_object(cChoiceField);
          break;
        case POPPLER_FORM_FIELD_SIGNATURE:
          obj = rbgobj_create_object(cSignatureField);
          break;
      }
      g_object_ref(field);
      G_INITIALIZE(obj, (gpointer)field);
    }

    return obj;
}

DEF_ACCESSOR_WITH_SETTER(form_field_mapping, area,
                         RVAL2FFM, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(form_field_mapping, field, RVAL2FFM, FF2RVAL, RVAL2FF)

/* FormField */
VALUE
form_field_get_field_type(VALUE self)
{
    return FFT2RVAL(poppler_form_field_get_field_type(RVAL2FF(self)));
}

VALUE
form_field_get_id(VALUE self)
{
    return INT2NUM(poppler_form_field_get_id(RVAL2FF(self)));
}

VALUE
form_field_get_font_size(VALUE self)
{
    return rb_float_new(poppler_form_field_get_font_size(RVAL2FF(self)));
}

VALUE
form_field_is_read_only(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_is_read_only(RVAL2FF(self)));
}

/* Button Field */
VALUE
button_field_get_button_type(VALUE self)
{
    return FBT2RVAL(poppler_form_field_button_get_button_type(RVAL2FF(self)));
}

VALUE
button_field_get_state(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_button_get_state(RVAL2BF(self)));
}

VALUE
button_field_set_state(VALUE self, VALUE state)
{
    poppler_form_field_button_set_state(RVAL2BF(self), RVAL2CBOOL(state));
    return Qnil;
}

/* Text Field */
VALUE
text_field_get_text_type(VALUE self)
{
    return FTT2RVAL(poppler_form_field_text_get_text_type(RVAL2TF(self)));
}

VALUE
text_field_get_text(VALUE self)
{
    return CSTR2RVAL(poppler_form_field_text_get_text(RVAL2TF(self)));
}

VALUE
text_field_set_text(VALUE self, VALUE text)
{
    poppler_form_field_text_set_text(RVAL2TF(self), RVAL2CSTR2(text));
    return Qnil;
}

VALUE
text_field_get_max_length(VALUE self)
{
    return INT2NUM(poppler_form_field_text_get_max_len(RVAL2TF(self)));
}

VALUE
text_field_do_spell_check(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_do_spell_check(RVAL2TF(self)));
}

VALUE
text_field_do_scroll(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_do_scroll(RVAL2TF(self)));
}

VALUE
text_field_is_rich_text(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_is_rich_text(RVAL2TF(self)));
}

VALUE
text_field_is_password(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_is_password(RVAL2TF(self)));
}


/* Choice Field */
VALUE
choice_field_get_choice_type(VALUE self)
{
    return FCT2RVAL(poppler_form_field_choice_get_choice_type(RVAL2CF(self)));
}

VALUE
choice_field_is_editable(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_is_editable(RVAL2CF(self)));
}

VALUE
choice_field_can_select_multiple(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_can_select_multiple(RVAL2CF(self)));
}

VALUE
choice_field_do_spell_check(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_do_spell_check(RVAL2CF(self)));
}

VALUE
choice_field_commit_on_change(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_commit_on_change(RVAL2CF(self)));
}

VALUE
choice_field_get_n_items(VALUE self)
{
    return INT2NUM(poppler_form_field_choice_get_n_items(RVAL2CF(self)));
}

VALUE
choice_field_get_item(VALUE self, VALUE index)
{
    return CSTR2RVAL(poppler_form_field_choice_get_item(RVAL2CF(self),
                                                        NUM2INT(index)));
}

VALUE
choice_field_is_item_selected(VALUE self, VALUE index)
{
    return CBOOL2RVAL(poppler_form_field_choice_is_item_selected(RVAL2CF(self),
                                                                 NUM2INT(index)));
}

VALUE
choice_field_select_item(VALUE self, VALUE index)
{
    poppler_form_field_choice_select_item(RVAL2CF(self), NUM2INT(index));
    return Qnil;
}

VALUE
choice_field_unselect_all(VALUE self)
{
    poppler_form_field_choice_unselect_all(RVAL2CF(self));
    return Qnil;
}

VALUE
choice_field_toggle_item(VALUE self, VALUE index)
{
    poppler_form_field_choice_toggle_item(RVAL2CF(self), NUM2INT(index));
    return Qnil;
}

VALUE
choice_field_set_text(VALUE self, VALUE text)
{
    poppler_form_field_choice_set_text(RVAL2CF(self), RVAL2CSTR2(text));
    return Qnil;
}

VALUE
choice_field_get_text(VALUE self)
{
    return CSTR2RVAL(poppler_form_field_choice_get_text(RVAL2CF(self)));
}
#endif

void
Init_poppler_page(VALUE mPoppler)
{
    VALUE cPage, cRectangle, cLinkMapping;
#if POPPLER_CHECK_VERSION(0, 5, 9)
    VALUE cPageTransition, cImageMapping, cFormFieldMapping, cFormField;
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
    cFormFieldMapping = G_DEF_CLASS(POPPLER_TYPE_FORM_FIELD_MAPPING,
                                    "FormFieldMapping", mPoppler);

    G_DEF_CLASS(POPPLER_TYPE_FORM_BUTTON_TYPE, "FormButtonType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_FORM_TEXT_TYPE, "FormTextType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_FORM_CHOICE_TYPE, "FormChoiceType", mPoppler);

    cFormField = G_DEF_CLASS(POPPLER_TYPE_FORM_FIELD, "FormField", mPoppler);
    cUnknownField = rb_define_class_under(mPoppler, "UnknownField", cFormField);
    cTextField = rb_define_class_under(mPoppler, "TextField", cFormField);
    cButtonField = rb_define_class_under(mPoppler, "ButtonField", cFormField);
    cChoiceField = rb_define_class_under(mPoppler, "ChoiceField", cFormField);
    cSignatureField = rb_define_class_under(mPoppler, "SignatureField",
                                            cFormField);
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
#if POPPLER_CHECK_VERSION(0, 5, 9)
    rb_define_method(cPage, "image_mapping", page_get_image_mapping, 0);
    rb_define_method(cPage, "form_field_mapping",
                     page_get_form_field_mapping, 0);
#endif
    rb_define_method(cPage, "get_selection_region",
                     page_get_selection_region, 2);
    rb_define_method(cPage, "render_selection",
                     page_render_selection_generic, -1);
#if POPPLER_CHECK_VERSION(0, 5, 9)
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


/* Mapping between areas on the current page and form fields */
    rb_define_method(cFormFieldMapping, "area", form_field_mapping_get_area, 0);
    rb_define_method(cFormFieldMapping, "field", form_field_mapping_get_field,
                     0);

    rb_define_method(cFormFieldMapping, "set_area",
                     form_field_mapping_set_area, 1);
    rb_define_method(cFormFieldMapping, "set_field",
                     form_field_mapping_set_field, 1);

    G_DEF_SETTERS(cFormFieldMapping);

/* FormField */
    rb_define_method(cFormField, "id", form_field_get_id, 0);
    rb_define_method(cFormField, "font_size", form_field_get_font_size, 0);
    rb_define_method(cFormField, "read_only?", form_field_is_read_only, 0);

    G_DEF_SETTERS(cFormField);


    rb_define_method(cButtonField, "active?", button_field_get_state, 0);
    rb_define_method(cButtonField, "set_active", button_field_set_state, 1);

    G_DEF_SETTERS(cButtonField);


    rb_define_method(cTextField, "type", text_field_get_text_type, 0);
    rb_define_method(cTextField, "text", text_field_get_text, 0);
    rb_define_method(cTextField, "set_text", text_field_set_text, 1);
    rb_define_method(cTextField, "max_length", text_field_get_max_length, 0);
    rb_define_method(cTextField, "spell_check?", text_field_do_spell_check, 0);
    rb_define_method(cTextField, "scroll?", text_field_do_scroll, 0);
    rb_define_method(cTextField, "rich_text?", text_field_is_rich_text, 0);
    rb_define_method(cTextField, "password?", text_field_is_password, 0);

    G_DEF_SETTERS(cTextField);


    rb_define_method(cChoiceField, "type", choice_field_get_choice_type, 0);
    rb_define_method(cChoiceField, "editable?", choice_field_is_editable, 0);
    rb_define_method(cChoiceField, "select_multiple?",
                     choice_field_can_select_multiple, 0);
    rb_define_method(cChoiceField, "spell_check?",
                     choice_field_do_spell_check, 0);
    rb_define_method(cChoiceField, "commit_on_change?",
                     choice_field_commit_on_change, 0);
    rb_define_method(cChoiceField, "n_items", choice_field_get_n_items, 0);
    rb_define_method(cChoiceField, "[]", choice_field_get_item, 1);
    rb_define_method(cChoiceField, "selected?",
                     choice_field_is_item_selected, 1);
    rb_define_method(cChoiceField, "select", choice_field_select_item, 1);
    rb_define_method(cChoiceField, "unselect_all", choice_field_unselect_all, 0);
    rb_define_method(cChoiceField, "text", choice_field_get_text, 0);
    rb_define_method(cChoiceField, "set_text", choice_field_set_text, 1);

    G_DEF_SETTERS(cChoiceField);
#endif
}
