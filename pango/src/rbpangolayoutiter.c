/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangolayoutiter.c -

  $Author: mutoh $
  $Date: 2002/12/31 07:00:58 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(r) ((PangoLayoutIter*)RVAL2BOXED(r, PANGO_TYPE_LAYOUT_ITER))

/**********************************/
static PangoLayoutIter*
layout_iter_copy(ref)
    const PangoLayoutIter* ref;
{
  return (PangoLayoutIter*)ref;
}

GType
pango_layout_iter_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoLayoutIter",
                    (GBoxedCopyFunc)layout_iter_copy,
                    (GBoxedFreeFunc)pango_layout_iter_free);
    return our_type;
}
/**********************************/

static VALUE
layout_iter_next_run(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_layout_iter_next_run(_SELF(self)));
}

static VALUE
layout_iter_next_char(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_layout_iter_next_char(_SELF(self)));
}

static VALUE
layout_iter_next_cluster(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_layout_iter_next_cluster(_SELF(self)));
}

static VALUE
layout_iter_next_line(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_layout_iter_next_line(_SELF(self)));
}

static VALUE
layout_iter_at_last_line(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_layout_iter_at_last_line(_SELF(self)));
}

static VALUE
layout_iter_get_index(self)
    VALUE self;
{
    return INT2NUM(pango_layout_iter_get_index(_SELF(self)));
}

static VALUE
layout_iter_get_baseline(self)
    VALUE self;
{
    return INT2NUM(pango_layout_iter_get_baseline(_SELF(self)));
}

/*
PangoLayoutRun* pango_layout_iter_get_run   (PangoLayoutIter *iter);
PangoLayoutLine* pango_layout_iter_get_line (PangoLayoutIter *iter);
void        pango_layout_iter_get_char_extents
                                            (PangoLayoutIter *iter,
                                             PangoRectangle *logical_rect);
void        pango_layout_iter_get_cluster_extents
                                            (PangoLayoutIter *iter,
                                             PangoRectangle *ink_rect,
                                             PangoRectangle *logical_rect);
void        pango_layout_iter_get_run_extents
                                            (PangoLayoutIter *iter,
                                             PangoRectangle *ink_rect,
                                             PangoRectangle *logical_rect);
*/

static VALUE
layout_iter_get_line_yrange(self)
    VALUE self;
{
    int y0, y1;
    pango_layout_iter_get_line_yrange(_SELF(self), &y0, &y1);
    return rb_ary_new3(2, NUM2INT(y0), NUM2INT(y1));
}

/*
void        pango_layout_iter_get_line_extents
                                            (PangoLayoutIter *iter,
                                             PangoRectangle *ink_rect,
                                             PangoRectangle *logical_rect);
void        pango_layout_iter_get_layout_extents
                                            (PangoLayoutIter *iter,
                                             PangoRectangle *ink_rect,
*/

void
Init_pango_layout_iter()
{
    VALUE pIter = G_DEF_CLASS(PANGO_TYPE_LAYOUT_ITER, "LayoutIter", mPango);

    rb_define_method(pIter, "next_run!", layout_iter_next_run, 0);
    rb_define_method(pIter, "next_char!", layout_iter_next_char, 0);
    rb_define_method(pIter, "next_cluster!", layout_iter_next_cluster, 0);
    rb_define_method(pIter, "next_line!", layout_iter_next_line, 0);
    rb_define_method(pIter, "at_last_line!", layout_iter_at_last_line, 0);
    rb_define_method(pIter, "index", layout_iter_get_index, 0);
    rb_define_method(pIter, "baseline", layout_iter_get_baseline, 0);
    rb_define_method(pIter, "line_yrange", layout_iter_get_line_yrange, 0);
   
}
