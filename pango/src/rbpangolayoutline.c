/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangolayoutline.c -

  $Author: mutoh $
  $Date: 2005/02/15 06:19:54 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"


#define _SELF(r) ((PangoLayoutLine*)RVAL2BOXED(r, PANGO_TYPE_LAYOUT_LINE))

/**********************************/
static PangoLayoutLine*
layout_line_copy(ref)
    const PangoLayoutLine* ref;
{
  PangoLayoutLine* new_ref;
  g_return_val_if_fail (ref != NULL, NULL);
  new_ref = g_new(PangoLayoutLine, 1);
  *new_ref = *ref;
  pango_layout_line_ref(new_ref);
  return new_ref;
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
/**********************************/

static VALUE
layout_line_get_extents(self)
    VALUE self;
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_line_get_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_line_get_pixel_extents(self)
    VALUE self;
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_line_get_pixel_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_line_index_to_x(self, index, trailing)
    VALUE self, index, trailing;
{
    int x_pos;
    pango_layout_line_index_to_x(_SELF(self), NUM2INT(index), 
                                 RTEST(trailing), &x_pos);
    return INT2NUM(x_pos);
}

static VALUE
layout_line_x_to_index(self, x_pos)
    VALUE self, x_pos;
{
    int index, trailing;
    
    gboolean ret = pango_layout_line_x_to_index(_SELF(self), NUM2INT(x_pos),
                                                &index, &trailing);
    return rb_ary_new3(CBOOL2RVAL(ret), INT2NUM(index), INT2NUM(trailing));
}

static VALUE
layout_line_get_x_ranges(self, start_index, end_index)
    VALUE self, start_index, end_index;
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
layout_line_get_layout(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->layout);
}

static VALUE
layout_line_set_layout(self, val)
    VALUE self, val;
{
    _SELF(self)->layout = PANGO_LAYOUT(RVAL2GOBJ(val));
    return self;
}

static VALUE
layout_line_get_start_index(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->start_index);
}

static VALUE
layout_line_set_start_index(self, val)
    VALUE self, val;
{
    _SELF(self)->start_index = NUM2INT(val);
    return self;
}

static VALUE
layout_line_get_length(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->length);
}

static VALUE
layout_line_set_length(self, val)
    VALUE self, val;
{
    _SELF(self)->length = NUM2INT(val);
    return self;
}

static VALUE
layout_line_get_runs(self)
    VALUE self;
{
    return GSLIST2ARY2(_SELF(self)->runs, PANGO_TYPE_GLYPH_ITEM);
}

static VALUE
layout_line_set_runs(self, ary)
    VALUE self;
{
    int i, len;
    GSList* list = NULL;

    len = RARRAY(ary)->len;

    for (i = 0; i < len; i++) {
        g_slist_append(list, RVAL2BOXED(RARRAY(ary)->ptr[i], 
                                        PANGO_TYPE_GLYPH_ITEM));
    }

    if (_SELF(self)->runs) {
        g_slist_free(_SELF(self)->runs);
    }
    _SELF(self)->runs = list;

    return self;
}

static VALUE
layout_line_is_paragraph_start(self)
    VALUE self;
{
    return CBOOL2RVAL(_SELF(self)->is_paragraph_start);
}

static VALUE
layout_line_set_paragraph_start(self, val)
    VALUE self, val;
{
    _SELF(self)->is_paragraph_start = RTEST(val);
    return self;
}

static VALUE
layout_line_get_resolved_dir(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->resolved_dir);
}

static VALUE
layout_line_set_resolved_dir(self, val)
    VALUE self, val;
{
    _SELF(self)->resolved_dir = NUM2UINT(val);
    return self;
}

void
Init_pango_layout_line()
{
    VALUE pLine = G_DEF_CLASS(PANGO_TYPE_LAYOUT_LINE, "LayoutLine", mPango);

    rb_define_method(pLine, "extents", layout_line_get_extents, 0);
    rb_define_method(pLine, "pixel_extents", layout_line_get_pixel_extents, 0);
    rb_define_method(pLine, "index_to_x", layout_line_index_to_x, 2);
    rb_define_method(pLine, "x_to_index", layout_line_x_to_index, 1); 
    rb_define_method(pLine, "get_x_ranges", layout_line_get_x_ranges, 2); 
    rb_define_method(pLine, "layout", layout_line_get_layout, 0); 
    rb_define_method(pLine, "set_layout", layout_line_set_layout, 1); 
    rb_define_method(pLine, "start_index", layout_line_get_start_index, 0); 
    rb_define_method(pLine, "set_start_index", layout_line_set_start_index, 1); 
    rb_define_method(pLine, "length", layout_line_get_length, 0); 
    rb_define_method(pLine, "set_length", layout_line_set_length, 1); 
    rb_define_method(pLine, "runs", layout_line_get_runs, 0); 
    rb_define_method(pLine, "set_runs", layout_line_set_runs, 1); 
    rb_define_method(pLine, "paragraph_start?", layout_line_is_paragraph_start, 0); 
    rb_define_method(pLine, "set_paragraph_start", layout_line_set_paragraph_start, 1); 
    rb_define_method(pLine, "resolved_dir", layout_line_get_resolved_dir, 0); 
    rb_define_method(pLine, "set_resolved_dir", layout_line_set_resolved_dir, 1); 

    G_DEF_SETTERS(pLine);
}
