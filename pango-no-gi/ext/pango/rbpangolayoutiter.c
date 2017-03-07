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

#define RG_TARGET_NAMESPACE cLayoutIter
#define _SELF(r) (RVAL2PANGOLAYOUTITER(r))

/**********************************/
#ifndef HAVE_PANGO_LAYOUT_ITER_GET_TYPE
static PangoLayoutIter *
layout_iter_copy(const PangoLayoutIter *ref)
{
  return (PangoLayoutIter *)ref;
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
rg_next_run_bang(VALUE self)
{
    return CBOOL2RVAL(pango_layout_iter_next_run(_SELF(self)));
}

static VALUE
rg_next_char_bang(VALUE self)
{
    return CBOOL2RVAL(pango_layout_iter_next_char(_SELF(self)));
}

static VALUE
rg_next_cluster_bang(VALUE self)
{
    return CBOOL2RVAL(pango_layout_iter_next_cluster(_SELF(self)));
}

static VALUE
rg_next_line_bang(VALUE self)
{
    return CBOOL2RVAL(pango_layout_iter_next_line(_SELF(self)));
}

static VALUE
rg_at_last_line_p(VALUE self)
{
    return CBOOL2RVAL(pango_layout_iter_at_last_line(_SELF(self)));
}

static VALUE
rg_index(VALUE self)
{
    return INT2NUM(pango_layout_iter_get_index(_SELF(self)));
}

static VALUE
rg_baseline(VALUE self)
{
    return INT2NUM(pango_layout_iter_get_baseline(_SELF(self)));
}

static VALUE
rg_run(VALUE self)
{
    PangoLayoutRun* run = pango_layout_iter_get_run(_SELF(self));
    return PANGOGLYPHITEM2RVAL(run);
}

static VALUE
rg_line(VALUE self)
{
    return PANGOLAYOUTLINE2RVAL(pango_layout_iter_get_line(_SELF(self)));
}

static VALUE
rg_char_extents(VALUE self)
{
    PangoRectangle logical_rect;

    pango_layout_iter_get_char_extents(_SELF(self), &logical_rect);
    return PANGORECTANGLE2RVAL(&logical_rect);
}

static VALUE
rg_cluster_extents(VALUE self)
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_iter_get_cluster_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(PANGORECTANGLE2RVAL(&ink_rect),
                        PANGORECTANGLE2RVAL(&logical_rect));
}

static VALUE
rg_run_extents(VALUE self)
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_iter_get_run_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(PANGORECTANGLE2RVAL(&ink_rect),
                        PANGORECTANGLE2RVAL(&logical_rect));
}

static VALUE
rg_line_yrange(VALUE self)
{
    int y0, y1;
    pango_layout_iter_get_line_yrange(_SELF(self), &y0, &y1);
    return rb_assoc_new(INT2NUM(y0), INT2NUM(y1));
}

static VALUE
rg_line_extents(VALUE self)
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_iter_get_line_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(PANGORECTANGLE2RVAL(&ink_rect),
                        PANGORECTANGLE2RVAL(&logical_rect));
}
static VALUE
rg_layout_extents(VALUE self)
{
    PangoRectangle ink_rect, logical_rect;

    pango_layout_iter_get_layout_extents(_SELF(self), &ink_rect, &logical_rect);

    return rb_assoc_new(PANGORECTANGLE2RVAL(&ink_rect),
                        PANGORECTANGLE2RVAL(&logical_rect));
}

void
Init_pango_layout_iter(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_LAYOUT_ITER, "LayoutIter", mPango);

    rbgobj_boxed_not_copy_obj(PANGO_TYPE_LAYOUT_ITER);

    RG_DEF_METHOD_BANG(next_run, 0);
    RG_DEF_METHOD_BANG(next_char, 0);
    RG_DEF_METHOD_BANG(next_cluster, 0);
    RG_DEF_METHOD_BANG(next_line, 0);
    RG_DEF_METHOD_P(at_last_line, 0);
    /* for backword compatibility. :< */
    RG_DEF_ALIAS("at_last_line!", "at_last_line?");
    RG_DEF_METHOD(index, 0);
    RG_DEF_METHOD(baseline, 0);
    RG_DEF_METHOD(run, 0);
    RG_DEF_METHOD(line, 0);
    RG_DEF_METHOD(char_extents, 0);
    RG_DEF_METHOD(cluster_extents, 0);
    RG_DEF_METHOD(run_extents, 0);
    RG_DEF_METHOD(line_yrange, 0);
    RG_DEF_METHOD(line_extents, 0);
    RG_DEF_METHOD(layout_extents, 0);

}
