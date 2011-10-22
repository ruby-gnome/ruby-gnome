/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#include "rbatkprivate.h"

#define _SELF(s) (ATK_TEXT(RVAL2GOBJ(s)))

static VALUE
rbatk_text_get_text(VALUE self, VALUE start_offset, VALUE end_offset)
{
    VALUE ret;
    gchar* text = atk_text_get_text(_SELF(self), 
                                    NUM2INT(start_offset), 
                                    NUM2INT(end_offset));
    ret = CSTR2RVAL(text);
    g_free(text);
    return ret;
}

static VALUE
rbatk_text_get_character_at_offset(VALUE self, VALUE offset)
{
    gchar buf[10];
    gint len = g_unichar_to_utf8(atk_text_get_character_at_offset
                                 (_SELF(self), NUM2INT(offset)), buf);
    buf[len] = '\0';
    return rb_str_new2(buf);
}

static VALUE
rbatk_text_get_text_after_offset(VALUE self, VALUE offset, VALUE boundary_type)
{
    gchar* ret;
    VALUE result;
    gint start_offset, end_offset;

    ret = atk_text_get_text_after_offset(_SELF(self), NUM2INT(offset),
                                         RVAL2GENUM(boundary_type, ATK_TYPE_TEXT_BOUNDARY),
                                         &start_offset, &end_offset);
    result = rb_ary_new3(3, CSTR2RVAL(ret), 
                         INT2NUM(start_offset), INT2NUM(end_offset));
    g_free(ret);

    return result;
}

static VALUE
rbatk_text_get_text_at_offset(VALUE self, VALUE offset, VALUE boundary_type)
{
    gchar* ret;
    VALUE result;
    gint start_offset, end_offset;

    ret = atk_text_get_text_at_offset(_SELF(self), NUM2INT(offset),
                                      RVAL2GENUM(boundary_type, ATK_TYPE_TEXT_BOUNDARY),
                                      &start_offset, &end_offset);
    result = rb_ary_new3(3, CSTR2RVAL(ret), 
                         INT2NUM(start_offset), INT2NUM(end_offset));
    g_free(ret);

    return result;
}

static VALUE
rbatk_text_get_text_before_offset(VALUE self, VALUE offset, VALUE boundary_type)
{
    gchar* ret;
    VALUE result;
    gint start_offset, end_offset;

    ret = atk_text_get_text_before_offset(_SELF(self), NUM2INT(offset),
                                          RVAL2GENUM(boundary_type, ATK_TYPE_TEXT_BOUNDARY),
                                          &start_offset, &end_offset);
    result = rb_ary_new3(3, CSTR2RVAL(ret), 
                         INT2NUM(start_offset), INT2NUM(end_offset));
    g_free(ret);

    return result;
}

static VALUE
rbatk_text_get_caret_offset(VALUE self)
{
    return INT2NUM(atk_text_get_caret_offset(_SELF(self)));
}

static VALUE
rbatk_text_get_character_extents(VALUE self, VALUE offset, VALUE coords)
{
    gint x, y, width, height;
    atk_text_get_character_extents(_SELF(self), NUM2INT(offset), 
                                   &x, &y, &width, &height, 
                                   RVAL2GENUM(coords, ATK_TYPE_COORD_TYPE));
    return rb_ary_new3(4, INT2NUM(x), INT2NUM(y), INT2NUM(width), INT2NUM(height));
}

static VALUE
rbatk_text_get_run_attributes(VALUE self, VALUE offset)
{
    gint start_offset, end_offset;
    AtkAttributeSet* list;
    VALUE ary;

    list = atk_text_get_run_attributes(_SELF(self), NUM2INT(offset),
                                       &start_offset, &end_offset);

    ary = rb_ary_new();
  
    while (list) {
        AtkAttribute* data = (AtkAttribute*)list->data;
        rb_ary_push(ary, rb_assoc_new(CSTR2RVAL(data->name), 
                                      CSTR2RVAL(data->value)));
        list = list->next;
    }
    atk_attribute_set_free(list);
    return ary;
}

