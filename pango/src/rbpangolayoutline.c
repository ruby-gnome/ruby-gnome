/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangolayoutline.c -

  $Author: mutoh $
  $Date: 2003/02/01 17:13:25 $

  Copyright (C) 2002,2003 Masao Mutoh
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

/*
void        pango_layout_line_get_extents   (PangoLayoutLine *line,
                                             PangoRectangle *ink_rect,
                                             PangoRectangle *logical_rect);
void        pango_layout_line_get_pixel_extents
                                            (PangoLayoutLine *layout_line,
                                             PangoRectangle *ink_rect,
                                             PangoRectangle *logical_rect);
*/

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

/*
void        pango_layout_line_get_x_ranges  (PangoLayoutLine *line,
                                             int start_index,
                                             int end_index,
                                             int **ranges,
                                             int *n_ranges);
*/

void
Init_pango_layout_line()
{
    VALUE pLine = G_DEF_CLASS(PANGO_TYPE_LAYOUT_LINE, "LayoutLine", mPango);

    rb_define_method(pLine, "index_to_x", layout_line_index_to_x, 2);
    rb_define_method(pLine, "x_to_index", layout_line_x_to_index, 1); 
}
