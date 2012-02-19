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

#define RG_TARGET_NAMESPACE cContext
#define _SELF(self) (RVAL2PANGOCONTEXT(self))

static VALUE
rg_itemize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2, arg3, arg4, arg5, arg6;
    GList* list;

    rb_scan_args(argc, argv, "42", &arg1, &arg2, &arg3, &arg4, &arg5, &arg6);

    if (TYPE(arg1) == T_STRING) {
        list = pango_itemize(_SELF(self), 
                             RVAL2CSTR(arg1),      /* text */ 
                             NUM2INT(arg2),        /* start_index */ 
                             NUM2INT(arg3),        /* length */
                             RVAL2PANGOATTRLIST(arg4), /* attrs */
                             NIL_P(arg5) ? NULL : RVAL2PANGOATTRITERATOR(arg5)); /* cached_iter */
    } else {
#ifdef HAVE_PANGO_ITEMIZE_WITH_BASE_DIR
        list = pango_itemize_with_base_dir(_SELF(self), 
                                           RVAL2PANGODIRECTION(arg1), /* base_dir */
                                           RVAL2CSTR(arg2),      /* text */ 
                                           NUM2INT(arg3),        /* start_index */ 
                                           NUM2INT(arg4),        /* length */
                                           RVAL2PANGOATTRLIST(arg5), /* attrs */
                                           NIL_P(arg6) ? NULL : RVAL2PANGOATTRITERATOR(arg6)); /* cached_iter */
#else
        rb_warn("Pango::Context#itemize(base_dir, text, start_index, length, attrs, cached_iter) isn't supported on this environment.");
        list = pango_itemize(_SELF(self), 
                             RVAL2CSTR(arg1),      /* text */ 
                             NUM2INT(arg2),        /* start_index */ 
                             NUM2INT(arg3),        /* length */
                             RVAL2PANGOATTRLIST(arg4), /* attrs */
                             NIL_P(arg5) ? NULL : RVAL2PANGOATTRITERATOR(arg5)); /* cached_iter */
#endif
    }
    return GLIST2ARY2F(list, PANGO_TYPE_ITEM);
}

/* Move to Pango module (rbpangomain.c)
GList*      pango_reorder_items             (GList *logical_items);
*/

#ifdef PANGO_ENABLE_BACKEND
static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, pango_context_new());
    return Qnil;
}

static VALUE
rg_set_font_map(VALUE self, VALUE font_map)
{
    pango_context_set_font_map(_SELF(self), RVAL2PANGOFONTMAP(font_map));
    return self;
}

#if PANGO_CHECK_VERSION(1,6,0)
static VALUE
rg_font_map(VALUE self)
{
    return GOBJ2RVAL(pango_context_get_font_map(_SELF(self)));
}
#endif
#endif /* PANGO_ENABLE_BACKEND */

static VALUE
rg_font_description(VALUE self)
{
    PangoFontDescription* ret = pango_context_get_font_description(_SELF(self));
    return PANGOFONTDESCRIPTION2RVAL(ret);
}

static VALUE
rg_set_font_description(VALUE self, VALUE desc)
{
    pango_context_set_font_description(_SELF(self), RVAL2PANGOFONTDESCRIPTION(desc));
    return self;
}

static VALUE
rg_language(VALUE self)
{
    PangoLanguage* ret = pango_context_get_language(_SELF(self));
    return PANGOLANGUAGE2RVAL(ret);
}

static VALUE
rg_set_language(VALUE self, VALUE lang)
{
    pango_context_set_language(_SELF(self), 
                               RVAL2PANGOLANGUAGE(lang));
    return self;
}

static VALUE
rg_base_dir(VALUE self)
{
    return PANGODIRECTION2RVAL(pango_context_get_base_dir(_SELF(self)));
}

static VALUE
rg_set_base_dir(VALUE self, VALUE direction)
{
    pango_context_set_base_dir(_SELF(self), RVAL2PANGODIRECTION(direction));
    return self;
}

#if PANGO_CHECK_VERSION(1,16,0)
static VALUE
rg_base_gravity(VALUE self)
{
    return PANGOGRAVITY2RVAL(pango_context_get_base_gravity(_SELF(self)));
}

static VALUE
rg_set_base_gravity(VALUE self, VALUE gravity)
{
    pango_context_set_base_gravity(_SELF(self), RVAL2PANGOGRAVITY(gravity));
    return self;
}

static VALUE
rg_gravity_hint(VALUE self)
{
    return PANGOGRAVITYHINT2RVAL(pango_context_get_gravity_hint(_SELF(self)));
}

static VALUE
rg_set_gravity_hint(VALUE self, VALUE gravity_hint)
{
    pango_context_set_gravity_hint(_SELF(self), RVAL2PANGOGRAVITYHINT(gravity_hint));
    return self;
}
#endif

#if PANGO_CHECK_VERSION(1,6,0)
static VALUE
rg_matrix(VALUE self)
{
    const PangoMatrix* matrix = pango_context_get_matrix(_SELF(self));
    return PANGOMATRIX2RVAL((PangoMatrix*)matrix);
}