static VALUE
rbatk_text_get_default_attributes(VALUE self)
{
    AtkAttributeSet* list;
    VALUE ary;

    list = atk_text_get_default_attributes(_SELF(self));
    ary = rb_ary_new();
  
    while (list) {
        AtkAttribute* data = (AtkAttribute*)list->data;
        rb_ary_push(ary, rb_assoc_new(CSTR2RVAL(data->name), 
                                      CSTR2RVAL(data->value)));
        list = list->next;
    }
    atk_attribute_set_free(list);
    return ary;
}

static VALUE
rbatk_text_get_character_count(VALUE self)
{
    return INT2NUM(atk_text_get_character_count(_SELF(self)));
}

static VALUE
rbatk_text_get_offset_at_point(VALUE self, VALUE x, VALUE y, VALUE coords)
{
    return INT2NUM(atk_text_get_offset_at_point(_SELF(self), 
                                                NUM2INT(x), NUM2INT(y),
                                                RVAL2GENUM(coords, ATK_TYPE_COORD_TYPE)));
}

#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#ifdef HAVE_ATK_TEXT_CLIP_TYPE_GET_TYPE
static VALUE
rbatk_text_get_bounded_ranges(VALUE self, VALUE rect, VALUE coord_type, VALUE x_clip_type, VALUE y_clip_type)
{
    AtkTextRange** ranges;
    int i = 0;
    VALUE ary;
    ranges = atk_text_get_bounded_ranges(_SELF(self),
                                         RVAL2BOXED(rect, ATK_TYPE_TEXT_RECTANGLE),
                                         RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE),
                                         RVAL2GENUM(x_clip_type, ATK_TYPE_TEXT_CLIP_TYPE),
                                         RVAL2GENUM(y_clip_type, ATK_TYPE_TEXT_CLIP_TYPE));
    ary = rb_ary_new();
    while(ranges[i]){
        rb_ary_push(ary, BOXED2RVAL(ranges[i], ATK_TYPE_TEXT_RANGE));
        i++;
    }
#ifdef HAVE_ATK_TEXT_FREE_RANGES
    atk_text_free_ranges(ranges);
#endif
    return ary;
}
#endif

static VALUE
rbatk_text_get_range_extents(VALUE self, VALUE start_offset, VALUE end_offset, VALUE coord_type)
{
    AtkTextRectangle rect;
    atk_text_get_range_extents(_SELF(self), NUM2INT(start_offset),
                               NUM2INT(end_offset), 
                               RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE),
                               &rect);
    return BOXED2RVAL(&rect, ATK_TYPE_TEXT_RECTANGLE);
}

/* Don't need this
void        atk_text_free_ranges            (AtkTextRange **ranges);
*/
#endif

static VALUE
rbatk_text_get_n_selections(VALUE self)
{
    return INT2NUM(atk_text_get_n_selections(_SELF(self)));
}

static VALUE
rbatk_text_get_selection(VALUE self, VALUE selection_num)
{
    gint start_offset, end_offset;
    VALUE ret;
    gchar* text = atk_text_get_selection(_SELF(self), NUM2INT(selection_num),
                                         &start_offset, &end_offset);
    ret = CSTR2RVAL(text);
    g_free(text);

    return ret;
}

static VALUE
rbatk_text_add_selection(VALUE self, VALUE start_offset, VALUE end_offset)
{
    gboolean ret = atk_text_add_selection(_SELF(self), NUM2INT(start_offset),
                                          NUM2INT(end_offset));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't add selection");
    return self;
}

static VALUE
rbatk_text_remove_selection(VALUE self, VALUE selection_num)
{
    gint num;
    gboolean ret;

    num = NUM2INT(selection_num);
    ret = atk_text_remove_selection(_SELF(self), num);
    if (! ret)
        rb_raise(rb_eRuntimeError, "Can't remove selection. num = %d", num);
    return self;
}

static VALUE
rbatk_text_set_selection(VALUE self, VALUE selection_num, VALUE start_offset, VALUE end_offset)
{
    gboolean ret = atk_text_set_selection(_SELF(self), NUM2INT(selection_num),
                                          NUM2INT(start_offset),
                                          NUM2INT(end_offset));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set selection");
    return self;
}

static VALUE
rbatk_text_set_caret_offset(VALUE self, VALUE offset)
{
    gboolean ret = atk_text_set_caret_offset(_SELF(self), NUM2INT(offset));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set caret offset");
    return self;
}


/* We don't need them.
    void        atk_attribute_set_free          (AtkAttributeSet *attrib_set);
*/

