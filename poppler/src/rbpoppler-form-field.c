/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbpoppler-form-field.c -

  Copyright (C) 2008 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler-private.h"

#define RVAL2FF(obj) RVAL2POPPLER_FORM_FIELD(obj)
#define RVAL2TF(obj) RVAL2FF(obj)
#define RVAL2BF(obj) RVAL2FF(obj)
#define RVAL2CF(obj) RVAL2FF(obj)

#define FFT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_FIELD_TYPE))
#define RVAL2FFT(obj) (RVAL2GENUM(obj, POPPLER_TYPE_FORM_FIELD_TYPE))
#define FBT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_BUTTON_TYPE))
#define FTT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_TEXT_TYPE))
#define FCT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_CHOICE_TYPE))

static VALUE cUnknownField, cTextField, cButtonField;
static VALUE cChoiceField, cSignatureField;

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

/* FormField */
static VALUE
form_field_get_id(VALUE self)
{
    return INT2NUM(poppler_form_field_get_id(RVAL2FF(self)));
}

static VALUE
form_field_get_font_size(VALUE self)
{
    return rb_float_new(poppler_form_field_get_font_size(RVAL2FF(self)));
}

static VALUE
form_field_is_read_only(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_is_read_only(RVAL2FF(self)));
}

/* Button Field */
static VALUE
button_field_get_button_type(VALUE self)
{
    return FBT2RVAL(poppler_form_field_button_get_button_type(RVAL2FF(self)));
}

static VALUE
button_field_get_state(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_button_get_state(RVAL2BF(self)));
}

static VALUE
button_field_set_state(VALUE self, VALUE state)
{
    poppler_form_field_button_set_state(RVAL2BF(self), RVAL2CBOOL(state));
    return Qnil;
}

/* Text Field */
static VALUE
text_field_get_text_type(VALUE self)
{
    return FTT2RVAL(poppler_form_field_text_get_text_type(RVAL2TF(self)));
}

static VALUE
text_field_get_text(VALUE self)
{
    return CSTR2RVAL(poppler_form_field_text_get_text(RVAL2TF(self)));
}

static VALUE
text_field_set_text(VALUE self, VALUE text)
{
    poppler_form_field_text_set_text(RVAL2TF(self), RVAL2CSTR2(text));
    return Qnil;
}

static VALUE
text_field_get_max_length(VALUE self)
{
    return INT2NUM(poppler_form_field_text_get_max_len(RVAL2TF(self)));
}

static VALUE
text_field_do_spell_check(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_do_spell_check(RVAL2TF(self)));
}

static VALUE
text_field_do_scroll(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_do_scroll(RVAL2TF(self)));
}

static VALUE
text_field_is_rich_text(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_is_rich_text(RVAL2TF(self)));
}

static VALUE
text_field_is_password(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_is_password(RVAL2TF(self)));
}


/* Choice Field */
static VALUE
choice_field_get_choice_type(VALUE self)
{
    return FCT2RVAL(poppler_form_field_choice_get_choice_type(RVAL2CF(self)));
}

static VALUE
choice_field_is_editable(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_is_editable(RVAL2CF(self)));
}

static VALUE
choice_field_can_select_multiple(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_can_select_multiple(RVAL2CF(self)));
}

static VALUE
choice_field_do_spell_check(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_do_spell_check(RVAL2CF(self)));
}

static VALUE
choice_field_commit_on_change(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_commit_on_change(RVAL2CF(self)));
}

static VALUE
choice_field_get_n_items(VALUE self)
{
    return INT2NUM(poppler_form_field_choice_get_n_items(RVAL2CF(self)));
}

static VALUE
choice_field_get_item(VALUE self, VALUE index)
{
    return CSTR2RVAL(poppler_form_field_choice_get_item(RVAL2CF(self),
                                                        NUM2INT(index)));
}

static VALUE
choice_field_is_item_selected(VALUE self, VALUE index)
{
    return CBOOL2RVAL(poppler_form_field_choice_is_item_selected(RVAL2CF(self),
                                                                 NUM2INT(index)));
}

static VALUE
choice_field_select_item(VALUE self, VALUE index)
{
    poppler_form_field_choice_select_item(RVAL2CF(self), NUM2INT(index));
    return Qnil;
}

static VALUE
choice_field_unselect_all(VALUE self)
{
    poppler_form_field_choice_unselect_all(RVAL2CF(self));
    return Qnil;
}

static VALUE
choice_field_toggle_item(VALUE self, VALUE index)
{
    poppler_form_field_choice_toggle_item(RVAL2CF(self), NUM2INT(index));
    return Qnil;
}

static VALUE
choice_field_set_text(VALUE self, VALUE text)
{
    poppler_form_field_choice_set_text(RVAL2CF(self), RVAL2CSTR2(text));
    return Qnil;
}

static VALUE
choice_field_get_text(VALUE self)
{
    return CSTR2RVAL(poppler_form_field_choice_get_text(RVAL2CF(self)));
}

void
Init_poppler_form_field(VALUE mPoppler)
{
    VALUE cFormField;

    cFormField = G_DEF_CLASS(POPPLER_TYPE_FORM_FIELD, "FormField", mPoppler);
    cUnknownField = rb_define_class_under(mPoppler, "UnknownField", cFormField);
    cTextField = rb_define_class_under(mPoppler, "TextField", cFormField);
    cButtonField = rb_define_class_under(mPoppler, "ButtonField", cFormField);
    cChoiceField = rb_define_class_under(mPoppler, "ChoiceField", cFormField);
    cSignatureField = rb_define_class_under(mPoppler, "SignatureField",
                                            cFormField);

/* FormField */
    rb_define_method(cFormField, "id", form_field_get_id, 0);
    rb_define_method(cFormField, "font_size", form_field_get_font_size, 0);
    rb_define_method(cFormField, "read_only?", form_field_is_read_only, 0);

    G_DEF_SETTERS(cFormField);


    rb_define_method(cButtonField, "type", button_field_get_button_type, 0);
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
    rb_define_method(cChoiceField, "toggle", choice_field_toggle_item, 1);
    rb_define_method(cChoiceField, "text", choice_field_get_text, 0);
    rb_define_method(cChoiceField, "set_text", choice_field_set_text, 1);

    G_DEF_SETTERS(cChoiceField);
}
