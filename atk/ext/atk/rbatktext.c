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

#define RG_TARGET_NAMESPACE mText
#define _SELF(s) (RVAL2ATKTEXT(s))

static VALUE
rg_get_text(VALUE self, VALUE start_offset, VALUE end_offset)
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
rg_get_character_at_offset(VALUE self, VALUE offset)
{
    gchar buf[10];
    gint len = g_unichar_to_utf8(atk_text_get_character_at_offset
                                 (_SELF(self), NUM2INT(offset)), buf);
    buf[len] = '\0';
    return rb_str_new2(buf);
}

static VALUE
rg_get_text_after_offset(VALUE self, VALUE offset, VALUE boundary_type)
{
    gchar* ret;
    VALUE result;
    gint start_offset, end_offset;

    ret = atk_text_get_text_after_offset(_SELF(self), NUM2INT(offset),
                                         RVAL2ATKTEXTBOUNDARY(boundary_type),
                                         &start_offset, &end_offset);
    result = rb_ary_new3(3, CSTR2RVAL(ret), 
                         INT2NUM(start_offset), INT2NUM(end_offset));
    g_free(ret);

    return result;
}

static VALUE
rg_get_text_at_offset(VALUE self, VALUE offset, VALUE boundary_type)
{
    gchar* ret;
    VALUE result;
    gint start_offset, end_offset;

    ret = atk_text_get_text_at_offset(_SELF(self), NUM2INT(offset),
                                      RVAL2ATKTEXTBOUNDARY(boundary_type),
                                      &start_offset, &end_offset);
    result = rb_ary_new3(3, CSTR2RVAL(ret), 
                         INT2NUM(start_offset), INT2NUM(end_offset));
    g_free(ret);

    return result;
}

static VALUE
rg_get_text_before_offset(VALUE self, VALUE offset, VALUE boundary_type)
{
    gchar* ret;
    VALUE result;
    gint start_offset, end_offset;

    ret = atk_text_get_text_before_offset(_SELF(self), NUM2INT(offset),
                                          RVAL2ATKTEXTBOUNDARY(boundary_type),
                                          &start_offset, &end_offset);
    result = rb_ary_new3(3, CSTR2RVAL(ret), 
                         INT2NUM(start_offset), INT2NUM(end_offset));
    g_free(ret);

    return result;
}

static VALUE
rg_caret_offset(VALUE self)
{
    return INT2NUM(atk_text_get_caret_offset(_SELF(self)));
}

static VALUE
rg_get_character_extents(VALUE self, VALUE offset, VALUE coords)
{
    gint x, y, width, height;
    atk_text_get_character_extents(_SELF(self), NUM2INT(offset), 
                                   &x, &y, &width, &height, 
                                   RVAL2ATKCOORDTYPE(coords));
    return rb_ary_new3(4, INT2NUM(x), INT2NUM(y), INT2NUM(width), INT2NUM(height));
}

static VALUE
rg_get_run_attributes(VALUE self, VALUE offset)
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
rg_default_attributes(VALUE self)
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
rg_character_count(VALUE self)
{
    return INT2NUM(atk_text_get_character_count(_SELF(self)));
}

static VALUE
rg_get_offset_at_point(VALUE self, VALUE x, VALUE y, VALUE coords)
{
    return INT2NUM(atk_text_get_offset_at_point(_SELF(self), 
                                                NUM2INT(x), NUM2INT(y),
                                                RVAL2ATKCOORDTYPE(coords)));
}

#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#ifdef HAVE_ATK_TEXT_CLIP_TYPE_GET_TYPE
static VALUE
rg_get_bounded_ranges(VALUE self, VALUE rect, VALUE coord_type, VALUE x_clip_type, VALUE y_clip_type)
{
    AtkTextRange** ranges;
    int i = 0;
    VALUE ary;
    ranges = atk_text_get_bounded_ranges(_SELF(self),
                                         RVAL2ATKTEXTRECTANGLE(rect),
                                         RVAL2ATKCOORDTYPE(coord_type),
                                         RVAL2ATKTEXTCLIPTYPE(x_clip_type),
                                         RVAL2ATKTEXTCLIPTYPE(y_clip_type));
    ary = rb_ary_new();
    while(ranges[i]){
        rb_ary_push(ary, ATKTEXTRANGE2RVAL(ranges[i]));
        i++;
    }
#ifdef HAVE_ATK_TEXT_FREE_RANGES
    atk_text_free_ranges(ranges);
#endif
    return ary;
}
#endif

