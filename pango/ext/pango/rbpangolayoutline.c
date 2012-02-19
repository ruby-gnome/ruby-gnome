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

#define RG_TARGET_NAMESPACE cLayoutLine
#define _SELF(r) (RVAL2PANGOLAYOUTLINE(r))

/**********************************/
#if !PANGO_CHECK_VERSION(1,9,0)
static PangoLayoutLine*
layout_line_copy(PangoLayoutLine *ref)
{
  g_return_val_if_fail (ref != NULL, NULL);
  pango_layout_line_ref(ref);
  return ref;
}

GType
pango_layout_line_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoLayoutLine",
                    (GBoxedCopyFunc)layout_line_copy,
                    (GBoxedFreeFunc)pango_layout_line_unref);
    return our_type;
}
#endif
/**********************************/

static VALUE
rg_extents(VALUE self)
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_line_get_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(PANGORECTANGLE2RVAL(&ink_rect),
                        PANGORECTANGLE2RVAL(&logical_rect));
}

static VALUE
rg_pixel_extents(VALUE self)
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_line_get_pixel_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(PANGORECTANGLE2RVAL(&ink_rect),
                        PANGORECTANGLE2RVAL(&logical_rect));
}

static VALUE
rg_index_to_x(VALUE self, VALUE index, VALUE trailing)
{
    int x_pos;
    pango_layout_line_index_to_x(_SELF(self), NUM2INT(index), 
                                 RVAL2CBOOL(trailing), &x_pos);
    return INT2NUM(x_pos);
}

static VALUE
rg_x_to_index(VALUE self, VALUE x_pos)
{
    int index, trailing;

    gboolean ret = pango_layout_line_x_to_index(_SELF(self), NUM2INT(x_pos),
                                                &index, &trailing);
    return rb_ary_new3(CBOOL2RVAL(ret), INT2NUM(index), INT2NUM(trailing));
}

static VALUE
rg_get_x_ranges(VALUE self, VALUE start_index, VALUE end_index)
{
    int* ranges;
    int i, n_ranges;
    VALUE ary;

    pango_layout_line_get_x_ranges(_SELF(self), NUM2INT(start_index),
                                   NUM2INT(end_index), &ranges, &n_ranges);

    ary = rb_ary_new();
    for (i = 0; i < n_ranges; i++) {
        rb_ary_push(ary, INT2NUM(ranges[i]));
    }

    g_free(ranges);
    return ary;
}

/* Structure members */
static VALUE
rg_layout(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->layout);
}

static VALUE
rg_set_layout(VALUE self, VALUE val)
{
    _SELF(self)->layout = RVAL2PANGOLAYOUT(val);
    return self;
}

static VALUE
rg_start_index(VALUE self)
{
    return INT2NUM(_SELF(self)->start_index);
}

static VALUE
rg_set_start_index(VALUE self, VALUE val)
{
    _SELF(self)->start_index = NUM2INT(val);
    return self;
}

static VALUE
rg_length(VALUE self)
{
    return INT2NUM(_SELF(self)->length);
}

static VALUE
rg_set_length(VALUE self, VALUE val)
{
    _SELF(self)->length = NUM2INT(val);
    return self;
}

#if PANGO_CHECK_VERSION(1,2,0)
static VALUE
rg_runs(VALUE self)
{
    GSList* list = _SELF(self)->runs;
    VALUE ary = rb_ary_new();
    while (list) {
        PangoGlyphItem* old_item = (PangoGlyphItem*)list->data; 
        PangoGlyphItem new_item;

        new_item.item = pango_item_copy(old_item->item);
        new_item.glyphs = pango_glyph_string_copy(old_item->glyphs);

        rb_ary_push(ary, PANGOGLYPHITEM2RVAL(&new_item));
        list = list->next;
    }
    return ary;
}
#endif

struct layout_line_set_runs_args {
    PangoLayoutLine *line;
    VALUE ary;
    long n;
    GSList *result;
};

static VALUE
layout_line_set_runs_body(VALUE value)
{
    struct layout_line_set_runs_args *args = (struct layout_line_set_runs_args *)value;
    long i;

    for (i = 0; i < args->n; i++)
        args->result = g_slist_append(args->result,
                                      RVAL2PANGOGLYPHITEM(RARRAY_PTR(args->ary)[i]));

    g_slist_free(args->line->runs);
    args->line->runs = args->result;

    return Qnil;
}

static G_GNUC_NORETURN VALUE
layout_line_set_runs_rescue(VALUE value)
{
    g_slist_free(((struct layout_line_set_runs_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static VALUE
rg_set_runs(VALUE self, VALUE attrs)
{
    struct layout_line_set_runs_args args;
    args.line = _SELF(self);
    args.ary = rb_ary_to_ary(attrs);
    args.n = RARRAY_LEN(args.ary);
    args.result = NULL;

    rb_rescue(layout_line_set_runs_body, (VALUE)&args,
              layout_line_set_runs_rescue, (VALUE)&args);

    return self;
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
rg_paragraph_start_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->is_paragraph_start);
}

static VALUE
rg_set_paragraph_start(VALUE self, VALUE val)
{
    _SELF(self)->is_paragraph_start = RVAL2CBOOL(val);
    return self;
}

static VALUE
rg_resolved_dir(VALUE self)
{
    return UINT2NUM(_SELF(self)->resolved_dir);
}

static VALUE
rg_set_resolved_dir(VALUE self, VALUE val)
{
    _SELF(self)->resolved_dir = NUM2UINT(val);
    return self;
}
#endif

void
Init_pango_layout_line(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_LAYOUT_LINE, "LayoutLine", mPango);

    RG_DEF_METHOD(extents, 0);
    RG_DEF_METHOD(pixel_extents, 0);
    RG_DEF_METHOD(index_to_x, 2);
    RG_DEF_METHOD(x_to_index, 1); 
    RG_DEF_METHOD(get_x_ranges, 2); 
    RG_DEF_METHOD(layout, 0); 
    RG_DEF_METHOD(set_layout, 1); 
    RG_DEF_METHOD(start_index, 0); 
    RG_DEF_METHOD(set_start_index, 1); 
    RG_DEF_METHOD(length, 0); 
    RG_DEF_METHOD(set_length, 1); 
#if PANGO_CHECK_VERSION(1,2,0)
    RG_DEF_METHOD(runs, 0); 
#endif
    RG_DEF_METHOD(set_runs, 1); 

#if PANGO_CHECK_VERSION(1,4,0)
    RG_DEF_METHOD_P(paragraph_start, 0); 
    RG_DEF_METHOD(set_paragraph_start, 1); 
    RG_DEF_METHOD(resolved_dir, 0); 
    RG_DEF_METHOD(set_resolved_dir, 1); 
#endif
}