static VALUE
rg_set_matrix(VALUE self, VALUE matrix)
{
    pango_context_set_matrix(_SELF(self), 
                             RVAL2PANGOMATRIX(matrix));
    return self;
}
#endif

static VALUE
rg_load_font(VALUE self, VALUE desc)
{
    return GOBJ2RVAL(pango_context_load_font(_SELF(self), RVAL2PANGOFONTDESCRIPTION(desc)));
}

static VALUE
rg_load_fontset(VALUE self, VALUE desc, VALUE lang)
{
    return GOBJ2RVAL(pango_context_load_fontset(_SELF(self),
                                                RVAL2PANGOFONTDESCRIPTION(desc), RVAL2PANGOLANGUAGE(lang)));
}

static VALUE
rg_get_metrics(int argc, VALUE *argv, VALUE self)
{
    VALUE desc, lang;

    rb_scan_args(argc, argv, "11", &desc, &lang);

    return PANGOFONTMETRICS2RVAL(pango_context_get_metrics(_SELF(self), 
                                                           RVAL2PANGOFONTDESCRIPTION(desc), 
                                                           NIL_P(lang) ? NULL : RVAL2PANGOLANGUAGE(lang)));
}

static VALUE
rg_families(VALUE self)
{
    int n_families;
    PangoFontFamily** families;
    int i;
    VALUE result;

    pango_context_list_families(_SELF(self),
                                &families,
                                &n_families);

    result = rb_ary_new2(n_families);
    for (i = 0; i < n_families; i++)
      rb_ary_store(result, i, GOBJ2RVAL(families[i]));

    g_free(families);

    return result;
}

#if PANGO_CHECK_VERSION(1,10,0)
#  ifdef HAVE_RB_CAIRO_H
static VALUE
rg_set_font_options(VALUE self, VALUE options)
{
    if (NIL_P(options))
        pango_cairo_context_set_font_options(_SELF(self), NULL);
    else
        pango_cairo_context_set_font_options(_SELF(self),
                                             RVAL2CRFONTOPTIONS(options));
    return self;
}

static VALUE
rg_font_options(VALUE self)
{
    const cairo_font_options_t *options;
    options = pango_cairo_context_get_font_options(_SELF(self));
    if (options)
        return CRFONTOPTIONS2RVAL(cairo_font_options_copy(options));
    else
        return Qnil;
}

static VALUE
rg_set_resolution(VALUE self, VALUE dpi)
{
    pango_cairo_context_set_resolution(_SELF(self), NUM2DBL(dpi));
    return self;
}

static VALUE
rg_resolution(VALUE self)
{
    return rb_float_new(pango_cairo_context_get_resolution(_SELF(self)));
}
#  endif
#endif

static VALUE
rg_list_families(VALUE self)
{
    rb_warn("Deprecated. Use Pango::Context#families instead.");
    return rg_families(self);
}

void
Init_pango_context(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_CONTEXT, "Context", mPango);

    RG_DEF_METHOD(itemize, -1);

#ifdef PANGO_ENABLE_BACKEND
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_font_map, 1);
#if PANGO_CHECK_VERSION(1,6,0)
    RG_DEF_METHOD(font_map, 0);
#endif
#endif /* PANGO_ENABLE_BACKEND */
    RG_DEF_METHOD(font_description, 0);
    RG_DEF_METHOD(set_font_description, 1);
    RG_DEF_METHOD(language, 0);
    RG_DEF_METHOD(set_language, 1);
    RG_DEF_METHOD(base_dir, 0);
    RG_DEF_METHOD(set_base_dir, 1);
#if PANGO_CHECK_VERSION(1,16,0)
    RG_DEF_METHOD(base_gravity, 0);
    RG_DEF_METHOD(set_base_gravity, 1);
    RG_DEF_METHOD(gravity_hint, 0);
    RG_DEF_METHOD(set_gravity_hint, 1);
#endif
#if PANGO_CHECK_VERSION(1,6,0)
    RG_DEF_METHOD(matrix, 0);
    RG_DEF_METHOD(set_matrix, 1);
#endif
    RG_DEF_METHOD(load_font, 1);
    RG_DEF_METHOD(load_fontset, 2);
    RG_DEF_METHOD(get_metrics, -1);
    RG_DEF_METHOD(families, 0);

#if PANGO_CHECK_VERSION(1,10,0)
#  ifdef HAVE_RB_CAIRO_H
    RG_DEF_METHOD(set_font_options, 1);
    RG_DEF_METHOD(font_options, 0);
    RG_DEF_METHOD(set_resolution, 1);
    RG_DEF_METHOD(resolution, 0);
#  endif
#endif

    /* This will remove 2 or 3 releases later since 0.14.0. */
    RG_DEF_METHOD(list_families, 0);

    /* PangoDirection */
    G_DEF_CLASS(PANGO_TYPE_DIRECTION, "Direction", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_DIRECTION, "PANGO_");
}
