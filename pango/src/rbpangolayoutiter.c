/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangolayoutiter.c -

  $Author: mutoh $
  $Date: 2005/03/05 16:23:46 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(r) ((PangoLayoutIter*)RVAL2BOXED(r, PANGO_TYPE_LAYOUT_ITER))

/**********************************/
#if ! HAVE_PANGO_LAYOUT_ITER_GET_TYPE
static PangoLayoutIter*
layout_iter_copy(ref)
    const PangoLayoutIter* ref;
{
#if PANGO_CHECK_VERSION(1,4,0)
  PangoLayoutIter* new_ref;
  g_return_val_if_fail (ref != NULL, NULL);
  /* This may be broken on pango-1.2.x, but 
     for backward compatibility. */
  new_ref = g_new(PangoLayoutIter, 1);
  *new_ref = *ref;
  return new_ref;
#else
  return (PangoLayoutIter*)ref;
#endif
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
#endif
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

static VALUE
layout_iter_get_run(self)
    VALUE self;
{
    PangoLayoutRun* run = pango_layout_iter_get_run(_SELF(self));
    return run ? BOXED2RVAL(run, PANGO_TYPE_GLYPH_ITEM) : Qnil;
}

static VALUE
layout_iter_get_line(self)
    VALUE self;
{
    return BOXED2RVAL(pango_layout_iter_get_line, PANGO_TYPE_LAYOUT_LINE);
}

static VALUE
layout_iter_get_char_extents(self)
    VALUE self;
{
    PangoRectangle logical_rect;
    
    pango_layout_iter_get_char_extents(_SELF(self), &logical_rect);
    return BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE);
}

static VALUE
layout_iter_get_cluster_extents(self)
    VALUE self;
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_iter_get_cluster_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_iter_get_run_extents(self)
    VALUE self;
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_iter_get_run_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));
}

static VALUE
layout_iter_get_line_yrange(self)
    VALUE self;
{
    int y0, y1;
    pango_layout_iter_get_line_yrange(_SELF(self), &y0, &y1);
    return rb_assoc_new(INT2NUM(y0), INT2NUM(y1));
}

static VALUE
layout_iter_get_line_extents(self)
    VALUE self;
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_iter_get_line_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));
}
static VALUE
layout_iter_get_layout_extents(self)
    VALUE self;
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_iter_get_layout_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));
}

void
Init_pango_layout_iter()
{
    VALUE pIter = G_DEF_CLASS(PANGO_TYPE_LAYOUT_ITER, "LayoutIter", mPango);

    rbgobj_boxed_not_copy_obj(PANGO_TYPE_LAYOUT_ITER);

    rb_define_method(pIter, "next_run!", layout_iter_next_run, 0);
    rb_define_method(pIter, "next_char!", layout_iter_next_char, 0);
    rb_define_method(pIter, "next_cluster!", layout_iter_next_cluster, 0);
    rb_define_method(pIter, "next_line!", layout_iter_next_line, 0);
    rb_define_method(pIter, "at_last_line!", layout_iter_at_last_line, 0);
    rb_define_method(pIter, "index", layout_iter_get_index, 0);
    rb_define_method(pIter, "baseline", layout_iter_get_baseline, 0);
    rb_define_method(pIter, "run", layout_iter_get_run, 0);
    rb_define_method(pIter, "line", layout_iter_get_line, 0);
    rb_define_method(pIter, "char_extents", layout_iter_get_char_extents, 0);
    rb_define_method(pIter, "cluster_extents", layout_iter_get_cluster_extents, 0);
    rb_define_method(pIter, "run_extents", layout_iter_get_run_extents, 0);
    rb_define_method(pIter, "line_yrange", layout_iter_get_line_yrange, 0);
    rb_define_method(pIter, "line_extents", layout_iter_get_line_extents, 0);
    rb_define_method(pIter, "layout_extents", layout_iter_get_layout_extents, 0);
   
}