static VALUE
rg_get_range_extents(VALUE self, VALUE start_offset, VALUE end_offset, VALUE coord_type)
{
    AtkTextRectangle rect;
    atk_text_get_range_extents(_SELF(self), NUM2INT(start_offset),
                               NUM2INT(end_offset), 
                               RVAL2ATKCOORDTYPE(coord_type),
                               &rect);
    return ATKTEXTRECTANGLE2RVAL(&rect);
}

/* Don't need this
void        atk_text_free_ranges            (AtkTextRange **ranges);
*/
#endif

static VALUE
rg_n_selections(VALUE self)
{
    return INT2NUM(atk_text_get_n_selections(_SELF(self)));
}

static VALUE
rg_get_selection(VALUE self, VALUE selection_num)
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
rg_add_selection(VALUE self, VALUE start_offset, VALUE end_offset)
{
    gboolean ret = atk_text_add_selection(_SELF(self), NUM2INT(start_offset),
                                          NUM2INT(end_offset));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't add selection");
    return self;
}

static VALUE
rg_remove_selection(VALUE self, VALUE selection_num)
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
rg_set_selection(VALUE self, VALUE selection_num, VALUE start_offset, VALUE end_offset)
{
    gboolean ret = atk_text_set_selection(_SELF(self), NUM2INT(selection_num),
                                          NUM2INT(start_offset),
                                          NUM2INT(end_offset));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set selection");
    return self;
}

static VALUE
rg_set_caret_offset(VALUE self, VALUE offset)
{
    gboolean ret = atk_text_set_caret_offset(_SELF(self), NUM2INT(offset));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set caret offset");
    return self;
}

/* We don't need them.
    void        atk_attribute_set_free          (AtkAttributeSet *attrib_set);
*/

void
Init_atk_text(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_TEXT, "Text", mAtk);

    RG_DEF_METHOD(get_text, 2);
    RG_DEF_METHOD(get_character_at_offset, 1);
    RG_DEF_METHOD(get_text_after_offset, 2);
    RG_DEF_METHOD(get_text_at_offset, 2);
    RG_DEF_METHOD(get_text_before_offset, 2);
    RG_DEF_METHOD(caret_offset, 0);
    RG_DEF_METHOD(get_character_extents, 2);
    RG_DEF_METHOD(get_run_attributes, 1);
    RG_DEF_METHOD(default_attributes, 0);
    RG_DEF_METHOD(character_count, 0);
    RG_DEF_METHOD(get_offset_at_point, 3);
#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#ifdef HAVE_ATK_TEXT_CLIP_TYPE_GET_TYPE
    RG_DEF_METHOD(get_bounded_ranges, 4);
    RG_DEF_METHOD(get_range_extents, 3);
#endif
#endif
    RG_DEF_METHOD(n_selections, 0);
    RG_DEF_METHOD(get_selection, 1);
    RG_DEF_METHOD(add_selection, 2);
    RG_DEF_METHOD(remove_selection, 1);
    RG_DEF_METHOD(set_selection, 3);
    RG_DEF_METHOD(set_caret_offset, 1);

    /* AtkTextBoundary */
#ifdef ATK_TYPE_TEXT_BOUNDARY
    G_DEF_CLASS(ATK_TYPE_TEXT_BOUNDARY, "Boundary", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, ATK_TYPE_TEXT_BOUNDARY, "ATK_TEXT_");
#endif    
    /* AtkTextClipType */
#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#ifdef HAVE_ATK_TEXT_CLIP_TYPE_GET_TYPE
    G_DEF_CLASS(ATK_TYPE_TEXT_CLIP_TYPE, "ClipType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, ATK_TYPE_TEXT_CLIP_TYPE, "ATK_TEXT_");
#endif
#endif

    Init_atk_text_attribute(RG_TARGET_NAMESPACE);
}