/*
 * Atk::TextAttribute
 */
static VALUE
rbatk_tattr_s_register(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_text_attribute_register(RVAL2CSTR(name)), ATK_TYPE_TEXT_ATTRIBUTE);
}

/* We don't need this.
G_CONST_RETURN gchar* atk_textattribute_type_get_name
                                            (AtkTextattributeType type);
*/

static VALUE
rbatk_tattr_s_for_name(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_text_attribute_for_name(RVAL2CSTR(name)), ATK_TYPE_TEXT_ATTRIBUTE);
}

static VALUE
rbatk_tattr_get_value(VALUE self, VALUE index)
{
    return CSTR2RVAL(atk_text_attribute_get_value(RVAL2GENUM(self, ATK_TYPE_TEXT_ATTRIBUTE), 
                                                  NUM2INT(index)));
}
void
Init_atk_text(void)
{
    VALUE tattr;
    VALUE mText = G_DEF_INTERFACE(ATK_TYPE_TEXT, "Text", mAtk);

    rb_define_method(mText, "get_text", rbatk_text_get_text, 2);
    rb_define_method(mText, "get_character_at_offset", rbatk_text_get_character_at_offset, 1);
    rb_define_method(mText, "get_text_after_offset", rbatk_text_get_text_after_offset, 2);
    rb_define_method(mText, "get_text_at_offset", rbatk_text_get_text_at_offset, 2);
    rb_define_method(mText, "get_text_before_offset", rbatk_text_get_text_before_offset, 2);
    rb_define_method(mText, "caret_offset", rbatk_text_get_caret_offset, 0);
    rb_define_method(mText, "get_character_extents", rbatk_text_get_character_extents, 2);
    rb_define_method(mText, "get_run_attributes", rbatk_text_get_run_attributes, 1);
    rb_define_method(mText, "default_attributes", rbatk_text_get_default_attributes, 0);
    rb_define_method(mText, "character_count", rbatk_text_get_character_count, 0);
    rb_define_method(mText, "get_offset_at_point", rbatk_text_get_offset_at_point, 3);
#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#ifdef HAVE_ATK_TEXT_CLIP_TYPE_GET_TYPE
    rb_define_method(mText, "get_bounded_ranges", rbatk_text_get_bounded_ranges, 4);
    rb_define_method(mText, "get_range_extents", rbatk_text_get_range_extents, 3);
#endif
#endif
    rb_define_method(mText, "n_selections", rbatk_text_get_n_selections, 0);
    rb_define_method(mText, "get_selection", rbatk_text_get_selection, 1);
    rb_define_method(mText, "add_selection", rbatk_text_add_selection, 2);
    rb_define_method(mText, "remove_selection", rbatk_text_remove_selection, 1);
    rb_define_method(mText, "set_selection", rbatk_text_set_selection, 3);
    rb_define_method(mText, "set_caret_offset", rbatk_text_set_caret_offset, 1);

    G_DEF_SETTERS(mText);
    
    /* AtkTextBoundary */
#ifdef ATK_TYPE_TEXT_BOUNDARY
    G_DEF_CLASS(ATK_TYPE_TEXT_BOUNDARY, "Boundary", mText);
    G_DEF_CONSTANTS(mText, ATK_TYPE_TEXT_BOUNDARY, "ATK_TEXT_");
#endif    
    /* AtkTextClipType */
#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#ifdef HAVE_ATK_TEXT_CLIP_TYPE_GET_TYPE
    G_DEF_CLASS(ATK_TYPE_TEXT_CLIP_TYPE, "ClipType", mText);
    G_DEF_CONSTANTS(mText, ATK_TYPE_TEXT_CLIP_TYPE, "ATK_TEXT_");
#endif
#endif

    tattr = G_DEF_CLASS(ATK_TYPE_TEXT_ATTRIBUTE, "Attribute", mText);
    G_DEF_CONSTANTS(mText, ATK_TYPE_TEXT_ATTRIBUTE, "ATK_TEXT_");

    rb_define_singleton_method(tattr, "type_register", rbatk_tattr_s_register, 1);
    rb_define_singleton_method(tattr, "for_name", rbatk_tattr_s_for_name, 1);
    rb_define_method(tattr, "get_value", rbatk_tattr_get_value, 1);   
}
